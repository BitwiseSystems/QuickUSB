#include <stdio.h>
#include <QuickUsb.h>

int main ( int argc, char **argv ) {
  char devices[512]; /* Grr */
  HANDLE hDevice = NULL;
  char *devicename = NULL;
  int setting;
  unsigned short setting_value;
  unsigned char buffer[64];
  unsigned short bufferlen;

  /* Check invoked correctly */
  if ( argc != 2 ) {
    printf ( "Syntax: %s devicename\n", argv[0] );
    exit ( 1 );
  }
  devicename = argv[1];

  /* We have to call QuickUsbFindModules() first, otherwise it refuses
   * to open the device.  We can discard the result.  Don't ask.
   */
  QuickUsbFindModules ( devices, sizeof(devices) );

  /* Open device */
  if ( ! QuickUsbOpen ( &hDevice, devicename ) ) {
    printf ( "Could not open QuickUSB device %s\n", devicename );
    printf ( "foo %x\n", hDevice );
    exit ( 1 );
  }

  /* Read settings */
  for ( setting = 1; setting <= 4 ; setting++ ) {
    if ( ! QuickUsbReadSetting ( hDevice, setting, &setting_value ) ) {
      printf ( "Could not read setting %d\n", setting );
    } else {
      printf ( "Setting %d is set to 0x%04x\n", setting, setting_value );
    }
  }

  /* Set port direction to half input, half output */
  if ( ! QuickUsbWritePortDir ( hDevice, 0, 0x0f ) ) {
    printf ( "Could not set port A direction\n" );
    exit ( 1 );
  }

  /* Enter read-write cycle */
  buffer[0] = 0xff;
  while ( buffer[0] ) {
    sleep ( 1 );
    bufferlen = 1;
    if ( ! QuickUsbReadPort ( hDevice, 0, buffer, &bufferlen ) ) {
      printf ( "Could not read from port\n" );
      exit ( 1 );
    }
    printf ( "Value read: 0x%02x\n", buffer[0] );
    bufferlen = 1;
    buffer[0] = buffer[0] >> 4;
    if ( ! QuickUsbWritePort ( hDevice, 0, buffer, bufferlen ) ) {
      printf ( "Could not write to port\n" );
      exit ( 1 );
    }
  }

  /* Close device */
  if ( ! QuickUsbClose ( hDevice ) ) {
    printf ( "Could not open QuickUSB device %s\n", devicename );
    exit ( 1 );
  }
  
  return ( 0 );
}
