from distutils.core import setup
import py2exe

# Remain backwards compatible with Python 2.2.3 and earlier
from sys import version
if version < '2.2.3':
    from distutils.dist import DistributionMetadata
    DistributionMetadata.classifiers = None
    DistributionMetadata.download_url = None


# Package Setup
setup(
    # Package Metadata
    name='QuickUSB Diagnostics (Python)',
    version='2.15.1',
    author='Bitwise Systems',
    author_email='support@quickusb.com',
#    maintainer='',
#    maintainer_email='',
    url='http://www.quickusb.com',
    description='QuickUSB Diagnostics Application',
#    long_description='',
#    download_url='',
#    classifiers='',
#    platforms='',
#    license='',

    # Package Files
    #py_modules=['QuickUsb'],
#    ext_modules=[Extension('foo', ['foo.c'])],
    options = {
        "py2exe": {
            "dll_excludes": ["MSVCP90.dll"], # This is still required on the user's machine to execute app
            "compressed": 1,
            "optimize": 2,
            #"ascii": 1,
            "bundle_files": 1
            }},
    zipfile = None,

    # py2exe targets to build
    windows = [
        {
            "script":"QuickUsbDiag.py",
            "icon_resources": [(1, "QuickUSB.ico")],
        }
    ],
)
