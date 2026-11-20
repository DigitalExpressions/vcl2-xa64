# Copyright 2026 DigitalExpressions Sweden
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

"""Exercise real profile configuration, emitted flags and rejection controls."""
import argparse
import json
import os
from pathlib import Path
import subprocess
import sys
from windows_test_profiles import resolve_profile, audit_commands


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--driver',choices=['msvc','clang'],required=True)
    parser.add_argument('--compiler',required=True)
    args=parser.parse_args()
    root=Path(__file__).resolve().parents[1]
    import datetime
    output=root/'build'/('windows-profile-controls-'+args.driver+'-'+datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%S%fZ'))
    output.mkdir(parents=True,exist_ok=False)
    rows=[]
    def run(label,command,success=True):
        r=subprocess.run(list(map(str,command)),capture_output=True,env=dict(os.environ,PYTHONDONTWRITEBYTECODE='1'))
        (output/(label+'.log')).write_bytes(r.stdout+r.stderr)
        rows.append(dict(label=label,args=list(map(str,command)),exit_code=r.returncode,expected_success=success))
        (output/'commands.json').write_text(json.dumps(rows,indent=2)+'\n')
        assert (r.returncode==0)==success,(label,r.returncode,output)
        return r
    runner=root/'tools/run_windows_validation.py'
    run('CLI-default-plan',[sys.executable,'-B',runner,'--driver',args.driver,'--mode','Release','--plan'])
    plan=json.loads((output/'CLI-default-plan.log').read_text());assert plan['profile']=='upstream-optimized' and plan['concurrency']==3 and plan['timeout_seconds']==60
    for i,extra in enumerate([['--profile','rtc-diagnostics'],['--profile','strict-ubsan'],['--profile','unknown']]):
        run('CLI-reject-'+str(i),[sys.executable,'-B',runner,'--driver',args.driver,'--mode','Release','--plan',*extra],success=False)
    if args.driver=='msvc':
        run('CLI-reject-Debug',[sys.executable,'-B',runner,'--driver','msvc','--mode','Debug','--plan'],success=False)
    positives=[('upstream-optimized','Release',False),('strict-validation','Release',False)]
    if args.driver=='clang':positives.append(('strict-ubsan','Debug',True))
    for profile,mode,sanitize in positives:
        assert resolve_profile(args.driver,mode,sanitize,profile)==profile
        build=output/profile
        cmd=['cmake','-S',root,'-B',build,'-G','Ninja','-DCMAKE_CXX_COMPILER='+args.compiler,'-DCMAKE_BUILD_TYPE='+mode,'-DVCL_TEST_PROFILE='+profile,'-DVCL_BACKEND=1','-DVCL_BUILD_TESTS=ON','-DCMAKE_EXPORT_COMPILE_COMMANDS=ON','-DVCL_SANITIZE='+('ON' if sanitize else 'OFF')]
        if sanitize:cmd+=['-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded']
        run(profile+'-configure',cmd)
        entries=json.loads((build/'compile_commands.json').read_text())
        audit_commands(entries,args.driver,mode,profile)
        run(profile+'-smoke-build',['cmake','--build',build,'--parallel','3','--target','vcl_example'])
        r=run(profile+'-smoke-run',[build/'tests/vcl_example.exe']);assert r.stdout.split()==[b'2',b'5',b'10',b'17']
        # Exercise the actual runner's standalone control subproject too: its
        # add_subdirectory must receive the same profile, especially for UBSan.
        from types import SimpleNamespace
        from run_windows_validation import WindowsRun, controls
        control_output=output/(profile+'-controls');control_output.mkdir()
        controls(WindowsRun(root,control_output),args.compiler,
                 SimpleNamespace(driver=args.driver,mode=mode,profile=profile,
                                 sanitize=sanitize,testbench_dir=None),'Ninja')
        run(profile+'-inventory',['ctest','--test-dir',build,'--show-only=json-v1'])
        inventory=json.loads((output/(profile+'-inventory.log')).read_text());assert len(inventory['tests'])==40
        assert all({p['name']:p['value'] for p in t['properties']}.get('TIMEOUT')==60 for t in inventory['tests'])
        # Actual compiler commands are the positive control for the independent
        # audit. Inject policy violations and require rejection, not execution.
        ordinary=next(e for e in entries if 'core_128.dir' in e['command'])
        for n,flag in enumerate(['/RTC1','/Od','/O1'] if args.driver=='msvc' else ['-ffast-math','-Ofast','-ffinite-math-only']):
            altered=dict(ordinary,command=ordinary['command']+' '+flag)
            try:audit_commands([altered],args.driver,mode,profile)
            except ValueError:pass
            else:raise AssertionError(('audit accepted injected policy',flag))
    negatives=[('unknown','Release',False,[]),('upstream-optimized','Debug',False,[]),('rtc-diagnostics','Debug',False,[]),('upstream-optimized','Release',True,[]),('upstream-optimized','Release',False,['-DCMAKE_CXX_FLAGS='+('/RTC1' if args.driver=='msvc' else '-ffast-math')])]
    if args.driver=='msvc':negatives.append(('strict-validation','Debug',False,[]))
    else:negatives.append(('strict-ubsan','Release',True,['-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded']))
    for i,(profile,mode,sanitize,extra) in enumerate(negatives):
        run('reject-'+str(i),['cmake','-S',root,'-B',output/('reject-'+str(i)),'-G','Ninja','-DCMAKE_CXX_COMPILER='+args.compiler,'-DCMAKE_BUILD_TYPE='+mode,'-DVCL_BUILD_TESTS=ON','-DVCL_TEST_PROFILE='+profile,'-DVCL_SANITIZE='+('ON' if sanitize else 'OFF'),*extra],success=False)
    (output/'summary.json').write_text(json.dumps(dict(passed=True,driver=args.driver,positive_profiles=len(positives),negative_configurations=len(negatives),commands=rows),indent=2)+'\n')
    print('Profile controls passed:',output)


if __name__=='__main__':main()
