/*
 * Drawing.hpp
 *
 *  Created on: Nov 8, 2023
 *      Author: Zeale
 */

#ifndef SRC_DRAWING_HPP_
#define SRC_DRAWING_HPP_

#include <minwindef.h>
#include <windef.h>

namespace drawing {

void drawFrame(HWND, WPARAM, LPARAM);
void drawNote(const HDC&, int, int);
inline void drawLine(const HDC&, int, int);

}  // namespace drawing

#endif /* SRC_DRAWING_HPP_ */
