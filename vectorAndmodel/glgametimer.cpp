#include"glgametimer.h"

glgametimer::glgametimer()
{
	delta = 0;
	prev = glfwGetTime();
	ifpause = false;
}

void glgametimer::tick()
{
	if (ifpause)
	{
		prev = glfwGetTime();
		return;
	}
	float crr = static_cast<float>(glfwGetTime());
	delta = crr - prev;
	prev = crr;
}

void glgametimer::stop()
{
	ifpause = true;
}

void glgametimer::restart()
{
	if (ifpause)
	{
		prev = glfwGetTime();
		ifpause = false;
	}
	return;
}

void glgametimer::reset()
{
	prev = glfwGetTime();
	delta = 0;
}

float glgametimer::fps()
{
	if (delta)
		return 1.0f / delta;
	else
		return 0.;
}

float glgametimer::getdelta()
{
	return delta;
}
