#include <iostream>
#include <windows.h>

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) {
	HWND hwnd =
			CreateWindow(0, L"STATIC", L"Minimal Window", WS_OVERLAPPEDWINDOW, 100, 100, 640, 480, nullptr, nullptr, nullptr, nullptr);
	ShowWindow(hwnd, nCmdShow);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
