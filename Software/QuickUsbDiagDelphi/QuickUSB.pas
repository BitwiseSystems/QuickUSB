unit QuickUSB;
{-----------------------------------------------------------------------------
  Title       : QuickUSB.pas
  Description : Delphi Unit for the QuickUSB Library DLL
  Author      : bhenry
  
  $History:$
------------------------------------------------------------------------------}
interface

uses Windows, Classes, SysUtils;

function FindModules: TStringList;

const
   { QuickUSB String Descriptor Indices }
   QUICKUSB_MAKE = $01;
   QUICKUSB_MODEL = $02;
   QUICKUSB_SERIAL = $03;

   { QuickUSB settings (wIndex values for QUICKUSB_SETTING }
   SETTING_EP26CONFIG = $0;        { MSB=EP2CFG, LSB=EP6CFG }
   SETTING_WORDWIDE = $1;          { *wbuf = 1 is 16 bit transfers, 0 is 8 bits  }
   SETTING_DATAADDRESS = $2;       { *wbuf = Data transfer address.  }
                                   { If the high bit is set, don't increment addresses  }
   SETTING_FIFOCONFIG = $3;        { *wbuf: 0xFB=Slave, 0xFA=Master mode  }
   SETTING_FPGATYPE = $4;          { FPGA configuration type  }
   SETTING_CPUCONFIG = $5;         { Sets/Reads CPUCS  }
   SETTING_SPICONFIG = $6;         { MSB=PORTECFG, LSB=SpiConfig }
   SETTING_SLAVEFIFOFLAGS = $7;    { Read Only }
   SETTING_I2CTL = $8;             { MSB=TBD, LSB=I2CTL }
   SETTING_PORTA = $9;             { MSB=OEA, LSB=IOA }
   SETTING_PORTB = $0A;            { MSB=OEB, LSB=IOB }
   SETTING_PORTC = $0B;            { MSB=OEC, LSB=IOC }
   SETTING_PORTD = $0C;            { MSB=OED, LSB=IOD }
   SETTING_PORTE = $0D;            { MSB=OEE, LSB=IOE }
   SETTING_PORTACCFG = $0E;        { MSB=PORTACFG, LSB=PORTCCFG }
   SETTING_PINFLAGS = $0F;         { MSB=PINFLAGSAB, LSB=PINFLAGSCD }
   SETTING_VERSIONSPEED = $11;     { Read Only - MSB=REVID, LSB=High Speed mode }

   { FIFO config settings }
   FIFOCONFIG_GPIF = $FA;
   FIFOCONFIG_SLAVE = $03;

   { FPGA type settings }
   FPGATYPE_ALTERAPS = $0;         { Altera or Xilinx passive serial }
   FPGATYPE_XILINXSS = $1;         { Xilinx slave serial }

   { QuickUSB Error Codes }
   { There is no last error code to report }
   QUICKUSB_ERROR_NO_ERROR = $0;
   { A memory allocation function failed while executing a function }
   QUICKUSB_ERROR_OUT_OF_MEMORY = $1;
   { The module was located, but was not able to be opened successfully }
   QUICKUSB_ERROR_CANNOT_OPEN_MODULE = $2;
   { The module could not be located }
   QUICKUSB_ERROR_CANNOT_FIND_DEVICE = $3;
   { Unable to obtain necessary information from device }
   QUICKUSB_ERROR_IOCTL_FAILED = $4;
   { Attempting to read or write data of length 0 }
   QUICKUSB_ERROR_INVALID_PARAMETER = $5;
   { Timeout occured while trying to read or write data }
   QUICKUSB_ERROR_TIMEOUT = $6;
   { The function you are trying to execute is not supported by this driver version }
   QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED = $7;

{ External DLL Function Declarations }
function QuickUsbGetLastError(error: PLongWord): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbFindModules(namelist: PChar; length: Longint): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbOpen(hDevice: PHandle; deviceName: PChar): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbClose(hDevice: THandle): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbGetStringDescriptor(hDevice: THandle; index: Byte; buffer: PChar; length: Word): Integer; stdcall; external 'QuickUsb.dll';

function QuickUsbReadSetting(hDevice: THandle; address: Word; value: PWord): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbWriteSetting(hDevice: THandle; address: Word; value: Word): Integer; stdcall; external 'QuickUsb.dll';

function QuickUsbStartFpgaConfiguration(hDevice: THandle): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbWriteFpgaData(hDevice: THandle; fpgaData: Pointer; length: LongWord): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbIsFpgaConfigured(hDevice: THandle; isConfigured: PWord): Integer; stdcall; external 'QuickUsb.dll';

function QuickUsbReadCommand(hDevice: THandle; address: Word; data: Pointer; length: PWord): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbWriteCommand(hDevice: THandle; address: Word; data: Pointer; length: Word): Integer; stdcall; external 'QuickUsb.dll';

function QuickUsbReadData(hDevice: THandle; data: Pointer; length: PLongWord): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbWriteData(hDevice: THandle; data: Pointer; length: LongWord): Integer; stdcall; external 'QuickUsb.dll';

function QuickUsbReadPortDir(hDevice: THandle; address: Word; data: PByte): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbWritePortDir(hDevice: THandle; address: Word; data: Byte): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbReadPort(hDevice: THandle; address: Word; data: Pointer; length: PWord): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbWritePort(hDevice: THandle; address: Word; data: Pointer; length: Word): Integer; stdcall; external 'QuickUsb.dll';

function QuickUsbSetRS232BaudRate(hDevice: THandle; baudrate: LongWord): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbGetNumRS232(hDevice: THandle; portNum: Byte; length: PWord): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbFlushRS232(hDevice: THandle; portNum: Byte): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbReadRS232(hDevice: THandle; portNum: Byte; data: Pointer; length: PWord): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbWriteRS232(hDevice: THandle; portNum: Byte; data: Pointer; length: Word): Integer; stdcall; external 'QuickUsb.dll';

function QuickUsbReadSpi(hDevice: THandle; portNum: Byte; data: Pointer; length: PWord): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbWriteSpi(hDevice: THandle; portNum: Byte; data: Pointer; length: Word): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbWriteReadSpi(hDevice: THandle; portNum: Byte; data: Pointer; length: Word): Integer; stdcall; external 'QuickUsb.dll';

function QuickUsbReadI2C(hDevice: THandle; address: Word; data: Pointer; length: PWord): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbWriteI2C(hDevice: THandle; address: Word; data: Pointer; length: Word): Integer; stdcall; external 'QuickUsb.dll';

function QuickUsbGetDriverVersion(MajorDriverVersion: PWord; MinorDriverVersion: PWord; BuildDriverVersion: PWord): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbGetDllVersion(MajorDllVersion: PWord; MinorDllVersion: PWord; BuildDllVersion: PWord): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbGetFirmwareVersion(hDevice: THandle; MajorFirmwareVersion: PWord; MinorFirmwareVersion: PWord; BuildFirmwareVersion: PWord): Integer; stdcall; external 'QuickUsb.dll';

function QuickUsbSetTimeout(hDevice: THandle; timeOut: Longint): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbWriteDataAsync(hDevice: THandle; data: PChar; length: LongWord; transaction: PChar): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbReadDataAsync(hDevice: THandle; data: PChar; length: PLongWord; transaction: PChar): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbAsyncWait(hDevice: THandle; bytecount: LongWord; transaction: Char; immediate: Char): Integer; stdcall; external 'QuickUsb.dll';

function QuickUsbReadDefault(hDevice: THandle; address: Word; value: PWord): Integer; stdcall; external 'QuickUsb.dll';
function QuickUsbWriteDefault(hDevice: THandle; address: Word; value: Word): Integer; stdcall; external 'QuickUsb.dll';

implementation


function FindModules: TStringList;
{-----------------------------------------------------------------------------
 Purpose : Find all the QuickUSB modules in a system
 Returns : A TStringList containging the module names.
 Notes   : Very handy for poplating a listbox or combobox.
------------------------------------------------------------------------------}
var list: TStringList;
var ptr: PChar;
var name, namelist: AnsiString;
begin
   { Create the string list }
   list := TStringList.Create;

   { Make space for the name list }
   SetLength(namelist, 256);

   { Call the Find Modules routine }
   QuickUsbFindModules(PChar(namelist), Length(namelist));

   { Scan the result and make a list of strings from it }
   ptr := PChar(namelist);
   repeat
      { Assign the ANSI string to a string}
      name := ptr;

      { If its not empty, add it to the list }
      if (name <> '') then begin
         list.Append(name);
      end;

      { Look for the next string in the list }
      ptr := ptr + length(name) + 1;
   until (name = '');

   { Return the list }
   FindModules := list;
end;

end.
