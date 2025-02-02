"""
DISCLAIMER:
This script interacts with system-level drivers using VMulti.
- It may require administrative privileges to run.
- Use at your own risk. The author is not responsible for any unintended behavior.
- Ensure you understand the implications of using virtual input devices before running.

REQUIREMENT:
- Windows XP or later, Windows 7 or later (Tested on Windows 11)
- 32bit Python
- Visual Studio 2015 (2017) for development

:author: Kolyn090, djpnewton(driver, test client code provider)
:email: kolynlin@protonmail.com
:license: MIT license
:date: 01/29/2025
"""


from mouse_c import Mouse_C


class Mouse:
    """
    The more user-friendly version of client code for interacting with the C library.
    This class instantiates a vmulti client instance as a property. Use context manager (with)
    if you do not want to manually free the space.
    """
    def __init__(self, dll_path='../DLL/mice.dll'):
        self._mouse_c = Mouse_C(dll_path)
        self._client = self._mouse_c.vmulti_alloc()
        self._has_connected = False

    def click(self, x, y, touch_width=10, touch_height=10, press_duration=0.5):
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
        :param x: x-coordinate, you can get this through pynput
        :param y: y-coordinate, you can get this through pynput
        :param touch_width: width of the touch contact area
        :param touch_height: height of the touch contact area
        :param press_duration: time spend on a click (in seconds)
        :return:
        """
        connection = True
        if not self._has_connected:
            connection = self._mouse_c.vmulti_connect(self._client)
            self._has_connected = True

        if connection:
            self._mouse_c.click(self._client, x, y, touch_width, touch_height, press_duration)

    def free(self):
        """
        Disconnect and free the vmulti client if it exists.
        :return:
        """
        if self._client:
            self._mouse_c.vmulti_disconnect(self._client)
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
