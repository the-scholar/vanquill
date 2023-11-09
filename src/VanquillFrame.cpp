/*
 * VanquillFrame.cpp
 *
 *  Created on: Nov 8, 2023
 *      Author: Zeale
 */

#include "VanquillFrame.h"

#include <minwindef.h>
#include <wingdi.h>
#include <winuser.h>
#include <iostream>

VanquillFrame::VanquillFrame() {
}

VanquillFrame::~VanquillFrame() {
}

int VanquillFrame::getBottom() const {
	return bottom;
}

int VanquillFrame::getLeft() const {
	return left;
}

int VanquillFrame::getRight() const {
	return right;
}

int VanquillFrame::getTop() const {
	return top;
}

void VanquillFrame::draw(HWND hwnd, WPARAM wParam, LPARAM lParam) const {
	HDC hdc = GetWindowDC(hwnd);

	if (hdc) {
		RECT rcWindow;
		GetWindowRect(hwnd, &rcWindow);
		int width = rcWindow.right - rcWindow.left;
		int height = rcWindow.bottom - rcWindow.top;

		COLORREF borderColor = wParam ? 0 : 0x777777;
		HBRUSH brush = CreateSolidBrush(borderColor);

		HPEN hBorderPen = CreatePen(PS_SOLID, 30, borderColor);
		HPEN hOldPen = (HPEN) SelectObject(hdc, hBorderPen);

		// Draw the border to cover the entire non-client area
		RECT rect = { 0, 0, width, getTop() };
		FillRect(hdc, &rect, brush); // Top

		rect = { 0, 0, getLeft(), height };
		FillRect(hdc, &rect, brush); // Left

		rect = { 0, height - getBottom(), width, height };
		FillRect(hdc, &rect, brush); // Bottom

		rect = { width - getRight(), 0, width, height };
		FillRect(hdc, &rect, brush); // Right

		// Clean up
		SelectObject(hdc, hOldPen);
		DeleteObject(hBorderPen);
		ReleaseDC(hwnd, hdc);
	}
}

void VanquillFrame::adjustClientRect(RECT &clientRect) const {
	clientRect.left += getLeft();
	clientRect.top += getTop();
	clientRect.right -= getRight();
	clientRect.bottom -= getBottom();
}
