#include "Note.h"

namespace {
/*
 * Draws a 54-px width, horizontal line (from right to left) at x, y
 */
inline void drawLine(const HDC &hdc, int x, int y) {
	MoveToEx(hdc, x, y, nullptr);
	LineTo(hdc, x + 54, y);
}
}

Note::~Note() {
}

Note::Note(long long chunkx, long long chunky, float x, float y) :
		Note(WorldPosition(chunkx, chunky, x, y)) {
}

Note::Note(WorldPosition pos) :
		pos(pos) {
}

const WorldPosition& Note::getPos() const {
	return pos;
}

WorldPosition& Note::getPosRef() {
	return pos;
}

void Note::setPos(const WorldPosition &pos) {
	this->pos = pos;
}

void Note::draw(const HDC &hdc) const {

	// TODO Check to make sure note is in viewport

	// TODO Set calculate screen position:
	int screenx, screeny;

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
