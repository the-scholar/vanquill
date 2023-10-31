#ifndef NOTE_H_
#define NOTE_H_

#include <windows.h>
#include <wingdi.h>

class Note {
	long long chunkx, chunky;
	float x, y;

public:
	Note(long long chunkx, long long chunky, float x, float y);
	virtual ~Note();
	long long getChunkx() const;
	void setChunkx(long long chunkx);
	long long getChunky() const;
	void setChunky(long long chunky);
	float getX() const;
	void setX(float x);
	float getY() const;
	void setY(float y);
	void draw(const HDC &hdc) const;
};

#endif /* NOTE_H_ */
