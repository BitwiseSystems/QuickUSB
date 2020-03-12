echo off

echo Copying QuickUSB DLLs
copy C:\Projects\Bitwise\QuickUSB\Library\Software\QuickUSB\x64\Release\Windows\QuickUSB.dll Driver-Signing-Sandbox\amd64\QuickUSBamd64.dll
copy C:\Projects\Bitwise\QuickUSB\Library\Software\QuickUSB\x64\Release_Util\Windows\QuickUsbUtil.dll Driver-Signing-Sandbox\amd64\QuickUsbUtilamd64.dll

copy C:\Projects\Bitwise\QuickUSB\Library\Software\QuickUSB\x86\Release\Windows\QuickUSB.dll Driver-Signing-Sandbox\i386\QuickUSB.dll
copy C:\Projects\Bitwise\QuickUSB\Library\Software\QuickUSB\x86\Release_Util\Windows\QuickUsbUtil.dll Driver-Signing-Sandbox\i386\QuickUsbUtil.dll

echo Copying QuickUSB Driver Files
copy objfre_win7_amd64\amd64\QuickUSB.sys Driver-Signing-Sandbox\amd64\QuickUSBamd64.sys
copy objfre_win7_x86\i386\QuickUSB.sys Driver-Signing-Sandbox\i386\QuickUSB.sys

echo Signing Driver Package Files
cd Driver-Signing-Sandbox\
call gencat.cmd
call sign.cmd
cd ..

echo Creating Driver Package Directory Structure
md QuickUSB-Driver-v3.0.0
md QuickUSB-Driver-v3.0.0\amd64
md QuickUSB-Driver-v3.0.0\i386

echo Copying Files to Driver Package
copy Driver-Signing-Sandbox\amd64\QuickUSBamd64.dll QuickUSB-Driver-v3.0.0\amd64
copy Driver-Signing-Sandbox\amd64\QuickUsbUtilamd64.dll QuickUSB-Driver-v3.0.0\amd64

copy Driver-Signing-Sandbox\i386\QuickUSB.dll QuickUSB-Driver-v3.0.0\i386
copy Driver-Signing-Sandbox\i386\QuickUsbUtil.dll QuickUSB-Driver-v3.0.0\i386

copy Driver-Signing-Sandbox\amd64\QuickUSBamd64.sys QuickUSB-Driver-v3.0.0\amd64
copy Driver-Signing-Sandbox\amd64\vcredist_x64.exe QuickUSB-Driver-v3.0.0\amd64
copy Driver-Signing-Sandbox\amd64\WdfCoInstaller01009.dll QuickUSB-Driver-v3.0.0\amd64

copy Driver-Signing-Sandbox\i386\QuickUSB.sys QuickUSB-Driver-v3.0.0\i386
copy Driver-Signing-Sandbox\i386\vcredist_x86.exe QuickUSB-Driver-v3.0.0\i386
copy Driver-Signing-Sandbox\i386\WdfCoInstaller01009.dll QuickUSB-Driver-v3.0.0\i386

copy Driver-Signing-Sandbox\DPInst.xml QuickUSB-Driver-v3.0.0\
copy Driver-Signing-Sandbox\DPInstx64.exe QuickUSB-Driver-v3.0.0\
copy Driver-Signing-Sandbox\DPInstx86.exe QuickUSB-Driver-v3.0.0\
copy "Driver-Signing-Sandbox\QuickUSB SLA.txt" QuickUSB-Driver-v3.0.0\
copy Driver-Signing-Sandbox\QuickUSB.cat QuickUSB-Driver-v3.0.0\
copy Driver-Signing-Sandbox\QuickUSB.ico QuickUSB-Driver-v3.0.0\
copy Driver-Signing-Sandbox\QuickUSB.inf QuickUSB-Driver-v3.0.0\
copy Driver-Signing-Sandbox\Setup.exe QuickUSB-Driver-v3.0.0\

echo Done
