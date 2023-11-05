#include "updateFPS.hpp"


void UpdateFPS::updateFPS(HWND hwnd) {
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
