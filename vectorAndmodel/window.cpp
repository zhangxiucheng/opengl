#include"window.h"
//glwindow* g_wnd = nullptr;
glwindow::glwindow(uint32_t _width, uint32_t _height, bool ifMSAA, int MSAAcount, GLFWframebuffersizefun func,GLFWcursorposfun func2, GLFWscrollfun func3):_tm()
{
	/*if (g_wnd != nullptr)
		return;
	else
		g_wnd = this;*/

	if (!glfwInit())
	{
		std::string c = "glfwInit() Failed!";
		g_log->newlog(c);
		return;
	}
	//glfw initialize



	//glad init glfw
	//set maxmin opengl version supported by glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//set core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (ifMSAA)
	{
		glfwWindowHint(GLFW_SAMPLES, MSAAcount);
	}

	window = glfwCreateWindow(_width, _height, "Hello World", nullptr, nullptr);
	
	if (!window)
	{
		glfwTerminate();
		std::string c("glfwCreateWindow Failed!");
		g_log->newlog(c);
	}
	//set view port
	//(0,0) controls the position of window's left bottom point
	//auto resize window
	glfwMakeContextCurrent(window);
	//glViewport(0, 0, 800, 600);
	if (func)
		glfwSetFramebufferSizeCallback(window, func);
	if (func2)
		glfwSetCursorPosCallback(window, func2);
	if (func3)
		glfwSetScrollCallback(window, func3);

	mfir = true;
	mscfir = true;
	sensitivity = DEFAULT_SENSITIVITY;
	width = _width;
	height = _height;
}

void glwindow::setWndCallBack(WndCallBackType type, void* func)
{
	if (func == nullptr)
		return;
	switch (type)
	{
	case WndCallBackType::FrameBufferResize:
		glfwSetFramebufferSizeCallback(window, (GLFWframebuffersizefun)func);
		break;
	case WndCallBackType::CursorPos:
		glfwSetCursorPosCallback(window, (GLFWcursorposfun)func);
		break;
	case WndCallBackType::CursorClick:
		glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)func);
		break;
	case WndCallBackType::Scroll:
		glfwSetScrollCallback(window, (GLFWscrollfun)func);
		break;
	default:
		break;
	}
}

float glwindow::ratio()
{
	return float(width) / float(height);
}

//void glwindow::processInput(glCamera* g_Camera)
//{
//	_tm.tick();
//	bool ifangel = false;
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//
//	
//	float cmspeed = g_Camera->cmspeed() * _tm.getdelta();
//
//
//	if (glfwGetKey(g_wnd->window, GLFW_KEY_W) == GLFW_PRESS)
//		g_Camera->move(g_Camera->getYn() * cmspeed);
//	if (glfwGetKey(g_wnd->window, GLFW_KEY_S) == GLFW_PRESS)
//		g_Camera->move(-g_Camera->getYn() * cmspeed);
//	if (glfwGetKey(g_wnd->window, GLFW_KEY_A) == GLFW_PRESS)
//		g_Camera->move(-g_Camera->getXn() * cmspeed);
//	if (glfwGetKey(g_wnd->window, GLFW_KEY_D) == GLFW_PRESS)
//		g_Camera->move(g_Camera->getXn() * cmspeed);
//	if (glfwGetKey(g_wnd->window, GLFW_KEY_Q) == GLFW_PRESS)
//		g_Camera->move(-g_Camera->getZn() * cmspeed);
//	if (glfwGetKey(g_wnd->window, GLFW_KEY_E) == GLFW_PRESS)
//		g_Camera->move(g_Camera->getZn() * cmspeed);
//
//	if (glfwGetKey(g_wnd->window, GLFW_KEY_I) == GLFW_PRESS)
//		g_Camera->Turn(glCameraDirection::Up, -3.0f);
//	if (glfwGetKey(g_wnd->window, GLFW_KEY_K) == GLFW_PRESS)
//		g_Camera->Turn(glCameraDirection::Up, 3.0f);
//	if (glfwGetKey(g_wnd->window, GLFW_KEY_J) == GLFW_PRESS)
//		g_Camera->Turn(glCameraDirection::Right, -3.0f);
//	if (glfwGetKey(g_wnd->window, GLFW_KEY_L) == GLFW_PRESS)
//		g_Camera->Turn(glCameraDirection::Right, 3.0f);
//	if (glfwGetKey(g_wnd->window, GLFW_KEY_U) == GLFW_PRESS)
//		g_Camera->Turn(glCameraDirection::Front, -3.0f);
//	if (glfwGetKey(g_wnd->window, GLFW_KEY_O) == GLFW_PRESS)
//		g_Camera->Turn(glCameraDirection::Front, 3.0f);
//
//	if (glfwGetKey(g_wnd->window, GLFW_KEY_F2) == GLFW_PRESS)
//		glfwSetInputMode(g_wnd->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	if (glfwGetKey(g_wnd->window, GLFW_KEY_F3) == GLFW_PRESS)
//		glfwSetInputMode(g_wnd->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//
//}
