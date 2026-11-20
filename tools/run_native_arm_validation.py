#!/usr/bin/env python3
# Copyright 2026 DigitalExpressions Sweden
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Validate a clean VCL checkout on native macOS ARM64; keep evidence under build/."""
import argparse
import datetime
import json
import os
from pathlib import Path
import platform
import re
import shutil
import signal
import subprocess
import sys
import xml.etree.ElementTree as ET

from validation_evidence import EvidenceRun, FIXTURES, package_evidence, sha
from capture_source import verify_capture

BASELINE = "943923a4cce1b3990ee69b37ebdf1c37f0fc67b6"
TESTBENCH = "a7293702385e5169a5f58b83177f2a43cd59139b"
SOURCE_HASHES = {
    "testbench1.cpp": "7411fc27af1055744c7e8b8b4fe64b52859b561dab0f48b9b0278413c71aea27",
    "testbench2.cpp": "6a23124de4495368bb1e343569f7c1c8cbf7c29f5ab0c0c729e9c068fb8c2a5b",
    "testbench3.cpp": "51964802a6789e230ce2fc2ed23e6adbc9ef5153815246b4cbc09a735d2ffc49",
}


def matrix(local_only=False):
    result = []
    for backend, width, local, full in [(2, 512, 42, 1350), (3, 128, 40, 486),
                                       (3, 256, 42, 890), (3, 512, 44, 1292)]:
        for mode in ["Release", "Debug"]:
            result.append(dict(name=("compat" if backend == 2 else "neon") + str(width) + "-" + mode.lower(),
                               backend=backend, width=width, mode=mode,
                               sanitize=mode == "Debug", expected=local if local_only else full))
    return result


class Run(EvidenceRun):
    def binary(self, label, path):
        _, description = self.command(label + "-file", ["file", "-b", path])
        if not description.strip().startswith("Mach-O 64-bit executable arm64"):
            raise RuntimeError(f"Expected a native arm64 executable: {path}: {description}")
        return dict(path=str(path), sha256=sha(path), file=description.strip())


def verify_source(run, expected_commit):
    run.head = run.git("source-head", "rev-parse", "HEAD")
    run.tree = run.git("source-tree", "rev-parse", "HEAD^{tree}")
    if expected_commit and run.head != run.git("expected-commit", "rev-parse", "--verify", expected_commit + "^{commit}"):
        raise RuntimeError("Checkout does not match --expected-commit")
    if run.git("source-status", "status", "--porcelain"):
        raise RuntimeError("Use a clean checkout/worktree; preserve existing changes")
    run.git("source-ignore", "check-ignore", str(run.output))
    run.git("source-ancestry", "merge-base", "--is-ancestor", BASELINE, "HEAD")
    return run.git("source-branch", "branch", "--show-current") or "detached"


def preflight(args):
    if sys.version_info < (3, 9):
        raise RuntimeError("Python 3.9+ is required")
    if platform.system() != "Darwin" or platform.machine().lower() != "arm64":
        raise RuntimeError("Use native arm64 Python in macOS Terminal on the Mac; Rosetta/Linux/Windows cannot run this validation")
    inherited = " ".join(os.environ.get(key, "") for key in ["CXXFLAGS", "CPPFLAGS", "LDFLAGS"])
    if re.search(r"-fsanitize=[^\s]*address", inherited):
        raise RuntimeError("Inherited flags enable ASan; do not retry the documented Apple-host startup hang")
    for tool in ["git", "xcrun", "cmake", "ctest", "file", "sysctl", "sw_vers"]:
        if not shutil.which(tool):
            raise RuntimeError(f"Missing tool: {tool}; inspect existing installations before installing anything")
    translated = subprocess.run(["sysctl", "-in", "sysctl.proc_translated"], capture_output=True, text=True)
    if translated.stdout.strip() == "1":
        raise RuntimeError("Rosetta process detected; open native Terminal and use arm64 Python")
    checkout = args.checkout.resolve()
    if not (checkout / "CMakeLists.txt").is_file():
        raise RuntimeError(f"Clone/import the supplied library bundle first: {checkout}")
    stamp = datetime.datetime.now(datetime.timezone.utc).strftime("%Y%m%dT%H%M%S%fZ")
    output = checkout / "build" / ("mac-m2-native-" + stamp)
    output.mkdir(parents=True, exist_ok=False)
    run = Run(checkout, output)
    args.evidence_run = run
    print(f"Evidence directory: {output}", flush=True)
    if args.capture_manifest:
        captured = verify_capture(checkout,args.capture_manifest.resolve())
        run.head = run.tree = None
        run.captured_source = captured
        branch = 'frozen-uncommitted-source'
        (output/'captured-source.json').write_text(json.dumps(captured,indent=2))
    else:
        branch = verify_source(run, args.expected_commit)
    _, compiler = run.command("compiler-path", ["xcrun", "--find", "clang++"])
    compiler = compiler.strip()
    for label, command in [("os", ["sw_vers"]), ("hardware", ["sysctl", "-n", "machdep.cpu.brand_string"]),
                           ("model", ["sysctl", "-n", "hw.model"]), ("compiler", [compiler, "--version"]),
                           ("sdk-version", ["xcrun", "--sdk", "macosx", "--show-sdk-version"]),
                           ("sdk-path", ["xcrun", "--sdk", "macosx", "--show-sdk-path"]),
                           ("cmake", ["cmake", "--version"]), ("ctest", ["ctest", "--version"])]:
        run.command(label, command)
    _, version = run.command("cmake-minimum", ["cmake", "--version"])
    numbers = re.search(r"(\d+)\.(\d+)", version)
    if not numbers or tuple(map(int, numbers.groups())) < (3, 20):
        raise RuntimeError("CMake 3.20+ is required")
    _, help_text = run.command("ctest-junit-support", ["ctest", "--help"])
    if "--output-junit" not in help_text:
        raise RuntimeError("Select a CTest installation supporting --output-junit")
    generator = "Ninja" if shutil.which("ninja") else "Unix Makefiles"
    if generator == "Unix Makefiles" and not shutil.which("make"):
        raise RuntimeError("Ninja or make is required")
    run.command("build-tool", ["ninja" if generator == "Ninja" else "make", "--version"])
    inventory = dict(head=run.head, tree=run.tree, expected_commit=args.expected_commit, branch=branch, compiler=compiler, generator=generator,
                     python=sys.version, python_executable=sys.executable, machine=platform.machine(),
                     translated_query=dict(exit_code=translated.returncode, stdout=translated.stdout, stderr=translated.stderr),
                     page_size=os.sysconf("SC_PAGE_SIZE"), environment={key: os.environ.get(key) for key in
                     ["CXXFLAGS", "CFLAGS", "CPPFLAGS", "LDFLAGS", "CC", "CXX", "SDKROOT", "MACOSX_DEPLOYMENT_TARGET"]})
    (output / "inventory.json").write_text(json.dumps(inventory, indent=2))
    paths = ([r["path"] for r in run.captured_source["files"]] if args.capture_manifest else run.git("tracked-files", "ls-files", "-z").split("\0"))
    sources = [dict(path=p, sha256=sha(checkout / p)) for p in sorted(paths) if p and not p.endswith(".md")]
    (output / "source-manifest.json").write_text(json.dumps(sources, indent=2))
    return run, compiler, generator


def testbench(run, args):
    if args.local_only:
        return None
    directory = (args.testbench_dir or (run.checkout / "build/_deps/testbench")).resolve()
    if not directory.exists():
        bundle = args.testbench_bundle.resolve() if args.testbench_bundle else None
        if bundle is None or not bundle.is_file():
            raise RuntimeError("Fetch the pinned tests first: sh tools/fetch_testbench.sh build/_deps/testbench; "
                               "or provide --testbench-bundle /path/to/testbench.bundle for offline use")
        run.git("testbench-bundle-verify", "bundle", "verify", str(bundle))
        directory.parent.mkdir(parents=True, exist_ok=True)
        run.command("testbench-clone", ["git", "clone", "--no-checkout", "--branch", "pinned-testbench", bundle, directory])
        run.git("testbench-detach", "checkout", "--detach", TESTBENCH, cwd=directory)
    if run.git("testbench-head", "rev-parse", "HEAD", cwd=directory) != TESTBENCH:
        raise RuntimeError("Existing testbench has a different revision; it was not overwritten")
    if run.git("testbench-status", "status", "--porcelain", cwd=directory):
        raise RuntimeError("Existing testbench is modified; preserve it and select a pristine directory")
    for name, expected in SOURCE_HASHES.items():
        if sha(directory / name) != expected:
            raise RuntimeError("Pinned upstream source hash mismatch: " + name)
    return directory


def probes(run, compiler, generator):
    import resource
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    directory = run.output / "probes"
    directory.mkdir()
    ub = directory / "ubsan.cpp"
    ub.write_text('#include <climits>\n#include <cstdio>\nint main(int argc,char**) {\n'
                  'if(argc==1) { std::puts("positive control"); return 0; }\n'
                  'volatile int x=INT_MAX; volatile int y=x+1; std::printf("UB continued %d\\n",int(y)); return 0; }\n')
    exe = directory / "ubsan"
    run.command("ubsan-compile", [compiler, "-std=c++17", "-arch", "arm64", "-g", "-fsanitize=undefined",
                "-fno-sanitize-recover=undefined", "-fno-omit-frame-pointer", ub, "-o", exe])
    artifacts = [run.binary("ubsan-binary", exe)]
    run.command("ubsan-positive", [exe], timeout=60)
    code, text = run.command("ubsan-negative", [exe, "trigger"], required=False, timeout=60)
    if code in (0, 124) or "runtime error: signed integer overflow" not in text or "UB continued" in text:
        raise RuntimeError("Strict nonrecovering UBSan probe failed; no sanitizer pass can be claimed")
    guard = directory / "guard.cpp"
    guard.write_text('#include "protected_memory.h"\nint main(int argc,char** argv) {\n'
        'const size_t page=vcl_test::page_size(); vcl_test::ProtectedMemory m(3*page); m.writable(page,page);\n'
        'volatile unsigned char* p=m.data(); p[page]=1; p[2*page-1]=2;\n'
        'if(argc==1) { std::printf("page_size=%zu\\n",page); return p[page]+p[2*page-1]!=3; }\n'
        "int mode=argv[1][0]-'0'; size_t offset=mode<2?page-1:2*page; if(mode%2) p[offset]=5; else return p[offset]; return 0; }\n")
    guard_exe = directory / "guard"
    run.command("guard-compile", [compiler, "-std=c++17", "-arch", "arm64", "-I" + str(run.checkout / "tests"), guard, "-o", guard_exe])
    artifacts.append(run.binary("guard-binary", guard_exe))
    run.command("guard-positive", [guard_exe], timeout=60)
    for mode in range(4):
        code, _ = run.command("guard-negative-" + str(mode), [guard_exe, str(mode)], required=False, timeout=60)
        if code not in (-signal.SIGSEGV, -signal.SIGBUS):
            raise RuntimeError("Expected protected-page access fault for guard mode " + str(mode))
    consumer = directory / "runtime-source"
    consumer.mkdir()
    (consumer / "CMakeLists.txt").write_text('cmake_minimum_required(VERSION 3.20)\nproject(runtime_check LANGUAGES CXX)\n'
        'set(VCL_BACKEND 2 CACHE STRING "" FORCE)\nadd_subdirectory("${VCL_CHECKOUT}" library)\n'
        'add_executable(runtime_check main.cpp)\ntarget_link_libraries(runtime_check PRIVATE vcl::runtime)\n')
    (consumer / "main.cpp").write_text('#include "vcl2_xa64.h"\n#include <cstdio>\nint main(){\n'
        'std::printf("arm64=%d neon=%d instrset_detect=%d pointer_bits=%zu long_bits=%zu\\n",'
        'int(VCL_ARCH_ARM64),int(has_neon()),instrset_detect(),sizeof(void*)*8,sizeof(long)*8);\n'
        'return !VCL_ARCH_ARM64 || !has_neon() || instrset_detect()!=0 || sizeof(void*)!=8; }\n')
    build = directory / "runtime-build"
    run.command("runtime-configure", ["cmake", "-S", consumer, "-B", build, "-G", generator,
                "-DVCL_CHECKOUT=" + str(run.checkout), "-DCMAKE_CXX_COMPILER=" + compiler,
                "-DCMAKE_OSX_ARCHITECTURES=arm64", "-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"])
    run.command("runtime-build", ["cmake", "--build", build, "--parallel", "3", "--verbose"])
    artifacts.append(run.binary("runtime-binary", build / "runtime_check"))
    run.command("runtime-run", [build / "runtime_check"], timeout=60)
    (run.output / "probe-binaries.json").write_text(json.dumps(artifacts, indent=2))


def profile(run, compiler, generator, bench, item):
    name = item["name"]
    directory = run.output / name
    record = dict(item, status="failed")
    try:
        run.command(name + "-configure", ["cmake", "-S", run.checkout, "-B", directory, "-G", generator,
            "-DCMAKE_CXX_COMPILER=" + compiler, "-DCMAKE_OSX_ARCHITECTURES=arm64", "-DCMAKE_BUILD_TYPE=" + item["mode"],
            "-DVCL_BUILD_TESTS=ON", "-DVCL_BACKEND=" + str(item["backend"]),
            "-DVCL_ENABLE_EXPERIMENTAL_NEON=ON", "-DVCL_NATIVE_MAX_VECTOR_SIZE=" + str(item["width"]),
            "-DVCL_SANITIZE=" + ("ON" if item["sanitize"] else "OFF"), "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
            "-DVCL_TESTBENCH_DIR=" + (str(bench) if bench else "")])
        if bench:
            for command in json.loads((directory / "compile_commands.json").read_text()):
                if Path(command["file"]).resolve().is_relative_to(bench.resolve()):
                    raise RuntimeError("Compile only include-adapted build copies of the pinned testbench: "+command["file"])
        run.command(name + "-build", ["cmake", "--build", directory, "--parallel", "3", "--verbose"])
        _, text = run.command(name + "-inventory", ["ctest", "--test-dir", directory, "--show-only=json-v1"])
        tests = json.loads(text)["tests"]
        if len(tests) != item["expected"] or not FIXTURES.issubset({t["name"] for t in tests}):
            raise RuntimeError("Registration mismatch; reconcile the actual inventory without removing cases")
        for test in tests:
            properties = {p["name"]: p["value"] for p in test["properties"]}
            if properties.get("TIMEOUT") != 60:
                raise RuntimeError("Changed case deadline: " + test["name"])
        code, _ = run.command(name + "-ctest", ["ctest", "--test-dir", directory, "--output-on-failure",
            "--no-tests=error", "--timeout", "60", "--parallel", "3", "--output-junit", "results.xml"], required=False)
        cases = list(ET.parse(directory / "results.xml").getroot().iter("testcase"))
        failures = [c.attrib["name"] for c in cases if c.find("failure") is not None]
        skipped = [c.attrib["name"] for c in cases if c.find("skipped") is not None]
        record.update(registered=len(tests), executed=len(cases), failed=failures, skipped=skipped,
                      junit=str(directory / "results.xml"), ctest_exit=code)
        artifacts = []
        for test in tests:
            if test["name"] not in FIXTURES:
                artifact = run.binary(name + "-file-" + test["name"], Path(test["command"][0]))
                artifacts.append(dict(artifact, test=test["name"]))
        commands = json.loads((directory / "compile_commands.json").read_text())
        for command in commands:
            flags = command["command"]
            if any(bad in flags for bad in ["-ffast-math", "-Ofast", "-march=native", "-mcpu=native"]):
                raise RuntimeError("Unexpected broad math/CPU option in compile commands")
            if "vcl_detect.dir" in flags or "vcl_example.dir" in flags:
                continue
            if "-ffp-contract=off" not in flags:
                raise RuntimeError("Missing no-contraction option")
            if "-ffinite-math-only" in flags and "fp_classification.dir" not in flags:
                raise RuntimeError("Finite-math stress leaked outside classification")
            if item["sanitize"] and any(option not in flags for option in
                    ["-fsanitize=undefined", "-fno-sanitize-recover=undefined"]):
                raise RuntimeError("Incomplete strict UBSan instrumentation")
        _, example = run.command(name + "-example", [directory / "tests/vcl_example"], timeout=60)
        if example.split() != ["2", "5", "10", "17"]:
            raise RuntimeError("Unexpected example output")
        artifacts.append(run.binary(name + "-example-binary", directory / "tests/vcl_example"))
        (run.output / (name + "-binaries.json")).write_text(json.dumps(artifacts, indent=2))
        test_log = (directory / "Testing/Temporary/LastTest.log").read_text(errors="replace")
        if code or failures or skipped or len(cases) != len(tests) or "runtime error:" in test_log or "UndefinedBehaviorSanitizer:" in test_log:
            raise RuntimeError("Regression failure, skip, timeout or sanitizer diagnostic; inspect retained logs")
        record.update(status="passed", passed=len(cases))
    except (RuntimeError, OSError, ValueError, ET.ParseError) as error:
        record["error"] = str(error)
        print(name + ": " + str(error), file=sys.stderr, flush=True)
    return record


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--checkout", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--capture-manifest",type=Path,help="Explicit frozen-uncommitted-source manifest; validate actual captured bytes, without Git status spoofing")
    parser.add_argument("--expected-commit", help="Require this Git revision; otherwise capture the current clean HEAD")
    parser.add_argument("--testbench-bundle", type=Path, help="Restore the prepared pinned-testbench Git bundle offline")
    parser.add_argument("--testbench-dir", type=Path, help="Reuse an existing clean checkout at the pinned revision")
    parser.add_argument("--local-only", action="store_true", help="Run the 336 local cases instead of the 8036-case pinned matrix")
    parser.add_argument("--plan", action="store_true", help="Print the plan without running tools or requiring a Mac")
    args = parser.parse_args()
    if args.capture_manifest and args.expected_commit:
        parser.error("--capture-manifest and --expected-commit identify different provenance modes")
    items = matrix(args.local_only)
    if args.plan:
        print(json.dumps(dict(expected_commit=args.expected_commit, source_selection="frozen uncommitted capture" if args.capture_manifest else "current clean checkout", minimum_ancestor=BASELINE, testbench=None if args.local_only else TESTBENCH,
                              case_instances=sum(p["expected"] for p in items), timeout_seconds=60, profiles=items), indent=2))
        return 0
    run = None
    try:
        run, compiler, generator = preflight(args)
        bench = testbench(run, args)
        run.command("repository-check", [sys.executable, "-B", "tools/check_repository.py"])
        run.command("integrity-before", [sys.executable, "-B", "tools/regenerate_headers.py", "--check", "--json", str(run.output/"integrity-before.json")])
        probes(run, compiler, generator)
        records = []
        for item in items:
            records.append(profile(run, compiler, generator, bench, item))
            (run.output / "profiles.json").write_text(json.dumps(records, indent=2))
        run.command("integrity-after", [sys.executable,"-B","tools/regenerate_headers.py","--check","--json",str(run.output/"integrity-after.json")])
        if args.capture_manifest:
            unchanged = verify_capture(run.checkout,args.capture_manifest.resolve()) == run.captured_source
        else:
            final_head = run.git("source-final-head", "rev-parse", "HEAD")
            final_tree = run.git("source-final-tree", "rev-parse", "HEAD^{tree}")
            final_status = run.git("source-final-status", "status", "--porcelain")
            unchanged = final_head == run.head and final_tree == run.tree and not final_status
        summary = dict(head=run.head, tree=run.tree, native_arm64=True, captured_source=getattr(run,"captured_source",None), profiles=records, source_unchanged=unchanged,
                       passed=sum(r.get("passed", 0) for r in records), expected=sum(p["expected"] for p in items),
                       success=unchanged and all(r["status"] == "passed" for r in records), evidence=str(run.output))
        (run.output / "summary.json").write_text(json.dumps(summary, indent=2))
        package_evidence(run)
        print(json.dumps(summary, indent=2))
        return 0 if summary["success"] else 1
    except (RuntimeError, OSError, ValueError) as error:
        print("Validation stopped: " + str(error), file=sys.stderr)
        run = run or getattr(args, "evidence_run", None)
        if run:
            (run.output / "preflight-failure.json").write_text(json.dumps(dict(success=False, error=str(error)), indent=2))
            package_evidence(run)
        return 1


if __name__ == "__main__":
    sys.exit(main())
