#include <iostream>
#include <windows.h>
#include <wingdi.h>

namespace {

// Used for drawing lines in the note icon.
class LineDrawer {
	const HDC &hdc;
public:
	int x, y;

	LineDrawer(const HDC &hdc, int initialX, int initialY) :
			hdc(hdc), x(initialX), y(initialY) {
	}

	void draw(int yshift) {
		Rectangle(hdc, x, y + yshift, x + 56, y + 1 + yshift);
	}
};

}  // namespace

void drawNote(const HDC &hdc, const int x, const int y) {
	PAINTSTRUCT ps;

	// Set the border color, background color, and stroke width
	int strokeWidth = 3; // Stroke width

	// Draw the filled background
	HBRUSH hBackground = CreateSolidBrush(0xFFFFFF);
	FillRect(hdc, &ps.rcPaint, hBackground);
	DeleteObject(hBackground);

	// Draw the rectangle border
	HPEN myPen = CreatePen(PS_ENDCAP_FLAT | PS_JOIN_MITER | PS_GEOMETRIC,
			strokeWidth, 0);
	HGDIOBJ hOldPen = SelectObject(hdc, myPen);

	Rectangle(hdc, x, y, x + 75, y + 90);

	int topShift = 15;
	LineDrawer drawer(hdc, x + 10, y + topShift);
	drawer.draw(0);
	drawer.draw(10);
	drawer.draw(30);
	drawer.draw(40);
	drawer.draw(50);
	drawer.draw(60);

	// Clean up GDI objects
	SelectObject(hdc, hOldPen);
	DeleteObject(myPen);
}

POINT lastMousePos;  // Stores the last mouse position
BOOL isPanning = FALSE;  // Indicates whether panning is active
int environmentX = 0;  // X-coordinate of the environment's top-left corner
int environmentY = 0;  // Y-coordinate of the environment's top-left corner

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_ERASEBKGND: {
		HDC hdc = (HDC) wParam;
		RECT rect;
		GetClientRect(hwnd, &rect);

		HBRUSH hBrush = CreateSolidBrush(0xE5F5FF);
		FillRect(hdc, &rect, hBrush);

		DeleteObject(hBrush);

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
			environmentX += deltaX;
			environmentY += deltaY;
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

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// Perform your drawing here based on environmentX and environmentY
		// You need to redraw the canvas or environment at the updated position

		// For demonstration purposes, let's draw a rectangle at the updated position
		Rectangle(hdc, 10 + environmentX, 10 + environmentY, 210 + environmentX,
				35 + environmentY);
		drawNote(hdc, 50, 50);

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

	ShowWindow(hwnd, nCmdShow);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
