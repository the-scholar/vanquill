#include <minwindef.h>
#include <windef.h>
#include <wingdi.h>
#include <winnt.h>
#include <winuser.h>
#include <iostream>
#include <string>

#include "Drawing.hpp"
#include "FPSCounter.hpp"
#include "VanquillFrame.h"

namespace {

template<typename T>
inline void printRect(const T &rect) {
	std::cout << "T: " << rect.top << ", L: " << rect.left << ", R: "
			<< rect.right << ", B: " << rect.bottom << std::endl;
}

template<typename T>
struct Rect {
	T top, right, bottom, left;
};

template<typename T>
inline bool rectIntersect(const T &top1, const T &right1, const T &left1,
		const T &bottom1, const T &top2, const T &right2, const T &left2,
		const T &bottom2) {
	return left1 <= right2 && left2 <= right1 && top1 <= bottom2
			&& top2 <= bottom1;
}

template<typename T>
inline bool rectIntersect(Rect<T> first, Rect<T> second) {
	return rectIntersect(first.top, first.right, first.left, first.bottom,
			second.top, second.right, second.left, second.bottom);
}

template<typename T>
void print(T item) {
	std::cout << item << std::endl;
}

}  // namespace

int viewportX, viewportY, noteX, noteY;

POINT lastMousePos;  // Stores the last mouse position
BOOL isPanning = FALSE;  // Indicates whether panning is active
int environmentX = 0;  // X-coordinate of the environment's top-left corner
int environmentY = 0;  // Y-coordinate of the environment's top-left corner

HDC backBufferDC = nullptr;
HBITMAP backBufferBitmap = nullptr;

VanquillFrame frame;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	/*
	 * Used to run the correct code based on the incoming message.S
	 */

	switch (msg) {
	case WM_COMMAND:
		break;
	case WM_CLOSE:
		print("WM_CLOSE");
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		lastMousePos.x = LOWORD(lParam);
		lastMousePos.y = HIWORD(lParam);
		isPanning = TRUE;
		SetCapture(hwnd);
		return 0;
	case WM_MOUSEMOVE:
		if (isPanning) {
			int deltaX = LOWORD(lParam) - lastMousePos.x;
			int deltaY = HIWORD(lParam) - lastMousePos.y;
			viewportX -= deltaX;
			viewportY -= deltaY;
			lastMousePos.x = LOWORD(lParam);
			lastMousePos.y = HIWORD(lParam);

			// Redraw the environment with the new position
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;  // Make sure to break here
	case WM_LBUTTONUP:
		if (isPanning) {
			isPanning = FALSE;
			ReleaseCapture();
		}
		return 0;

	case WM_SIZE:
		print("WM_SIZE");
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			RECT clientRect;
			GetClientRect(hwnd, &clientRect);

			if (backBufferDC)
				DeleteDC(backBufferDC);
			if (backBufferBitmap)
				DeleteObject(backBufferBitmap);

			backBufferDC = CreateCompatibleDC(hdc);
			backBufferBitmap = CreateCompatibleBitmap(hdc, clientRect.right,
					clientRect.bottom);

			SelectObject(backBufferDC, backBufferBitmap);

			EndPaint(hwnd, &ps);
			break;
		}

	case WM_NCPAINT:
		print("WM_NCPAINT");
		{
			frame.draw(hwnd, wParam, lParam);
			return 0;
		}

	case WM_NCCALCSIZE:
		print("WM_NCCALCSIZE");
		if (wParam) {

			NCCALCSIZE_PARAMS *params = (NCCALCSIZE_PARAMS*) lParam;
			frame.adjustClientRect(params->rgrc[0]);

			return WVR_REDRAW; // Signal to recalculate the client area of the window.
		}

		break;

		/*
		 * This message is sent by the OS when a window becomes deactivated or reactivated.
		 * By intercepting it and running our own code, we stop the window from reverting
		 * back to the original frame styling.
		 */

	case WM_NCACTIVATE:
		print("WM_NCACTIVATE");
		{

			frame.draw(hwnd, wParam, lParam);

			break;
		}

	case WM_SHOWWINDOW:
		print("WM_SHOWWINDOW");
		RedrawWindow(hwnd, NULL, NULL,
		RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
		break;
	case WM_CREATE:
		print("WM_CREATE");
		{

			/*
			 * Get the window rect.
			 */

			RECT rcClient;
			GetWindowRect(hwnd, &rcClient);

			SetWindowPos(hwnd, NULL, 100, 100, 900, 600, SWP_FRAMECHANGED);

			break;
		}

	case WM_PAINT:
		print("WM_PAINT");
		{

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			drawing::drawScene(hdc);

			RECT clientRect;
			GetClientRect(hwnd, &clientRect);

			HBRUSH hBackground = CreateSolidBrush(0xE5F5FF);
			FillRect(backBufferDC, &clientRect, hBackground);
			DeleteObject(hBackground);

			RECT rect;
			GetClientRect(hwnd, &rect);
			auto &width = rect.right, &height = rect.bottom;
			auto centerX = width / 2, centerY = height / 2;

			// TODO Use same coords
			Rect<long> viewportBounds = { viewportY, width + viewportX, height
					+ viewportY, viewportX };
			Rect<long> noteBounds = { noteY, noteX + 76, noteY + 96, noteX };

			drawing::drawNote(backBufferDC, noteX - viewportX,
					noteY - viewportY);

			BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, backBufferDC,
					0, 0, SRCCOPY);

			/*
			 * This creates a new 'UpdateFPS' object which handles the FPS counter in the title bar
			 * of the window for development reasons.
			 */

			FPSCounter().updateFPS(hwnd);

			EndPaint(hwnd, &ps);
			return 0;
		}

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) {
	// Window class
	const char *className = "TextInputWindowClass";
	WNDCLASS wc = { };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = className;
	RegisterClass(&wc);

	// Window
	HWND hwnd = CreateWindowEx(
	WS_EX_ACCEPTFILES, className, "Text Input Window",
	WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT, CW_USEDEFAULT, 0, 0,
	NULL,
	NULL, hInstance,
	NULL);

	ShowWindow(hwnd, nCmdShow);

	// Msg loop
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
