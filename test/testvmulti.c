#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
#define INITGUID
#define INC_OLE2  // Ensure OLE2 definitions are included

#include <windows.h>
#include <oleauto.h>  // Required for VARIANT_BOOL
#include <objbase.h>  // Required for COM-related types

#include <stdio.h>
#include <stdlib.h>
#include "vmulticlient.h"

//
// Function prototypes
//

VOID
SendHidRequests(
    pvmulti_client vmulti,
    BYTE requestType
    );

//
// Implementation
//

void
Usage(void)
{
    printf("Usage: testvmulti </multitouch | /mouse | /digitizer | /joystick | /keyboard | /message>\n");
}

INT main(
    int argc,
    PCHAR argv[]
    )
{
    BYTE   reportId;
    pvmulti_client vmulti;

    UNREFERENCED_PARAMETER(argv);

    //
    // Parse command line
    //

    if (argc == 1)
    {
        Usage();
        return 1;
    }
    if (strcmp(argv[1], "/multitouch") == 0)
    {
        reportId = REPORTID_MTOUCH;
    }
    else if (strcmp(argv[1], "/mouse") == 0)
    {
        reportId = REPORTID_MOUSE;
    }
    else if (strcmp(argv[1], "/digitizer") == 0)
    {
        reportId = REPORTID_DIGI;
    }
    else if (strcmp(argv[1], "/joystick") == 0)
    {
        reportId = REPORTID_JOYSTICK;
    }
    else if (strcmp(argv[1], "/keyboard") == 0)
    {
        reportId = REPORTID_KEYBOARD;
    }
    else if (strcmp(argv[1], "/message") == 0)
    {
        reportId = REPORTID_MESSAGE;
    }
    else
    {
        Usage();
        return 1;
    }

    //
    // File device
    //

    vmulti = vmulti_alloc();

    if (vmulti == NULL)
    {
        return 2;
    }

    if (!vmulti_connect(vmulti))
    {
        vmulti_free(vmulti);
        return 3;
    }

    printf("...sending request(s) to our device\n");
    SendHidRequests(vmulti, reportId);

    vmulti_disconnect(vmulti);

    vmulti_free(vmulti);

    return 0;
}

