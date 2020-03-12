# -*- mode: python -*-
a = Analysis([os.path.join(HOMEPATH,'support/_mountzlib.py'), os.path.join(HOMEPATH,'support/useUnicode.py'), '/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiagPy/QuickUsbDiag.py'],
             pathex=['/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiagPy/PyInstaller'])
pyz = PYZ(a.pure)
exe = EXE( pyz,
          a.scripts,
          a.binaries,
          a.zipfiles,
          a.datas,
          name=os.path.join('dist', 'QuickUsbDiagPy'),
          debug=False,
          strip=False,
          upx=True,
          console=1 , icon='/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiagPy/QuickUSB.ico')
