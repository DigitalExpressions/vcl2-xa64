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

"""Shared command logging, fixture inventory and native validation evidence."""
import datetime
import hashlib
import json
import os
from pathlib import Path
import subprocess
import zipfile

FIXTURES = {"configuration", "hyperbolic_reference", "inverse_pi_reference",
            "trig_reference", "power_roots_reference", "exp_log_reference"}


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


class EvidenceRun:
    def __init__(self, checkout, output):
        self.checkout, self.output = checkout, output
        self.env = os.environ.copy()
        self.env.update(UBSAN_OPTIONS="halt_on_error=1:print_stacktrace=1", PYTHONDONTWRITEBYTECODE="1")

    def command(self, label, args, cwd=None, required=True, timeout=None):
        args = [str(a) for a in args]
        log = self.output / (label + ".log")
        record = dict(label=label, command=args, cwd=str(cwd or self.checkout),
                      start_utc=datetime.datetime.now(datetime.timezone.utc).isoformat())
        with log.open("wb") as stream:
            try:
                process = subprocess.run(args, cwd=cwd or self.checkout, env=self.env,
                                         stdout=stream, stderr=subprocess.STDOUT, timeout=timeout)
                code = process.returncode
            except subprocess.TimeoutExpired:
                code = 124
                record["timed_out"] = True
        record.update(exit_code=code, log=str(log),
                      end_utc=datetime.datetime.now(datetime.timezone.utc).isoformat())
        with (self.output / "commands.jsonl").open("a", encoding="utf-8") as journal:
            journal.write(json.dumps(record) + "\n")
        text = log.read_text(encoding="utf-8", errors="replace")
        print(f"{label}: exit={code} [{log}]", flush=True)
        print("\n".join(line[:220] for line in text.splitlines()[-5:]), flush=True)
        if required and code:
            raise RuntimeError(f"{label} failed; inspect {log}")
        return code, text

    def git(self, label, *args, cwd=None):
        return self.command(label, ["git", *args], cwd=cwd)[1].strip()


def package_evidence(run):
    destination = run.output / "return-evidence.zip"
    with zipfile.ZipFile(destination, "w", compression=zipfile.ZIP_DEFLATED) as archive:
        for path in sorted(run.output.rglob("*")):
            if path.is_file() and (path.suffix in {".log", ".json", ".jsonl", ".xml"} or path.name == "CMakeCache.txt"):
                archive.write(path, path.relative_to(run.output))
    (run.output / "return-evidence.zip.sha256").write_text(sha(destination) + "  return-evidence.zip\n")
    print("Return this compact evidence packet: " + str(destination), flush=True)