VOID
SendHidRequests(
    pvmulti_client vmulti,
    BYTE requestType
    )
{
    switch (requestType)
    {
        case REPORTID_MTOUCH:
        {
            //
            // Send the multitouch reports
            //
            
            BYTE i;
            BYTE actualCount = 1; // set whatever number you want, lower than MULTI_MAX_COUNT
			BOOLEAN press_once = actualCount == 1;
			if (actualCount == 1) {
				actualCount = 2;
			}

            PTOUCH pTouch = (PTOUCH)malloc(actualCount * sizeof(TOUCH));

            printf("Sending multitouch report\n");
            Sleep(1000);

			while (1) {
				if (press_once) {
					pTouch[0].ContactID = 0;
					pTouch[0].Status = MULTI_CONFIDENCE_BIT | MULTI_IN_RANGE_BIT | MULTI_TIPSWITCH_BIT;
					pTouch[0].XValue = (0 + 1) * 1000;
					pTouch[0].YValue = (0 + 1) * 1500 + 5000;
					pTouch[0].Width = 10;
					pTouch[0].Height = 10;
					pTouch[1] = pTouch[0]; // Copy first touch data
					pTouch[1].ContactID = 1; // Ensure unique ID

					printf("x: %d, y: %d", pTouch[0].XValue, pTouch[0].YValue);

					if (!vmulti_update_multitouch(vmulti, pTouch, actualCount))
						printf("vmulti_update_multitouch TOUCH_DOWN FAILED\n");

					Sleep(500); // Wait for 0.5 seconds

					pTouch[0].Status = 0;   // Lift fingers (TOUCH UP)
					pTouch[1].Status = 0;

					if (!vmulti_update_multitouch(vmulti, pTouch, actualCount))
						printf("vmulti_update_multitouch TOUCH_UP FAILED\n");

					Sleep(500); // Wait for 0.5 seconds before repeating
				}
			}
            
            free(pTouch);
            
            break;
        }

        case REPORTID_MOUSE:
            //
            // Send the mouse report
            //
            printf("Sending mouse report\n");
			printf("Pressed 1000, 10000");
            vmulti_update_mouse(vmulti, 1, 1000, 10000, 0);
			Sleep(100);
			vmulti_update_mouse(vmulti, 0, 1000, 10000, 0);
			Sleep(50);
			vmulti_update_mouse(vmulti, 1, 1000, 10000, 0);
			Sleep(50);
			vmulti_update_mouse(vmulti, 0, 1000, 10000, 0);

            break;

        case REPORTID_DIGI:
            //
            // Send the digitizer reports
            //
            printf("Sending digitizer report\n");
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT, 1000, 10000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT, 1000, 12000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT, 1000, 14000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT | DIGI_TIPSWITCH_BIT, 1000, 16000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT | DIGI_TIPSWITCH_BIT, 1000, 18000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT | DIGI_TIPSWITCH_BIT, 1000, 20000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT | DIGI_TIPSWITCH_BIT, 2000, 20000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT | DIGI_TIPSWITCH_BIT, 3000, 20000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT, 3000, 20000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT, 3000, 15000);
            Sleep(100);
            vmulti_update_digi(vmulti, DIGI_IN_RANGE_BIT, 3000, 10000);
            vmulti_update_digi(vmulti, 0, 3000, 10000);
            break;

        case REPORTID_JOYSTICK:
        {
            //
            // Send the joystick report
            //
            USHORT buttons = 0;
            BYTE hat = 0, x = 0, y = 128, rx = 128, ry = 64, throttle = 0;
            printf("Sending joystick report\n");
            while (1)
            {
                vmulti_update_joystick(vmulti, buttons, hat, x, y, rx, ry, throttle);
                buttons = rand() | ((rand()&1) << 15); //rand() | rand() << 15 | rand() % 4 << 30;
                hat++;
                x++;
                y++;
                rx++;
                ry--;
                throttle++;
                Sleep(10);
            }
            break;
        }
        
        case REPORTID_KEYBOARD:
        {
            //
            // Send the keyboard report
            //

            // See http://www.usb.org/developers/devclass_docs/Hut1_11.pdf
            // for a list of key codes            
                        
            BYTE shiftKeys = KBD_LGUI_BIT;
            BYTE keyCodes[KBD_KEY_CODES] = {0, 0, 0, 0, 0, 0};
            printf("Sending keyboard report\n");

            // Windows key
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);
            shiftKeys = 0;
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);
            Sleep(100);
            
            // 'Hello'
            shiftKeys = KBD_LSHIFT_BIT;
            keyCodes[0] = 0x0b; // 'h'
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);
            shiftKeys = 0;
            keyCodes[0] = 0x08; // 'e'
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);
            keyCodes[0] = 0x0f; // 'l'
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);
            keyCodes[0] = 0x0;
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);
            keyCodes[0] = 0x0f; // 'l'
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);
            keyCodes[0] = 0x12; // 'o'
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);
            keyCodes[0] = 0x0;
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);
            
            // Toggle caps lock
            keyCodes[0] = 0x39; // caps lock
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);
            keyCodes[0] = 0x0;
            vmulti_update_keyboard(vmulti, shiftKeys, keyCodes);

            break;
        }

        case REPORTID_MESSAGE:
        {
            VMultiMessageReport report;

            printf("Writing vendor message report\n");

            memcpy(report.Message, "Hello VMulti\x00", 13);

            if (vmulti_write_message(vmulti, &report))
            {
                memset(&report, 0, sizeof(report));
                printf("Reading vendor message report\n");
                if (vmulti_read_message(vmulti, &report))
                {
                    printf("Success!\n    ");
                    printf(report.Message);
                }
            }

            break;
        }
    }
}


