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

param(
    [string]$ForkCommit = 'HEAD',
    [string]$UpstreamCommit = '667577495efaa36a10cbb5d6cb8c942c23913848',
    [int]$Pairs = 3,
    [int]$Trials = 7,
    [double]$SampleMs = 4,
    [int]$Cpu = -1,
    [string]$Output = '',
    [string]$PortableOutput = '',
    [string]$CandidatePatch = ''
)
$ErrorActionPreference = 'Stop'
$taskVswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$env:PATH = (Split-Path -Parent $taskVswhere) + ';' + $env:PATH
$taskInstallation = & $taskVswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if ($LASTEXITCODE -ne 0 -or -not $taskInstallation) { throw 'No installed MSVC x64 toolchain' }
Import-Module (Join-Path $taskInstallation 'Common7/Tools/Microsoft.VisualStudio.DevShell.dll')
Enter-VsDevShell -VsInstallPath $taskInstallation -SkipAutomaticLocation -DevCmdArguments '-arch=x64 -host_arch=x64' | Out-Null
$taskLlvm = Join-Path $taskInstallation 'VC/Tools/Llvm/x64/bin'
$taskArgs = @('-B', 'tools/run_x86_upstream_comparison.py', '--fork-commit', $ForkCommit,
    '--upstream-commit', $UpstreamCommit, '--pairs', $Pairs, '--trials', $Trials,
    '--sample-ms', $SampleMs, '--cpu', $Cpu,
    '--clang', (Join-Path $taskLlvm 'clang++.exe'), '--objdump', (Join-Path $taskLlvm 'llvm-objdump.exe'))
if ($CandidatePatch) {
    $taskArgs[1] = 'tools/run_x86_candidate_comparison.py'
    $taskArgs += @('--candidate-patch', $CandidatePatch)
}
if ($Output) { $taskArgs += @('--output', $Output) }
if ($PortableOutput) { $taskArgs += @('--portable-output', $PortableOutput) }
& python @taskArgs
if ($LASTEXITCODE -ne 0) { throw "Comparison failed with exit $LASTEXITCODE; retain the new build directory" }
