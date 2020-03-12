rem Configure PyInstalller setup
python Configure.py

rem Create a spec file for the project
python Makespec.py --onefile --noconsole --icon=C:\Projects\Bitwise\QuickUSB\Library\Software\QuickUsbDiagPy\QuickUSB.ico --name=QuickUsbDiagPy C:\Projects\Bitwise\QuickUSB\Library\Software\QuickUsbDiagPy\QuickUsbDiag.py

rem Build the project
python Build.py QuickUsbDiagPy\QuickUsbDiagPy.spec
