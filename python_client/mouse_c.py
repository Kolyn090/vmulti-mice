"""
DISCLAIMER:
This script interacts with system-level drivers using VMulti.
- It may require administrative privileges to run.
- Use at your own risk. The author is not responsible for any unintended behavior.
- Ensure you understand the implications of using virtual input devices before running.

REQUIREMENT:
- Windows XP or later, Windows 7 or later (Tested on Windows 11)
- 32bit Python
- Visual Studio 15.0 (2017) for compling

:author: Kolyn090, djpnewton(driver, test client code provider)
:email: kolynlin@protonmail.com
:license: MIT license
:date: 01/29/2025
"""


import ctypes
from screeninfo import get_monitors


class Mouse_C:
    """
    Python class for the Mouse that contains most of the functions in its C version.
    Find the C code in vmulti-mice/mice/mice.c, change and compile that script will
    generate a new mice.dll file under vmulti-mice/Debug. This class loads that .dll
    file in order to use the C code. For simplicity, you can use mouse.py instead.
    """
    def __init__(self, dll_path='../Debug/mice.dll'):
        self._vmulti_lib = ctypes.WinDLL(dll_path)

        # pvmulti_client CreateVmultiClient()
        self._vmulti_lib.CreateVmultiClient.restype = ctypes.c_void_p

        # pvmulti_client VmultiAlloc()
        self._vmulti_lib.VmultiAlloc.restype = ctypes.c_void_p

        # bool VmultiConnect(pvmulti_client vmulti)
        self._vmulti_lib.VmultiDisconnect.argtypes = [
            ctypes.c_void_p  # pvmulti_client (Pointer)
        ]
        self._vmulti_lib.VmultiDisconnect.restype = ctypes.c_bool

        # void VmultiDisconnect(pvmulti_client vmulti)
        self._vmulti_lib.VmultiDisconnect.argtypes = [
            ctypes.c_void_p  # pvmulti_client (Pointer)
        ]
        self._vmulti_lib.VmultiDisconnect.restype = None

        # void VmultiFree(pvmulti_client vmulti)
        self._vmulti_lib.VmultiFree.argtypes = [
            ctypes.c_void_p  # pvmulti_client (Pointer)
        ]
        self._vmulti_lib.VmultiFree.restype = None

        # void Click(pvmulti_client vmulti,
        # 	ushort normX,
        # 	ushort normY,
        # 	ushort width,
        # 	ushort height,
        # 	int pressDuration)
        self._vmulti_lib.Click.argtypes = [
            ctypes.c_void_p,  # pvmulti_client (Pointer)
            ctypes.c_ushort,  # normX
            ctypes.c_ushort,  # normY
            ctypes.c_ushort,  # width
            ctypes.c_ushort,  # height
            ctypes.c_int  # pressDuration
        ]
        self._vmulti_lib.Click.restype = None

    def create_client(self) -> ctypes.POINTER(ctypes.c_void_p):
        """
        Allocate and connect a new vmulti client instance.
        :return: vmulti client if success otherwise None
        """
        return self._vmulti_lib.CreateVmultiClient()

    def create_client_detailed(self) -> ctypes.POINTER(ctypes.c_void_p):
        """
        Allocate and connect a new vmulti client instance.
        Doing exact same work as create_client().
        :return: vmulti client if success otherwise None
        """
        client = self.vmulti_alloc()
        if client is None:
            print("Failed to allocate vmulti client.")
            return None
        if not self.vmulti_connect(client):
            print("Failed to connect vmulti client to driver.")
            self.vmulti_free(client)
            return None
        return client

    def vmulti_alloc(self) -> ctypes.POINTER(ctypes.c_void_p):
        """
        Allocate a new vmulti client instance.
        :return: A pointer to a new vmulti client
        """
        return self._vmulti_lib.VmultiAlloc()

    def vmulti_connect(self, client: ctypes.POINTER(ctypes.c_void_p)):
        """
        Connect the given vmulti client.
        :param client: vmulti client
        :return: True if the connection were successful False otherwise
        """
        return self._vmulti_lib.VmultiConnect(client)

    def vmulti_disconnect(self, client: ctypes.POINTER(ctypes.c_void_p)):
        """
        Disconnect the given vmulti client.
        :param client: vmulti client
        :return:
        """
        self._vmulti_lib.VmultiDisconnect(client)

    def vmulti_free(self, client: ctypes.POINTER(ctypes.c_void_p)):
        """
        Free the given vmulti client.
        :param client: vmulti client
        :return:
        """
        self._vmulti_lib.VmultiFree(client)

    def click(self, client: ctypes.POINTER(ctypes.c_void_p),
              x, y, touch_width=10, touch_height=10, press_duration=0.5):
        """
        Click the given position (x, y) on the screen with a HID vmulti device (if
        available). In this case it is acting as a mouse, and it won't move the
        system cursor, but it will disable the system cursor for a short amount of
        time. This means that they cannot coexist but the advantage is that the
        system cursor preserves its current position.

        This was achieved by making the computer to recognize the click as a
        multitouch action. Additionally, this program will perform the click
        TWICE on the given position because the minimal amount of touches needed
        for the computer to recognize it as a multitouch action is two.
        :param client: vmulti client
        :param x: x-coordinate, you can get this through pynput
        :param y: y-coordinate, you can get this through pynput
        :param touch_width: width of the touch contact area
        :param touch_height: height of the touch contact area
        :param press_duration: time spend on a click (in seconds)
        :return:
        """
        def normalize():
            monitor = get_monitors()[0]
            screen_width = monitor.width
            screen_height = monitor.height
            norm_x = x / screen_width * 65535 / 2
            norm_y = y / screen_height * 65535 / 2
            return int(norm_x), int(norm_y)

        norm = normalize()
        self._vmulti_lib.Click(client, norm[0], norm[1], touch_width, touch_height, int(press_duration*100))


if __name__ == '__main__':
    mouse_c = Mouse_C()
    vmulti_client = mouse_c.create_client()
    mouse_c.click(vmulti_client, 300, 300)
    mouse_c.vmulti_disconnect(vmulti_client)
    mouse_c.vmulti_free(vmulti_client)
