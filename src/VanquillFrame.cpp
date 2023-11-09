/*
 * VanquillFrame.cpp
 *
 *  Created on: Nov 8, 2023
 *      Author: Zeale
 */

#include "VanquillFrame.h"

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

void VanquillFrame::draw(const HDC &hdc) const {

}

void VanquillFrame::adjustClientRect(RECT &clientRect) const {
	clientRect.left += getLeft();
	clientRect.top += getTop();
	clientRect.right -= getRight();
	clientRect.bottom -= getBottom();
}
