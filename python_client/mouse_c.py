import ctypes
from screeninfo import get_monitors


class Mouse_C:
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
        return self._vmulti_lib.CreateVmultiClient()

    def create_client_detailed(self) -> ctypes.POINTER(ctypes.c_void_p):
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
        return self._vmulti_lib.VmultiAlloc()

    def vmulti_connect(self, client: ctypes.POINTER(ctypes.c_void_p)):
        return self._vmulti_lib.VmultiConnect(client)

    def vmulti_disconnect(self, client: ctypes.POINTER(ctypes.c_void_p)):
        self._vmulti_lib.VmultiDisconnect(client)

    def vmulti_free(self, client: ctypes.POINTER(ctypes.c_void_p)):
        self._vmulti_lib.VmultiFree(client)

    def click(self, client: ctypes.POINTER(ctypes.c_void_p),
              x, y, touch_width=10, touch_height=10, press_duration=0.5):
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
