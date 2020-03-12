% QuickUSB SPI Test

% Scan the system and find out how many modules are connected
modules = QuickUSB.FindModules;

% Create a QuickUSB object and call it qusb
qusb = QuickUSB(modules{1,1});

% Create a data array
bytes = [32,3];

% Write the data array to SPI port 0 (nSS0 = PE6)
qusb.WriteSpiBytes(0, bytes);

% Read two bytes from SPI port 0 and display them
result = qusb.ReadSpiBytes(0, 2)


