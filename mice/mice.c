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


__declspec(dllexport) pvmulti_client
CreateVmultiClient();

__declspec(dllexport) pvmulti_client
VmultiAlloc();

__declspec(dllexport) VOID
VmultiConnect(
	pvmulti_client vmulti
);

__declspec(dllexport) VOID
VmultiDisconnect(
	pvmulti_client vmulti
);

__declspec(dllexport) VOID
VmultiFree(
	pvmulti_client vmulti
);

//
// Send multitouch reports to simulate a click on the position.
// Will disable the system cursor for a short amount of period.
// Will click the position twice (the minimal amount of clicks
// for the action to be registered as multitouch).
//

__declspec(dllexport) VOID
Click(
	pvmulti_client vmulti,
	USHORT normX,
	USHORT normY,
	USHORT width,
	USHORT height,
	INT pressDuration
);

INT main(
    int argc,
    PCHAR argv[]
    )
{
	/*
    pvmulti_client vmulti;

	UNREFERENCED_PARAMETER(argv);

    //
    // File device
    //

	vmulti = VmultiAlloc();

    if (vmulti == NULL)
    {
        return 2;
    }

    if (!vmulti_connect(vmulti))
    {
        vmulti_free(vmulti);
        return 3;
    }

	Click(vmulti, 300, 450, 20, 20, 500);
	Sleep(500);
	Click(vmulti, 300, 450, 20, 20, 500);
	Sleep(500);
	Click(vmulti, 300, 450, 20, 20, 500);
	Sleep(500);
	Click(vmulti, 300, 450, 20, 20, 500);
	Sleep(500);
	Click(vmulti, 300, 450, 20, 20, 500);
	Sleep(500);
	Click(vmulti, 300, 450, 20, 20, 500);

    VmultiDisconnect(vmulti);

    VmultiFree(vmulti);
	*/
    return 0;
}

__declspec(dllexport) pvmulti_client
VmultiAlloc() {
	pvmulti_client vmulti;
	vmulti = vmulti_alloc();
	return vmulti;
}

__declspec(dllexport) VOID
VmultiConnect(vmulti) {
	return vmulti_connect(vmulti);
}

__declspec(dllexport) VOID
VmultiDisconnect(vmulti) {
	vmulti_disconnect(vmulti);
}

__declspec(dllexport) VOID
VmultiFree(vmulti) {
	vmulti_free(vmulti);
}

__declspec(dllexport) pvmulti_client
CreateVmultiClient() {
	pvmulti_client vmulti;
	vmulti = vmulti_alloc();

	if (vmulti == NULL)
	{
		return NULL;
	}

	if (!vmulti_connect(vmulti))
	{
		vmulti_free(vmulti);
		return NULL;
	}
	return vmulti;
}

__declspec(dllexport) VOID
Click(
	pvmulti_client vmulti,
	USHORT normX,
	USHORT normY,
	USHORT width,
	USHORT height,
	INT pressDuration
) {
	PTOUCH pTouch = (PTOUCH)malloc(2 * sizeof(TOUCH));
	if (!pTouch) {
		printf("Memory allocation failed!\n");
		return;
	}

	// USHORT screenWidth = 1920;
	// USHORT screenHeight = 1080;

	// USHORT normX = (USHORT)((x / (double)screenWidth) * 65535 / 2);
	// USHORT normY = (USHORT)((y / (double)screenHeight) * 65535 / 2);

	pTouch[0].ContactID = 0;
	pTouch[0].Status = MULTI_CONFIDENCE_BIT | MULTI_IN_RANGE_BIT | MULTI_TIPSWITCH_BIT;
	pTouch[0].XValue = normX;
	pTouch[0].YValue = normY;
	pTouch[0].Width = width;
	pTouch[0].Height = height;
	memcpy(&pTouch[1], &pTouch[0], sizeof(TOUCH));
	pTouch[1].ContactID = 1; // Ensure unique ID

	// printf("x: %d, y: %d\n", pTouch[0].XValue, pTouch[0].YValue);
	if (!vmulti_update_multitouch(vmulti, pTouch, 2))
		printf("vmulti_update_multitouch TOUCH_DOWN FAILED\n");
	free(pTouch); // Free memory before returning
	return;

	Sleep(pressDuration); // Sleep for pressDuration milliseconds

	pTouch[0].Status = 0;   // Lift fingers (TOUCH UP)
	pTouch[1].Status = 0;

	if (!vmulti_update_multitouch(vmulti, pTouch, 2))
		printf("vmulti_update_multitouch TOUCH_UP FAILED\n");

	free(pTouch);
}
