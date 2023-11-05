#ifndef UPDATE_FPS_HPP_
#define UPDATE_FPS_HPP_

#include <Windows.h>
#include <chrono>
#include <string>

class UpdateFPS {
	std::chrono::steady_clock::time_point startTime;
	int frameCount = 0, currentFPS = 0;
public:
	void updateFPS(HWND hwnd);
};

#endif /* UPDATE_FPS_HPP_ */
