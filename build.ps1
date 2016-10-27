# Get the ID and security principal of the current user account
$myWindowsID=[System.Security.Principal.WindowsIdentity]::GetCurrent()
$myWindowsPrincipal=new-object System.Security.Principal.WindowsPrincipal($myWindowsID)
 
# Get the security principal for the Administrator role
$adminRole=[System.Security.Principal.WindowsBuiltInRole]::Administrator

# Check to see if we are currently running "as Administrator"
if ($myWindowsPrincipal.IsInRole($adminRole))
{
   # We are running "as Administrator" - so change the title and background color to indicate this
   $Host.UI.RawUI.WindowTitle = $myInvocation.MyCommand.Definition + "(Elevated)"
   $Host.UI.RawUI.BackgroundColor = "DarkBlue"
   clear-host
}
else
{
   # We are not running "as Administrator" - so relaunch as administrator
   # Create a new process object that starts PowerShell
   $newProcess = new-object System.Diagnostics.ProcessStartInfo "PowerShell";

   # Specify the current script path and name as a parameter
   $newProcess.Arguments = $myInvocation.MyCommand.Definition;

   # Indicate that the process should be elevated
   $newProcess.Verb = "runas";

   # Start the new process
   [System.Diagnostics.Process]::Start($newProcess);

   # Exit from the current, unelevated, process
   exit
} 

#----------------------------------------------------------------------

function Get-ScriptDirectory
{
  $Invocation = (Get-Variable MyInvocation -Scope 1).Value
  Split-Path $Invocation.MyCommand.Path
}

$devenv = "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.exe"
$shadow = Get-ScriptDirectory;
$config = "Debug"

"Recreating log dir: " + $shadow + "\Log"
New-Item -ItemType Directory -Force -Path $shadow"\Log" | Out-File null
"Deleting old logs`n"
Remove-Item $shadow"\Log\*"

"Building Mathematics"
&$devenv $shadow"\Source\Mathematics\Mathematics.vcxproj" /Build $config /out $shadow"\Log\Mathematics.txt" | Out-File null
Get-Content $shadow"\Log\Mathematics.txt" | Out-Host

"Building Timer"
&$devenv $shadow"\Source\Timer\Timer.vcxproj" /Build $config /out $shadow"\Log\Timer.txt" | Out-File null
Get-Content $shadow"\Log\Timer.txt" | Out-Host

"Building ObjectExtensions"
&$devenv $shadow"\Source\ObjectExtensions\ObjectExtensions.vcxproj" /Build $config /out $shadow"\Log\ObjectExtensions.txt" | Out-File null
Get-Content $shadow"\Log\ObjectExtensions.txt" | Out-Host

"Building Debugging"
&$devenv $shadow"\Source\Debugging\Debugging.vcxproj" /Build $config /out $shadow"\Log\Debugging.txt" | Out-File null
Get-Content $shadow"\Log\Debugging.txt" | Out-Host

"Building Imaging"
&$devenv $shadow"\Source\Imaging\Imaging.vcxproj" /Build $config /out $shadow"\Log\Imaging.txt" | Out-File null
Get-Content $shadow"\Log\Imaging.txt" | Out-Host

"Building Graphics"
&$devenv $shadow"\Source\Graphics\Graphics.vcxproj" /Build $config /out $shadow"\Log\Graphics.txt" | Out-File null
Get-Content $shadow"\Log\Graphics.txt" | Out-Host

"Building XML"
&$devenv $shadow"\Source\XML\XML.vcxproj" /Build $config /out $shadow"\Log\XML.txt" | Out-File null
Get-Content $shadow"\Log\XML.txt" | Out-Host

"Building DatafileIO"
&$devenv $shadow"\Source\DatafileIO\DatafileIO.vcxproj" /Build $config /out $shadow"\Log\DatafileIO.txt" | Out-File null
Get-Content $shadow"\Log\DatafileIO.txt" | Out-Host

"Building Scripting"
&$devenv $shadow"\Source\Scripting\Scripting.vcxproj" /Build $config /out $shadow"\Log\Scripting.txt" | Out-File null
Get-Content $shadow"\Log\Scripting.txt" | Out-Host
