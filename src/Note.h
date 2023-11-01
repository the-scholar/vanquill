#ifndef NOTE_H_
#define NOTE_H_

#include <windows.h>
#include <wingdi.h>
#include "WorldPosition.h"

class Note {
	WorldPosition pos;
public:
	Note(long long, long long, float, float);
	Note(const WorldPosition&);
	Note();
	virtual ~Note();
	void draw(const HDC&, int, int) const;
	const WorldPosition& getPos() const;
	WorldPosition& getPosRef();
	void setPos(const WorldPosition &);
};

#endif /* NOTE_H_ */
