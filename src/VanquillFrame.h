/*
 * VanquillFrame.h
 *
 *  Created on: Nov 8, 2023
 *      Author: Zeale
 */

#ifndef SRC_VANQUILLFRAME_H_
#define SRC_VANQUILLFRAME_H_

#include <windef.h>

/*
 * Class used to manage drawing of the window frame for Vanquill.
 */
class VanquillFrame {
	int top = 30, left = 5, bottom = 5, right = 5;
public:
	VanquillFrame();
	virtual ~VanquillFrame();
	void draw(const HDC&) const;
	int getBottom() const;
	int getLeft() const;
	int getRight() const;
	int getTop() const;
	/*
	 * Used to adjust the size of the client rect.
	 * This is designed to be called in a handler for the WM_NCCALCSIZE message.
	 */
	void adjustClientRect(RECT&) const;
};

#endif /* SRC_VANQUILLFRAME_H_ */
