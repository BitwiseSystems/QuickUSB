from bundlebuilder import buildapp

buildapp(
	name='QuickUsbDiagPy.app',
	mainprogram='QuickUsbDiag.py',
	#argv_emulation=1,
	iconfile='QuickUSB.ico',
	standalone=1,
	includeModules=[],
	includePackages=[],
	libs=['/usr/lib/libwx_macud-2.8.dylib'],
)
