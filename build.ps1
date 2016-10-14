$devenv = "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.exe"
$shadow = "D:\source\Shadow\Shadow Common Files\"
$config = "Debug"

&$devenv $shadow"Source\Mathematics\Mathematics.vcxproj" /Build $config
&$devenv $shadow"Source\Timer\Timer.vcxproj" /Build $config

&$devenv $shadow"Source\ObjectExtensions\ObjectExtensions.vcxproj" /Build $config | Out-Null

&$devenv $shadow"Source\Debugging\Debugging.vcxproj" /Build $config
&$devenv $shadow"Source\Imaging\Imaging.vcxproj" /Build $config
&$devenv $shadow"Source\Graphics\Graphics.vcxproj" /Build $config
&$devenv $shadow"Source\XML\XML.vcxproj" /Build $config
&$devenv $shadow"Source\DatafileIO\DatafileIO.vcxproj" /Build $config
&$devenv $shadow"Source\Scripting\Scripting.vcxproj" /Build $config
