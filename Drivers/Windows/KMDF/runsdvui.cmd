cd /d "C:\Projects\Bitwise\QuickUSB\Library\Driver\Windows\KMDF" &msbuild "QuickUsbDriverKmdf.vcxproj" /t:sdvViewer /p:configuration="Debug" /p:platform=Win32
exit %errorlevel% 