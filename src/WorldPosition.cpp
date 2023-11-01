#include "WorldPosition.h"

WorldPosition::WorldPosition() :
		WorldPosition(0, 0, 0, 0) {
}

WorldPosition::WorldPosition(const WorldPosition &pos) :
		chunkx(pos.chunkx), chunky(pos.chunky), x(pos.x), y(pos.y) {
}

WorldPosition::~WorldPosition() {
}

WorldPosition::WorldPosition(long long chunkx, long long chunky, float x,
		float y) :
		chunkx(chunkx), chunky(chunky), x(x), y(y) {

}

WorldPosition WorldPosition::move(long long chunkx, long long chunky, float x,
		float y) {
	return WorldPosition(this->chunkx + chunkx, this->chunky + chunky,
			this->x + x, this->y + y);
}

WorldPosition WorldPosition::move(long long chunkx, long long chunky) {
	return move(chunkx, chunky, 0, 0);
}

WorldPosition WorldPosition::move(float x, float y) {
	return move(0, 0, x, y);
}

WorldPosition WorldPosition::moveX(long long chunkx, float x) {
	return move(chunkx, 0, x, 0);
}

WorldPosition WorldPosition::moveY(long long chunky, float y) {
	return move(0, chunky, 0, y);
}
