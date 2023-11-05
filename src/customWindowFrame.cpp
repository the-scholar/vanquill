#include <windows.h>
#include <iostream>
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
	HDC hdc = GetWindowDC(hwnd);

	if (hdc) {
		RECT rcWindow;
		GetWindowRect(hwnd, &rcWindow);
		int width = rcWindow.right - rcWindow.left;
		int height = rcWindow.bottom - rcWindow.top;

		COLORREF borderColor;

		std::cout<<wParam;

		if (wParam) {
			// The window is being activated, use an active border color
			borderColor = RGB(0, 0, 255);
		} else {
			// The window is being deactivated (inactive), use an inactive border color
			borderColor = RGB(128, 128, 128);
		}

		HPEN hBorderPen = CreatePen(PS_SOLID, 3, borderColor);
		HPEN hOldPen = (HPEN) SelectObject(hdc, hBorderPen);

		// Draw the border to cover the entire non-client area
		Rectangle(hdc, 0, 0, width, height);

		// Clean up
		SelectObject(hdc, hOldPen);
		DeleteObject(hBorderPen);
		ReleaseDC(hwnd, hdc);
	}
}
