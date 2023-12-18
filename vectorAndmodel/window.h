#ifndef __GL_WINDOW__
#define __GL_WINDOW__
#define __ZAXIC_GL_ERROR_PROC__
#define __GL_MATHEMATICS__
#include"glFrameWork.h"
#include"camera.h"
#include"glgametimer.h"

#define DEFAULT_SENSITIVITY 0.5f

enum class WndCallBackType {
	FrameBufferResize,
	CursorPos,
	CursorClick,
	Scroll
};

class glwindow
{
public:
	int width, height;


	
public:
	GLFWwindow* window;
	glgametimer _tm;
	float sensitivity;
	float mpos_x, mpos_y;
	double mscof_x, mscof_y;
	bool mfir, mscfir;

	glwindow() = delete;
	glwindow(uint32_t width, uint32_t height, bool ifMSAA, int MSAAcount, GLFWframebuffersizefun func = nullptr, GLFWcursorposfun func2 = nullptr, GLFWscrollfun func3 = nullptr);
	glwindow(const glwindow&) = delete;
	glwindow& operator=(const glwindow&) = delete;

	void setWndCallBack(WndCallBackType type, void* func);

	float ratio();

	//override
	//virtual void processInput(glCamera* g_Camera);
};
//extern glwindow* g_wnd;
#endif
