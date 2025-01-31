from mouse_c import Mouse_C


class Mouse:
    def __init__(self, dll_path='../Debug/mice.dll'):
        self._mouse_c = Mouse_C(dll_path)
        self._client = self._mouse_c.vmulti_alloc()

    def click(self, x, y, touch_width=10, touch_height=10, press_duration=0.5):
        self._mouse_c.vmulti_connect(self._client)
        self._mouse_c.click(self._client, x, y, touch_width, touch_height, press_duration)
        self._mouse_c.vmulti_disconnect(self._client)

    def free(self):
        if self._client:
            self._mouse_c.vmulti_free(self._client)  # Free the client
            self._client = None  # Avoid double freeing

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.free()

if __name__ == '__main__':
    mouse = Mouse()
    with Mouse() as m:
        m.click(300, 300)
