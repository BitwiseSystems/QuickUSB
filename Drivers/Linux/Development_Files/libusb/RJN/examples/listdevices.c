#include <stdio.h>
#include <QuickUsb.h>

int main ( void ) {
  char devices[512];
  char *device;

  // Retrieve list of devices
  if ( ! QuickUsbFindModules(devices,sizeof(devices)) ) {
    printf ( "QuickUsbFindModules failed\n" );
    exit ( 1 );
  }
  if ( devices[0] == '\0' ) {
    printf ( "No QuickUSB devices found\n" );
    exit ( 1 );
  }

  // Print out list of devices
  printf ( "Found QuickUSB devices:\n" );
  device = devices;
  while ( *device != '\0' ) {
    printf ( "  %s\n", device );
    device += strlen(device) + 1;
  }

  return 0;
}
