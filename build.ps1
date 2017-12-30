function Get-ScriptDirectory
{
    $Invocation = (Get-Variable MyInvocation -Scope 1).Value
    Split-Path $Invocation.MyCommand.Path
}

function Get-VisualStudioPath
{
    $result = "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe"

    if (!(Test-Path $result))
    {
        $result = "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\Common7\IDE\devenv.exe"
    }

    return $result;
}

function Parse-Log($path)
{
    Get-Content $path | Select-String -Pattern "==========" | Out-Host
}

function Cleanup
{
    "Recreating log dir: " + $shadow + "\Log"
    New-Item -ItemType Directory -Force -Path $shadow"\Log" | Out-File null
    "Deleting old logs`n"
    Remove-Item $shadow"\Log\*"
}

function Build-Solution($name)
{
    $logFile = $shadow + "\Log\" + $name + ".txt";
    $solutionFile = $shadow + "\Source\" + $name + "\" + $name + ".vcxproj";

    "Building " + $name;

    &$devenv $solutionFile /Build $config /out $logFile | Out-File null

    Get-Content $logFile | Select-String -Pattern "==========" | Out-Host
}

$devenv = Get-VisualStudioPath;
$shadow = Get-ScriptDirectory;
$config = "Debug"

Cleanup

Build-Solution("Mathematics")
Build-Solution("Timer")
Build-Solution("ObjectExtensions")
Build-Solution("Debugging")
Build-Solution("Imaging")
Build-Solution("Graphics")
Build-Solution("XML")
Build-Solution("DatafileIO")
Build-Solution("Scripting")