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

long long Note::getChunkx() const {
	return chunkx;
}

void Note::setChunkx(long long chunkx) {
	this->chunkx = chunkx;
}

long long Note::getChunky() const {
	return chunky;
}

void Note::setChunky(long long chunky) {
	this->chunky = chunky;
}

float Note::getX() const {
	return x;
}

void Note::setX(float x) {
	this->x = x;
}

float Note::getY() const {
	return y;
}

Note::~Note() {
}

Note::Note(long long chunkx, long long chunky, float x, float y) :
		chunkx(chunkx), chunky(chunky), x(x), y(y) {
}

void Note::setY(float y) {
	this->y = y;
}

void Note::draw(const HDC &hdc) const {

	// TODO Check to make sure note is in viewport

	// Draw the filled background
	HBRUSH hBackground = CreateSolidBrush(0xFFFFFF);
	PAINTSTRUCT ps;
	FillRect(hdc, &ps.rcPaint, hBackground);
	DeleteObject(hBackground);

	// Draw the rectangle border
	HPEN rectPen = CreatePen(PS_SOLID, 3, 0);
	HGDIOBJ hOldPen = SelectObject(hdc, rectPen);
	Rectangle(hdc, x, y, x + 75, y + 90);

	// Draw lines
	LOGBRUSH lb = {
	BS_SOLID, 0 };
	HPEN linePen = ExtCreatePen(
	PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, 3, &lb, 0,
			nullptr);
	SelectObject(hdc, linePen);
	DeleteObject(rectPen); // Delete rect pen after switching to line pen.

	int screenx, screeny;
	LineDrawer drawer(hdc, x + 10, y + 14);
	drawer.draw(0);
	drawer.draw(10);
	drawer.draw(30);
	drawer.draw(40);
	drawer.draw(50);
	drawer.draw(60);
}
