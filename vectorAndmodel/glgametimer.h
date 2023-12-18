#ifndef __GL_GAMETIMER__
#define __GL_GAMETIMER__
#include"glFrameWork.h"

class glgametimer
{
private:
	float prev;
	float delta;
	bool ifpause;
public:
	glgametimer();
	glgametimer(const glgametimer&) = delete;
	glgametimer& operator=(const glgametimer&) = delete;

	void tick();
	void stop();
	void restart();
	void reset();

	float fps();
	float getdelta();
};

#endif // !__GL_GAMETIMER__

