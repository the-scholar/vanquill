#ifndef NOTE_H_
#define NOTE_H_

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
};

#endif /* NOTE_H_ */
