#!/bin/sh

# Grab needed software
sudo apt-get install build-essential python-dev

# Build the bootloaders
cd source/linux
python Make.py
make
cd ../..

# Configuring PyInstaller setup
python Configure.py

# Create a spec file for the project
python Makespec.py --onefile --noconsole --icon=/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiagPy/QuickUSB.ico --name=QuickUsbDiagPy /Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiagPy/QuickUsbDiag.py

# Build the project
python Build.py QuickUsbDiagPy/QuickUsbDiagPy.spec
