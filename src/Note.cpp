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

const WorldPosition& Note::getPos() const {
	return pos;
}

WorldPosition& Note::getPosRef() {
	return pos;
}

Note::Note() :
		Note(WorldPosition()) {
}

Note::Note(const WorldPosition &pos) :
		pos(pos) {
}

void Note::setPos(const WorldPosition &pos) {
	WorldPosition myPos(pos);
	this->pos = myPos;
}

void Note::draw(const HDC &hdc, int screenx, int screeny) const {


}
