#ifndef WORLDPOSITION_H_
#define WORLDPOSITION_H_

class WorldPosition {
public:
	const long long chunkx, chunky;
	const float x, y;
	WorldPosition();
	WorldPosition(long long, long long, float, float);
	WorldPosition(const WorldPosition&);
	WorldPosition move(long long, long long, float, float);
	WorldPosition move(long long, long long);
	WorldPosition move(float, float);
	WorldPosition moveX(long long, float);
	WorldPosition moveY(long long, float);
	virtual ~WorldPosition();
};

#endif /* WORLDPOSITION_H_ */
