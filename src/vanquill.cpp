#include <iostream>
#include <windows.h>
#include <wingdi.h>
#include <chrono>

namespace {

template<typename T>
struct Rect {
	T top, right, bottom, left;
};

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

template<typename T>
void printRect(const T &rect) {
	std::cout << "T: " << rect.top << ", L: " << rect.left << ", R: "
			<< rect.right << ", B: " << rect.bottom << std::endl;
}

/*
 * Draws a 54-px width, horizontal line (from right to left) at x, y
 */
inline void drawLine(const HDC &hdc, int x, int y) {
	MoveToEx(hdc, x, y, nullptr);
	LineTo(hdc, x + 54, y);
}

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

// Used for drawing lines in the note icon.
// Note icon is 76x96
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
int viewportX, viewportY, noteX, noteY;

POINT lastMousePos;  // Stores the last mouse position
BOOL isPanning = FALSE;  // Indicates whether panning is active
int environmentX = 0;  // X-coordinate of the environment's top-left corner
int environmentY = 0;  // Y-coordinate of the environment's top-left corner

/*
 * WndProc is the message handler that receives messages from the operating system used to handle input and
 * output and other events created by the user.
 *
 * 'hwnd' is the reference to window.
 *
 * 'msg' is the incoming message from the operating system.
 *
 * 'wParam' is used to carry information related to a specific message.
 *
 * 'lParam' is used to carry additional data related to a message.
 */

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	/*
	 * Used to run the correct code based on the incoming message.S
	 */

	switch (msg) {

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
		auto &width = rect.right, &height = rect.bottom;
		auto centerX = width / 2, centerY = height / 2;

		// TODO Use same coords
		Rect<long> viewportBounds = { viewportY, width + viewportX, height
				+ viewportY, viewportX };
		Rect<long> noteBounds = { noteY, noteX + 76, noteY + 96, noteX };

		std::cout << "\nViewport: ";
		printRect(viewportBounds);
		std::cout << "Note: ";
		printRect(noteBounds);

		drawNote(backBufferDC, noteX - viewportX, noteY - viewportY);
		if (rectIntersect(viewportBounds, noteBounds))
			std::cout << "In bounds" << std::endl;
		else
			std::cout << "Out of bounds" << std::endl;

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

/*
 * This is the main Windows function where the program begins.
 *
 * 'hInstance' is used to represent the instance handle of a Windows application.
 *
 * 'lpCmdLine' is a string that contains command-line arguments that might be
 * passed to the application.
 *
 * 'nCmdShow' is used to specify how the main window should be initially displayed.
 */

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) {
	const char *className = "TextInputWindowClass";

	/*
	 * Stores properties used to create our window
	 */

	WNDCLASS wc = { };

	/*
	 * Used to specify the memory address of the window procedure.
	 */

	wc.lpfnWndProc = WndProc;

	/*
	 * Used to represent the instance handle of a Windows application.
	 */

	wc.hInstance = hInstance;

	/*
	 * Used to specify the name of the window class.
	 */

	wc.lpszClassName = className;

	/*
	 * Used to register a window class with the Windows operating system
	 */

	RegisterClass(&wc);

	/*
	 * Creates Handle to Window function which is used to reference
	 * the main window that will open when the program is run.
	 *
	 * Initialised with values our window will be created with.S
	 */

	HWND hwnd = CreateWindow(
			className,
			"Text Input Window",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
			NULL,
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
  
  
	/*
	 * Begins clock that the FPS counter used to count FPS
	 */
	startTime = std::chrono::steady_clock::now();

	/*
	 * Displays the window using values specified in 'hwnd' and the WinMain function.
	 */

	ShowWindow(hwnd, nCmdShow);

	/*
	 * Begins the while-loop used to translate and send messages to WndProc by the
	 * Windows operating system.
	 */

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
