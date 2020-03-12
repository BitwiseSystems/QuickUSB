#include <windows.h>

#include <iostream>
using namespace std;

#include <QuickUSB.h>

void ProgramCB(QHANDLE hDevice, QWORD percentComplete, PQVOID tag)
{
    switch ((int)tag) {
    case 0:
        cout << "Verifying firmware: " << percentComplete << "% complete\r";
        break;
    case 1:
        cout << "Reading firmware:   " << percentComplete << "% complete\r";
        break;
    case 2:
        cout << "Writing firmware:   " << percentComplete << "% complete\r";
        break;
    default:
        cout << "Error!\n";
        break;
    }
}

// Main Program
int main(int argc, char* argv[]) {
	QCHAR nameList[1024], *deviceName;
	
	QHANDLE hDevice;
	QRESULT ok;
	QULONG lastError;
    QCHAR str[64];

    int k, cmdCount = 0;

    // Count command-line arguments
    /*
    while (argv[++cmdCount] != 0)
        ;

    for (k=1; k<cmdCount; ++k) {
        if (argv[k][0] != '-') {
            return 1;
        }

        switch (argv[k][1]) {
        case 'd':
            ok = QuickUsbFindModules(nameList, 1024);
            if (!ok) {
                return 1;
            }
            deviceName = nameList;
            cout << " Name     | Serial | Firmware Model                                  " << endl;
            cout << "---------------------------------------------------------------------" << endl;
            if (*deviceName == 0) {
                cout << "No modules found." << endl;
            } else {
                do {
                    cout << ' ';
                    cout.width(9);
                    cout << left << deviceName;
                    cout << '|';

                    ok = QuickUsbOpen(&hDevice, deviceName);
		            if (!ok) {
			            return 1;
		            }

                    ok = QuickUsbGetStringDescriptor(hDevice, QUICKUSB_SERIAL, str, 64);
                    if (!ok) {
                        return 1;
                    }

                    cout << ' ';
                    cout.width(7);
                    cout << left << str;
                    cout << '|';

                    ok = QuickUsbGetStringDescriptor(hDevice, QUICKUSB_MODEL, str, 64);
                    if (!ok) {
                        return 1;
                    }

                    cout << ' ';
                    cout << str << endl;

                    ok = QuickUsbClose(hDevice);
		            if (!ok) {
			            return 1;
		            }

                    while (*deviceName != 0)
                        ++deviceName;
                } while (*(++deviceName) != 0);
            }

            break;
        case 'p':
            break;
        default:
            return 1;
            break;
        }
    }*/

	ok = QuickUsbFindModules(nameList, 1024);
	ok = QuickUsbGetLastError(&lastError);
	if (ok && nameList != 0) {
		ok = QuickUsbOpen(&hDevice, nameList);
		if (!ok) {
			ok = QuickUsbGetLastError(&lastError);
			cout << "Error: " << lastError << endl;
			return 1;
		}
/*
        ok = QuickUsbVerifyFirmware(hDevice, 
            "C:\\Program Files (x86)\\Bitwise Systems\\QuickUsb\\QuickUsbProgrammer\\Simple\\quickusb-simple v2.14.0.qusb", 
            &ProgramCB, (PQVOID)0);
        cout << endl;
		if (!ok) {
			ok = QuickUsbGetLastError(&lastError);
            cout << "\tFAIL (" << lastError << ")" << endl;
        } else {
            cout << "\tOK" << endl;
        }

        ok = QuickUsbWriteFirmware(hDevice, 
            "C:\\Program Files (x86)\\Bitwise Systems\\QuickUsb\\QuickUsbProgrammer\\Simple\\quickusb-simple v2.14.0.qusb", 
            0, &ProgramCB, (PQVOID)2);
        cout << endl;
		if (!ok) {
			ok = QuickUsbGetLastError(&lastError);
            cout << "\tFAIL (" << lastError << ")" << endl;
        } else {
            cout << "\tOK" << endl;
        }

        ok = QuickUsbVerifyFirmware(hDevice, 
            "C:\\Program Files (x86)\\Bitwise Systems\\QuickUsb\\QuickUsbProgrammer\\Simple\\quickusb-simple v2.14.0.qusb", 
            &ProgramCB, (PQVOID)0);
        cout << endl;
		if (!ok) {
			ok = QuickUsbGetLastError(&lastError);
            cout << "\tFAIL (" << lastError << ")" << endl;
        } else {
            cout << "\tOK" << endl;
        }
*/
        ok = QuickUsbReadFirmware(hDevice, "C:\\Users\\Jason Wolfe\\Desktop\\firmware.qusb", &ProgramCB, (PQVOID)1);
        cout << endl;
		if (!ok) {
			ok = QuickUsbGetLastError(&lastError);
            cout << "\tFAIL (" << lastError << ")" << endl;
        } else {
            cout << "\tOK" << endl;
        }
/*
        ok = QuickUsbWriteFirmware(hDevice, 
            "C:\\Users\\Jason Wolfe\\Desktop\\firmware.qusb", 
            0, &ProgramCB, (PQVOID)2);
        cout << endl;
		if (!ok) {
			ok = QuickUsbGetLastError(&lastError);
            cout << "\tFAIL (" << lastError << ")" << endl;
        } else {
            cout << "\tOK" << endl;
        }

        ok = QuickUsbVerifyFirmware(hDevice, 
            "C:\\Users\\Jason Wolfe\\Desktop\\firmware.qusb", 
            &ProgramCB, (PQVOID)0);
        cout << endl;
		if (!ok) {
			ok = QuickUsbGetLastError(&lastError);
            cout << "\tFAIL (" << lastError << ")" << endl;
        } else {
            cout << "\tOK" << endl;
        }

        ok = QuickUsbVerifyFirmware(hDevice, 
            "C:\\Program Files (x86)\\Bitwise Systems\\QuickUsb\\QuickUsbProgrammer\\Simple\\quickusb-simple v2.14.0.qusb", 
            &ProgramCB, (PQVOID)0);
        cout << endl;
		if (!ok) {
			ok = QuickUsbGetLastError(&lastError);
            cout << "\tFAIL (" << lastError << ")" << endl;
        } else {
            cout << "\tOK" << endl;
        }*/

		ok = QuickUsbClose(hDevice);
		if (!ok) {
			ok = QuickUsbGetLastError(&lastError);
			cout << "Error: " << lastError << endl;
			return 1;
		}
    }

    return 0;
}
