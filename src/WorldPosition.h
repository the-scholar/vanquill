#ifndef WORLDPOSITION_H_
#define WORLDPOSITION_H_

class WorldPosition {
public:
	long long chunkx, chunky;
	float x, y;
	WorldPosition();
	WorldPosition(long long, long long, float x, float y);
	WorldPosition move(long long, long long, float, float);
	WorldPosition move(long long, long long);
	WorldPosition move(float, float);
	WorldPosition moveX(long long, float);
	WorldPosition moveY(long long, float);
	virtual ~WorldPosition();
};

#endif /* WORLDPOSITION_H_ */
