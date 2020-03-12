# -*- mode: python -*-
a = Analysis([os.path.join(HOMEPATH,'support\\_mountzlib.py'), os.path.join(HOMEPATH,'support\\useUnicode.py'), 'QuickUsbDiag.py'],
             pathex=['C:\\Projects\\Bitwise\\QuickUSB\\Library\\Software\\QuickUsbDiagPy\\pyinstaller-1.5.1'])
pyz = PYZ(a.pure)
exe = EXE( pyz,
          a.scripts,
          a.binaries,
          a.zipfiles,
          a.datas,
          name=os.path.join('dist', 'QuickUsbDiagPy.exe'),
          debug=False,
          strip=False,
          upx=True,
          console=False , icon='QuickUSB.ico')
app = BUNDLE(exe,
             name=os.path.join('dist', 'QuickUsbDiagPy.exe.app'))
