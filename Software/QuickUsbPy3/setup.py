from distutils.core import setup

# Remain backwards compatible with Python 2.2.3 and earlier
from sys import version
if version < '2.2.3':
    from distutils.dist import DistributionMetadata
    DistributionMetadata.classifiers = None
    DistributionMetadata.download_url = None

# Package Setup
setup(
    # Package Metadata
    name='QuickUsb',
    version='2.15.1',
    author='Bitwise Systems',
    author_email='support@quickusb.com',
#    maintainer='',
#    maintainer_email='',
    url='http://www.quickusb.com',
    description='QuickUSB Python Interface Distribution Utility',
#    long_description='',
#    download_url='',
#    classifiers='',
#    platforms='',
#    license='',

    # Package Files
    py_modules=['QuickUsb'],
#    ext_modules=[Extension('foo', ['foo.c'])],
)
