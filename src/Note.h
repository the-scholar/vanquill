#ifndef NOTE_H_
#define NOTE_H_

#include <windows.h>
#include <wingdi.h>
#include "WorldPosition.h"

class Note {
	WorldPosition pos;
public:
	Note(long long chunkx, long long chunky, float x, float y);
	Note(WorldPosition pos);
	virtual ~Note();
	void draw(const HDC &hdc) const;
	const WorldPosition& getPos() const;
	void setPos(const WorldPosition &pos);
};

#endif /* NOTE_H_ */
