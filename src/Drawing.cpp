/*
 * Drawing.cpp
 *
 *  Created on: Nov 8, 2023
 *      Author: Zeale
 */

#include "Drawing.hpp"

#include <wingdi.h>
#include <winuser.h>
#include <iostream>

namespace drawing {

void drawFrame(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	HDC hdc = GetWindowDC(hwnd);

	if (hdc) {
		RECT rcWindow;
		GetWindowRect(hwnd, &rcWindow);
		int width = rcWindow.right - rcWindow.left;
		int height = rcWindow.bottom - rcWindow.top;

		COLORREF borderColor;

		std::cout << wParam;

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
void drawNote(const HDC &hdc, int screenx, int screeny) {
	// TODO Check to make sure note is in viewport

	// Draw the filled background
	HBRUSH hBackground = CreateSolidBrush(0xFFFFFF);
	PAINTSTRUCT ps;
	FillRect(hdc, &ps.rcPaint, hBackground);
	DeleteObject(hBackground);

	// Draw the rectangle border
	HPEN rectPen = CreatePen(PS_SOLID, 3, 0);
	HGDIOBJ hOldPen = SelectObject(hdc, rectPen);
	Rectangle(hdc, screenx, screeny, screenx + 75, screeny + 90);

	// Draw lines
	LOGBRUSH lb = {
	BS_SOLID, 0 };
	HPEN linePen = ExtCreatePen(
	PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, 3, &lb, 0,
			nullptr);
	SelectObject(hdc, linePen);
	DeleteObject(rectPen); // Delete rect pen after switching to line pen.

	drawLine(hdc, screenx += 10, screeny + 14);
	drawLine(hdc, screenx, screeny + 24);
	drawLine(hdc, screenx, screeny + 44);
	drawLine(hdc, screenx, screeny + 54);
	drawLine(hdc, screenx, screeny + 64);
	drawLine(hdc, screenx, screeny + 74);

	SelectObject(hdc, hOldPen);
	DeleteObject(linePen);
}

/*
 * Draws a 54-px width, horizontal line (from left to right) at x, y
 */
inline void drawLine(const HDC &hdc, int x, int y) {
	MoveToEx(hdc, x, y, nullptr);
	LineTo(hdc, x + 54, y);
}

}  // namespace drawing

