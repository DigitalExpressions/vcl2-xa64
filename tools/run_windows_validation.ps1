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

# Initialize an installed x64 developer environment for manual native testing.
param(
    [Parameter(Mandatory=$true)][ValidateSet('msvc','clang')][string]$Driver,
    [Parameter(Mandatory=$true)][ValidateSet('Release','Debug')][string]$Mode,
    [switch]$Sanitize,
    [ValidateSet('upstream-optimized','strict-validation','strict-ubsan','rtc-diagnostics')][string]$Profile,
    [switch]$Upstream,
    [switch]$Plan
)
$ErrorActionPreference = 'Stop'
$taskVswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$env:PATH = (Split-Path -Parent $taskVswhere) + ';' + $env:PATH
$taskInstallation = & $taskVswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if ($LASTEXITCODE -ne 0 -or -not $taskInstallation) { throw 'No suitable Visual Studio x64 toolchain' }
Import-Module (Join-Path $taskInstallation 'Common7/Tools/Microsoft.VisualStudio.DevShell.dll')
Enter-VsDevShell -VsInstallPath $taskInstallation -SkipAutomaticLocation -DevCmdArguments '-arch=x64 -host_arch=x64' | Out-Null
$taskCompiler = 'cl'
if ($Driver -eq 'clang') {
    $taskCompiler = $null
    $taskCandidates = @(
        (Join-Path $taskInstallation 'VC/Tools/Llvm/x64/bin/clang++.exe'),
        (Join-Path $env:ProgramFiles 'LLVM/bin/clang++.exe')
    )
    foreach ($taskCandidate in $taskCandidates) {
        if (-not (Test-Path -LiteralPath $taskCandidate)) { continue }
        $taskVersion = & $taskCandidate --version
        if ($LASTEXITCODE -eq 0 -and $taskVersion -match 'Target: x86_64-.*windows') {
            $taskCompiler = $taskCandidate
            break
        }
    }
    if (-not $taskCompiler) { throw 'No installed native x64 GNU-driver Clang found' }
}
$taskArguments = @('-B','tools/run_windows_validation.py','--driver',$Driver,'--mode',$Mode,'--compiler',$taskCompiler)
if ($Profile) { $taskArguments += @('--profile',$Profile) }
if ($Sanitize) { $taskArguments += '--sanitize' }
if ($Plan) { $taskArguments += '--plan' }
if ($Upstream) {
    $taskTestbench = 'build/_deps/testbench'
    if (-not (Test-Path -LiteralPath $taskTestbench)) {
        git clone --no-checkout https://github.com/vectorclass/testbench.git $taskTestbench
        if ($LASTEXITCODE -ne 0) { throw 'Pinned testbench clone failed' }
        git -C $taskTestbench checkout --detach a7293702385e5169a5f58b83177f2a43cd59139b
        if ($LASTEXITCODE -ne 0) { throw 'Pinned testbench checkout failed' }
    }
    $taskArguments += @('--testbench-dir',$taskTestbench)
}
& python @taskArguments
exit $LASTEXITCODE
