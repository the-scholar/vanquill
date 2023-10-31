#include "Note.h"

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

