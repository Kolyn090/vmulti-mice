from mouse_c import Mouse_C


class Mouse:
    def __init__(self, dll_path='../Debug/mice.dll'):
        self._mouse_c = Mouse_C(dll_path)

    def click(self, x, y, touch_width=10, touch_height=10, press_duration=0.5):
        vmulti_client = self._mouse_c.create_client()
        self._mouse_c.click(vmulti_client, x, y, touch_width, touch_height, press_duration)
        self._mouse_c.vmulti_disconnect(vmulti_client)
        self._mouse_c.vmulti_free(vmulti_client)


if __name__ == '__main__':
    mouse = Mouse()
    mouse.click(300, 300)
