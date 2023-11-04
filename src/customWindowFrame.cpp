#include <windows.h>
#include "customWindowFrame.hpp"

/*
 * This source file is used for drawing the window frame.
 *
 * 'hwnd' is used to get the handle to the window.
 *
 * 'wParam' is used to read any data given by the message from the operating system.
 *
 * 'lParam' is used to read any other data given by the message from the operating system.
 */

void CustomWindowFrame::customWindowFrame(HWND hwnd, WPARAM wParam,
		LPARAM lParam) {

	/*
	 * Creates a new device context to a device context used for drawing operations.
	 */

	HDC hdc = GetWindowDC(hwnd);

	if (hdc) {
		RECT rcWindow;
		GetWindowRect(hwnd, &rcWindow);

		// Define the red border color
		COLORREF borderColor = RGB(255, 0, 0);

		// Create a red pen for drawing with a thicker line (3 pixels in this example)
		HPEN hRedPen = CreatePen(PS_SOLID, 3, borderColor);
		HPEN hOldPen = (HPEN) SelectObject(hdc, hRedPen);

		// Draw the border to cover the entire non-client area
		Rectangle(hdc, 0, 0, rcWindow.right - rcWindow.left,
				rcWindow.bottom - rcWindow.top);

		// Clean up
		SelectObject(hdc, hOldPen);
		DeleteObject(hRedPen);
		ReleaseDC(hwnd, hdc);
	}
}
