#include <iostream>
#include <windows.h>
#include <wingdi.h>
#include <chrono>

namespace {
HDC backBufferDC = nullptr;
HBITMAP backBufferBitmap = nullptr;

std::chrono::steady_clock::time_point startTime;
int frameCount = 0;
int currentFPS = 0;



void UpdateFPS(HWND hwnd) {
	frameCount++;

	// Calculate elapsed time since the start
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
			currentTime - startTime).count();

	// Recalculate FPS every second
	if (elapsedTime >= 30) {
		currentFPS = frameCount;
		frameCount = 0;
		startTime = currentTime;

		// Update the window title with FPS information
		std::wstring title = L"Your Window Title - FPS: "
				+ std::to_wstring(currentFPS * 1000 / 30);
		SetWindowTextW(hwnd, title.c_str());
	}
}

/*
 * Draws a 54-px width, horizontal line (from right to left) at x, y
 */
inline void drawLine(const HDC &hdc, int x, int y) {
	MoveToEx(hdc, x, y, nullptr);
	LineTo(hdc, x + 54, y);
}
// Used for drawing lines in the note icon.
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
}  // namespace
float viewportX, viewportY;
float noteX, noteY;

POINT lastMousePos;  // Stores the last mouse position
BOOL isPanning = FALSE;  // Indicates whether panning is active
int environmentX = 0;  // X-coordinate of the environment's top-left corner
int environmentY = 0;  // Y-coordinate of the environment's top-left corner

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_ERASEBKGND: {
//		HDC hdc = (HDC) wParam;

		return 1;
	}
	case WM_COMMAND:
		break;

	case WM_CLOSE:
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

	case WM_SIZE: {
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

		break;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		RECT clientRect;
		GetClientRect(hwnd, &clientRect);

		HBRUSH hBackground = CreateSolidBrush(0xE5F5FF);
		FillRect(backBufferDC, &clientRect, hBackground);
		DeleteObject(hBackground);

		RECT rect;
		GetClientRect(hwnd, &rect);
		auto width = rect.right - rect.left, height = rect.bottom - rect.top;
		auto centerX = width / 2, centerY = height / 2;

		drawNote(backBufferDC, centerX + noteX - viewportX,
				centerY + noteY - viewportY);

		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, backBufferDC, 0,
				0, SRCCOPY);

		UpdateFPS(hwnd);

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
	const char *className = "TextInputWindowClass";

	WNDCLASS wc = { };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = className;
	RegisterClass(&wc);

	HWND hwnd = CreateWindow(
			className,
			"Text Input Window",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
			NULL,
			NULL,
			hInstance,
			NULL
	);
	// Create a text input control
	CreateWindow(
			"EDIT",
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
			10, 10, 200, 25,
			hwnd,
			NULL,
			hInstance,
			NULL
	);
	// Check if V-Sync is supported
	BOOL vsyncSupported = SystemParametersInfo(SPI_GETFONTSMOOTHINGTYPE, 0,
			NULL, 0);

	if (vsyncSupported) {
		// V-Sync is supported, attempt to enable it
		BOOL result = SystemParametersInfo(SPI_SETFONTSMOOTHINGTYPE, 1, NULL,
				0);
		if (result) {
			// V-Sync enabled successfully
		} else {
			// V-Sync couldn't be enabled
			std::cerr
					<< "V-Sync couldn't be enabled. Your application will run without V-Sync."
					<< std::endl;
		}
	} else {
		// V-Sync is not supported
		std::cerr
				<< "V-Sync is not supported on this system. Your application will run without V-Sync."
				<< std::endl;
	}
	startTime = std::chrono::steady_clock::now();

	ShowWindow(hwnd, nCmdShow);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
