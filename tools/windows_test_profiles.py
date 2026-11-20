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

"""Explicit Windows test policy; no runtime implementation or result waiver."""
import re

PROFILES = ('upstream-optimized', 'strict-validation', 'strict-ubsan', 'rtc-diagnostics')


def resolve_profile(driver, mode, sanitize, profile=None):
    profile = profile or ('strict-ubsan' if sanitize else 'upstream-optimized')
    if profile not in PROFILES:
        raise ValueError('Unknown test profile: ' + profile)
    if profile == 'rtc-diagnostics' or driver == 'msvc' and mode == 'Debug':
        raise ValueError('MSVC RTC diagnostics are deferred until noninteractive reporting is validated')
    if profile == 'upstream-optimized' and (mode != 'Release' or sanitize):
        raise ValueError('upstream-optimized requires Release without sanitizers')
    if profile == 'strict-ubsan' and (driver != 'clang' or mode != 'Debug' or not sanitize):
        raise ValueError('strict-ubsan requires GNU-driver Clang Debug with --sanitize')
    if sanitize and profile != 'strict-ubsan':
        raise ValueError('Select strict-ubsan for sanitizer execution')
    return profile


def audit_commands(entries, driver, mode, profile, allow_higher_isa=False):
    """Audit every emitted command before build/execution, retaining its raw text."""
    if not entries:
        raise ValueError('Missing exported compile commands')
    records = []
    for entry in entries:
        flags = entry['command']
        match = re.search(r'CMakeFiles[/\\]([^/\\]+)\.dir[/\\]', flags)
        if not match:
            raise ValueError('Unrecognized compilation record')
        target = match[1]
        if any(x in flags for x in ('-ffast-math', '-Ofast', '-march=native', '-mcpu=native')):
            raise ValueError('Unexpected math/native target option: ' + target)
        if not allow_higher_isa and any(x in flags for x in ('/arch:AVX', '-mavx')):
            raise ValueError('Higher ISA requires the separately probed manual profile')
        if driver == 'msvc' and (re.search(r'(?i)(?:^|\s)[/-]RTC', flags) or
                                 re.search(r'(?:^|\s)/Od(?:\s|$)', flags)):
            raise ValueError('Interactive RTC/Debug instrumentation is not permitted: ' + target)
        if profile == 'upstream-optimized' or driver == 'msvc' and mode == 'Release':
            expected = '/O2' if driver == 'msvc' else '-O2'
            if expected not in flags or re.search(r'(?:^|\s)(?:/O[d13]|-O[013sgz])(?:\s|$)', flags):
                raise ValueError('Expected unambiguous O2 optimization: ' + target)
        if target not in {'vcl_detect', 'vcl_example'}:
            stress = target == 'fp_classification'
            if driver == 'msvc':
                if ('/fp:fast' in flags) != stress:
                    raise ValueError('Classification-only FP policy mismatch: ' + target)
                if not stress:
                    required = ('/fp:precise', '/fp:except-') if profile == 'upstream-optimized' else ('/fp:strict',)
                    forbidden = ('/fp:strict',) if profile == 'upstream-optimized' else ('/fp:precise', '/fp:except-')
                    if not all(x in flags for x in required) or any(x in flags for x in forbidden):
                        raise ValueError('MSVC FP profile mismatch: ' + target)
            else:
                if ('-ffinite-math-only' in flags) != stress:
                    raise ValueError('Classification-only finite-math policy mismatch: ' + target)
                required = ('-fno-trapping-math', '-fno-math-errno') if profile == 'upstream-optimized' else ('-ffp-contract=off',)
                if not all(x in flags for x in required):
                    raise ValueError('Clang FP profile mismatch: ' + target)
                if profile == 'upstream-optimized' and any(x in flags for x in ('-ffp-contract=off', '-frounding-math')):
                    raise ValueError('Strict policy leaked into optimized profile: ' + target)
            if profile == 'strict-ubsan' and not all(x in flags for x in (
                    '-fsanitize=undefined', '-fno-sanitize-recover=undefined', '-fno-omit-frame-pointer')):
                raise ValueError('Incomplete strict UBSan instrumentation: ' + target)
        records.append(dict(target=target, source=entry['file'], command=flags))
    return records
