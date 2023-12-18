#define __ZAXIC_GL_DATA__
#define __ZAXIC_GL_ERROR_PROC__
#define __GL_MATHEMATICS__
#include"ZaxicEngine.h"
#define ANIMATE_FRAME_COUNT 30.0f
glwindow* g_wnd;
glCamera* g_Camera;
shaderProgram* g_sp_ptr = nullptr;
glModel* g_model_ptr = nullptr;
//std::vector<glm::vec4>* g_colorV_ptr = nullptr;
bool view_euler = true, animating = false, ifpause = false, ifspace = false;
float animateF = 0.0f;

int chosenCar = -1;


void frambuffer_size_callback(GLFWwindow* wnd, int width, int height)
{
	//reset viewport
	glViewport(0, 0, width, height);

	std::string name = "proj";
	//glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.01f, 100.0f);
	g_wnd->width = width;
	g_wnd->height = height;
	//update projection matrix
}

//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	if (g_wnd->mfir)
//	{
//		g_wnd->mpos_x = xpos;
//		g_wnd->mpos_y = ypos;
//		g_wnd->mfir = false;
//	}
//
//	float xoffset = (xpos - g_wnd->mpos_x) * g_wnd->sensitivity;
//	float yoffset = (ypos - g_wnd->mpos_y) * g_wnd->sensitivity;
//	g_wnd->mpos_x = xpos;
//	g_wnd->mpos_y = ypos;
//
//	
//	//update
//	g_Camera->el.set(EulerMember::yaw, xoffset);
//	g_Camera->el.set(EulerMember::pitch, yoffset);
//
//	/*g_Camera->Turn(glCameraDirection::Right, xoffset);
//	g_Camera->Turn(glCameraDirection::Up, yoffset);*/
//	/*glm::vec3 newEye;
//	newEye.x = cos(g_Camera->el.get(EulerMember::pitch)) * cos(g_Camera->el.get(EulerMember::yaw));
//	newEye.y = sin(g_Camera->el.get(EulerMember::pitch));
//	newEye.z = cos(ZaxicMathUtil::toRadians(g_Camera->el.get(EulerMember::pitch))) * sin(g_Camera->el.get(EulerMember::yaw));
//
//	g_Camera->updatevf(newEye, glCameraMember::CameraEyeLight);
//	g_Camera->updatevf(glm::cross(glm::cross(newEye, g_Camera->getYn()), newEye), glCameraMember::CameraUpAxis);*/
//
//}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (animating)
		return;
	float _fov = g_Camera->fov - yoffset;
	if (_fov >= 1.0f && _fov <= 45.0f)
		g_Camera->fov = _fov;
	if (_fov < 1.0f)
		g_Camera->fov = 1.0f;
	if (_fov > 45.0f)
		g_Camera->fov = 45.0f;
}


void mouse_click_callback(GLFWwindow* window, int button, int action, int targ)
{
	if (animating)
		return;
	unsigned char buffer;

	if (action == GLFW_PRESS)
	{
		double posx, posy;
		GLint view[4];
		glGetIntegerv(GL_VIEWPORT, view);
		glfwGetCursorPos(window, &posx, &posy);
		//std::cout << posx << ',' << posy << std::endl;
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			//process left button click
			glReadPixels(posx, static_cast<double>(view[3]) - posy, 1, 1, GL_ALPHA, GL_UNSIGNED_BYTE, &buffer);
			if ((buffer > 7) || (buffer < 0))
				chosenCar = 0;
			else
				chosenCar = buffer;
			//std::cout << chosenCar << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			break;
		default:
			break;
		}
	}
}

void processInput()
{
	g_wnd->_tm.tick();
	if (animating)
		return;
	bool ifangel = false;
	if (glfwGetKey(g_wnd->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(g_wnd->window, true);


	float cmspeed = g_Camera->cmspeed() * g_wnd->_tm.getdelta();
	float delta = g_wnd->sensitivity * 3.0f;
	glm::mat4 mat = transZaxicMatToGLM(g_Camera->el.EulerMatrix());
	glm::mat3 _mat = glm::mat3(mat);

	if (glfwGetKey(g_wnd->window, GLFW_KEY_1) == GLFW_PRESS)
		view_euler = true;
	if (glfwGetKey(g_wnd->window, GLFW_KEY_2) == GLFW_PRESS)
		view_euler = false;

	if (glfwGetKey(g_wnd->window, GLFW_KEY_W) == GLFW_PRESS)
		g_Camera->move(g_Camera->getYn() * _mat * cmspeed);
	if (glfwGetKey(g_wnd->window, GLFW_KEY_S) == GLFW_PRESS)
		g_Camera->move(-g_Camera->getYn() * _mat * cmspeed);
	if (glfwGetKey(g_wnd->window, GLFW_KEY_A) == GLFW_PRESS)
		g_Camera->move(-g_Camera->getXn() * _mat * cmspeed);
	if (glfwGetKey(g_wnd->window, GLFW_KEY_D) == GLFW_PRESS)
		g_Camera->move(g_Camera->getXn() * _mat * cmspeed);
	if (glfwGetKey(g_wnd->window, GLFW_KEY_Q) == GLFW_PRESS)
		g_Camera->move(-g_Camera->getZn() * _mat * cmspeed);
	if (glfwGetKey(g_wnd->window, GLFW_KEY_E) == GLFW_PRESS)
		g_Camera->move(g_Camera->getZn() * _mat * cmspeed);
	if (view_euler)
	{
		if (glfwGetKey(g_wnd->window, GLFW_KEY_I) == GLFW_PRESS)
			g_Camera->Turn(glCameraDirection::Up, -delta);
		if (glfwGetKey(g_wnd->window, GLFW_KEY_K) == GLFW_PRESS)
			g_Camera->Turn(glCameraDirection::Up, delta);
		if (glfwGetKey(g_wnd->window, GLFW_KEY_J) == GLFW_PRESS)
			g_Camera->Turn(glCameraDirection::Right, -delta);
		if (glfwGetKey(g_wnd->window, GLFW_KEY_L) == GLFW_PRESS)
			g_Camera->Turn(glCameraDirection::Right, delta);
		if (glfwGetKey(g_wnd->window, GLFW_KEY_U) == GLFW_PRESS)
			g_Camera->Turn(glCameraDirection::Front, -delta);
		if (glfwGetKey(g_wnd->window, GLFW_KEY_O) == GLFW_PRESS)
			g_Camera->Turn(glCameraDirection::Front, delta);
	}
	else
	{
		if (glfwGetKey(g_wnd->window, GLFW_KEY_I) == GLFW_PRESS)
		{
			glm::mat4 mat;
			g_Camera->updatevf(glm::mat3(glm::rotate(mat, glm::radians(delta), g_Camera->getXn())) * g_Camera->get(glCameraMember::CameraEyeLight), glCameraMember::CameraEyeLight);
			g_Camera->updatevf(glm::cross(g_Camera->getXn(), g_Camera->get(glCameraMember::CameraEyeLight)), glCameraMember::CameraUpAxis);
		}
		if (glfwGetKey(g_wnd->window, GLFW_KEY_K) == GLFW_PRESS)
		{
			glm::mat4 mat;
			g_Camera->updatevf(glm::mat3(glm::rotate(mat, glm::radians(-delta), g_Camera->getXn())) * g_Camera->get(glCameraMember::CameraEyeLight), glCameraMember::CameraEyeLight);
			g_Camera->updatevf(glm::cross(g_Camera->getXn(), g_Camera->get(glCameraMember::CameraEyeLight)), glCameraMember::CameraUpAxis);
		}
		if (glfwGetKey(g_wnd->window, GLFW_KEY_J) == GLFW_PRESS)
		{
			glm::mat4 mat;
			g_Camera->updatevf(glm::mat3(glm::rotate(mat, glm::radians(delta), g_Camera->getYn())) * g_Camera->get(glCameraMember::CameraEyeLight), glCameraMember::CameraEyeLight);
		}
		if (glfwGetKey(g_wnd->window, GLFW_KEY_L) == GLFW_PRESS)
		{
			glm::mat4 mat;
			g_Camera->updatevf(glm::mat3(glm::rotate(mat, glm::radians(-delta), g_Camera->getYn())) * g_Camera->get(glCameraMember::CameraEyeLight), glCameraMember::CameraEyeLight);
		}
		if (glfwGetKey(g_wnd->window, GLFW_KEY_U) == GLFW_PRESS)
		{
			glm::mat4 mat;
			g_Camera->updatevf(glm::mat3(glm::rotate(mat, glm::radians(delta), g_Camera->getZn())) * g_Camera->get(glCameraMember::CameraUpAxis), glCameraMember::CameraUpAxis);
			g_Camera->updatevf(glm::cross(g_Camera->get(glCameraMember::CameraUpAxis), g_Camera->getXn()), glCameraMember::CameraEyeLight);
		}
		if (glfwGetKey(g_wnd->window, GLFW_KEY_O) == GLFW_PRESS)
		{
			glm::mat4 mat;
			g_Camera->updatevf(glm::mat3(glm::rotate(mat, glm::radians(-delta), g_Camera->getZn())) * g_Camera->get(glCameraMember::CameraUpAxis), glCameraMember::CameraUpAxis);
			g_Camera->updatevf(glm::cross(g_Camera->get(glCameraMember::CameraUpAxis),g_Camera->getXn()), glCameraMember::CameraEyeLight);
		}
	}
	

	if (glfwGetKey(g_wnd->window, GLFW_KEY_F2) == GLFW_PRESS)
		glfwSetInputMode(g_wnd->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwGetKey(g_wnd->window, GLFW_KEY_F3) == GLFW_PRESS)
		glfwSetInputMode(g_wnd->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void camspeed_callback(GLFWwindow* wnd, int key, int scanmode, int action, int mods)
{
	if (animating)
		return;
	if (mods & GLFW_MOD_SHIFT)
	{
		if ((key == GLFW_KEY_LEFT_BRACKET) && (action == GLFW_PRESS))
		{
			g_wnd->sensitivity -= 0.05f;
			if (g_wnd->sensitivity < 0.1f)
				g_wnd->sensitivity = 0.1f;
		}
		if ((key == GLFW_KEY_RIGHT_BRACKET) && (action == GLFW_PRESS))
			g_wnd->sensitivity += 0.05f;
	}
	else
	{
		if ((key == GLFW_KEY_LEFT_BRACKET) && (action == GLFW_PRESS))
			g_Camera->setCmSpeed(-DEFAULT_SPEED_DELTA);
		if ((key == GLFW_KEY_RIGHT_BRACKET) && (action == GLFW_PRESS))
			g_Camera->setCmSpeed(DEFAULT_SPEED_DELTA);
	}
}

void CreateShadowMap(glID fbo, shaderProgram* sp, glModel& mod, std::vector<glPhysics>& cars_phy, glID grdVAO)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, 20480, 10240);
	glClear(GL_DEPTH_BUFFER_BIT);
	//render scene
	//ground
	//sp_grd->use();
	std::string s = "model";
	sp->setUniform(glUniformType::MAT4, (char*)glm::value_ptr(glm::mat4()), sizeof(float) * 16, s);
	glBindVertexArray(grdVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

	
	//std::string s = "model";
	glm::mat4 scale = glm::scale(glm::mat4(),glm::vec3(0.05f,0.05f,0.05f));
	for (auto i = 0; i < 8; i++)
	{
		sp->setUniform(glUniformType::MAT4, (char*)glm::value_ptr(cars_phy[i].newModelMatrix() * scale), sizeof(float) * 16, s);
		mod.Draw(sp);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//restore view port
	glViewport(0, 0, g_wnd->width, g_wnd->height);
}

void v1()
{
	glwindow mwnd(1280, 960, true, 32, frambuffer_size_callback, nullptr, scroll_callback);
	g_wnd = &mwnd;
	glfwSetKeyCallback(g_wnd->window, camspeed_callback);
	glfwSetMouseButtonCallback(g_wnd->window, mouse_click_callback);
	glm::vec3 pos(0.0f, 30.f, 15.0f), eyelight(0, -.5f, -.5f), up(0, .5f, -.5f);
	glCamera cm(pos, eyelight, up);
	g_Camera = &cm;

	//load gl should be done after window created
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return;
		//return false;
	}
	glEnable(GL_MULTISAMPLE);

	//ground
	const float ground[] =
	{
		-60.0f, 0.0f, -40.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 60.0f, 0.0f, -40.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 60.0f, 0.0f,  40.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-60.0f, 0.0f,  40.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
	};
	const int indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	const float skycube[] =
	{
		-100.f, 100.f, 100.f,
		100.f, 100.f, 100.f,
		100.f, -100.f, 100.f,
		-100.f, -100.f, 100.f,
		-100.f, -100.f, -100.f,
		100.f, -100.f, -100.f,
		100.f, 100.f, -100.f,
		-100.f, 100.f, -100.f
	};

	const int cubeindex[] =
	{
		0,1,2,
		0,2,3,
		5,6,7,
		4,5,7,
		5,2,1,
		6,5,1,
		0,4,7,
		0,3,4,
		7,6,1,
		7,1,0,
		2,5,4,
		3,2,4
	};

	//ground
	glID vao[2], vbo[4];
	glGenVertexArrays(2, vao);
	glGenBuffers(4, vbo);
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//cube
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skycube), skycube, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeindex), cubeindex, GL_STATIC_DRAW);


	//depthmap
	glID depthMap, fbo;
	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 20480, 10240, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//bind tex to fbo
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glModel car("./model/bmw_pro/bmw-2002.obj");

	g_model_ptr = &car;

	glm::mat4 scale = glm::scale(glm::vec3(0.05f, 0.05f, 0.05f));
	glm::mat4 scaler = glm::scale(glm::vec3(0.06f, 0.06f, 0.06f));
	std::vector<glPhysics> cars_phy;
	for (auto i = 0; i < 8; i++)
	{
		cars_phy.push_back(car.mp);
		cars_phy[i].bb.max *= 0.05f;
		cars_phy[i].bb.min *= 0.05f;
		/*std::cout << i << ':' << std::endl;
		std::cout << cars_phy[i].bb.max.x << ',' << cars_phy[i].bb.max.y << ',' << cars_phy[i].bb.max.z << std::endl;
		std::cout << cars_phy[i].bb.min.x << ',' << cars_phy[i].bb.min.y << ',' << cars_phy[i].bb.min.z << std::endl;*/
	}
	std::vector<glm::vec4> car_colors;
	{
		srand(glfwGetTimerFrequency());
		cars_phy[0].dirAngle = static_cast<float>(rand() % 90);
		cars_phy[1].dirAngle = static_cast<float>(rand() % 90);
		cars_phy[2].dirAngle = static_cast<float>(rand() % 90);
		cars_phy[3].dirAngle = static_cast<float>(rand() % 90);
		cars_phy[4].dirAngle = static_cast<float>(rand() % 90);
		cars_phy[5].dirAngle = static_cast<float>(rand() % 90);
		cars_phy[6].dirAngle = static_cast<float>(rand() % 90);
		cars_phy[7].dirAngle = static_cast<float>(rand() % 90);

		cars_phy[0].pos = glm::vec3(-10.0f, 0.0f, 30.0f);
		cars_phy[1].pos = glm::vec3(10.0f, 0.0f, 20.0f);
		cars_phy[2].pos = glm::vec3(-10.0f, 0.0f, 20.0f);
		cars_phy[3].pos = glm::vec3(10.0f, 0.0f, -20.0f);
		cars_phy[4].pos = glm::vec3(15.0f, 0.0f, -5.0f);
		cars_phy[5].pos = glm::vec3(15.0f, 0.0f, 5.0f);
		cars_phy[6].pos = glm::vec3(-15.0f, 0.0f, 5.0f);
		cars_phy[7].pos = glm::vec3(-15.0f, 0.0f, -5.0f);

		//car colors
		/*car_colors.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.1f));
		car_colors.push_back(glm::vec4(0.8f, 0.2f, 0.0f, 0.2f));
		car_colors.push_back(glm::vec4(0.6f, 0.4f, 0.0f, 0.3f));
		car_colors.push_back(glm::vec4(0.6f, 0.2f, 0.2f, 0.4f));
		car_colors.push_back(glm::vec4(0.4f, 0.4, 0.2f, 0.5f));
		car_colors.push_back(glm::vec4(0.4f, 0.2f, 0.4f, 0.6f));
		car_colors.push_back(glm::vec4(0.2f, 0.4f, 0.4f, 0.7f));
		car_colors.push_back(glm::vec4(0.0f, 0.6f, 0.4f, 0.8f));*/
	}
	//g_colorV_ptr = &car_colors;

	std::string p1 = ".\\texture";
	glID tex0 = TextureFromFile("1.jpg", p1);
	//glID tex1 = TextureFromFile("2.jpg", p1);
	std::vector< std::string > paths =
	{
		".\\texture\\skybox\\right.jpg",
		".\\texture\\skybox\\left.jpg",
		".\\texture\\skybox\\top.jpg",
		".\\texture\\skybox\\bottom.jpg",
		".\\texture\\skybox\\front.jpg",
		".\\texture\\skybox\\back.jpg"
	};
	glID cubemap = CreateCubeMap(paths);

	p1 = "./shader/vs_shadow.shader";
	std::string p2 = "./shader/fs_shadow.shader";
	shaderProgram sp_withShadow(p1, p2);
	/*shaderProgram sp_ground(p1, p2);
	p1 = "./shader/vs_model.shader";
	p2 = "./shader/fs_model.shader";
	shaderProgram sp_model(p1, p2);*/
	p1 = "./shader/shadowVS.shader";
	p2 = "./shader/shadowFS.shader";
	shaderProgram sp_shadowBuf(p1, p2);
	p1 = "./shader/simplevs.shader";
	p2 = "./shader/simplefs.shader";
	shaderProgram sp_skycube(p1, p2);
	p1 = "vp";
	pos = glm::vec3(50.0f, 5.0f, -40.0f);
	eyelight = glm::vec3(0.f, 0.f, 0.f);
	up = glm::vec3(0.f, 1.f, 0.f);
	sp_shadowBuf.setUniform(glUniformType::MAT4, (char*)glm::value_ptr(glm::ortho(-100.f, 100.f, -100.f, 100.f, .1f, 100.f) * glm::lookAt(pos, eyelight, up)), sizeof(float) * 16, p1);
	p1 = "lt_pv";
	sp_withShadow.setUniform(glUniformType::MAT4, (char*)glm::value_ptr(glm::ortho(-100.f, 100.f, -100.f, 100.f, .1f, 100.f) * glm::lookAt(pos, eyelight, up)), sizeof(float) * 16, p1);
	//p1 = "./shader/simplevs.shader";
	//p2 = "./shader/simplefs.shader";
	//shaderProgram sp_simple(p1, p2);

	//config global shader for callback functions
	//g_sp_ptr = &sp_simple;
	//config uniforms
	p1 = "model";
	sp_shadowBuf.setUniform(glUniformType::MAT4, (char*)glm::value_ptr(glm::mat4()), sizeof(float) * 16, p1);
	p1 = "tx";
	int t = 0;
	sp_withShadow.setUniform(glUniformType::INT, (char*)&t, sizeof(int), p1);
	t++;
	p1 = "shadowMap";
	sp_withShadow.setUniform(glUniformType::INT, (char*)&t, sizeof(int), p1);
	p1 = "lightPos";
	sp_withShadow.setUniform(glUniformType::VEC3F, (char*)glm::value_ptr(pos), sizeof(float) * 3, p1);
	//sp_model.setUniform(glUniformType::VEC3F, (char*)glm::value_ptr(glm::vec3(10000.0f, 10000.0f, 10000.0f)), sizeof(float) * 3, p1);
	p1 = "ambient";
	float ambient = 0.3f;
	sp_withShadow.setUniform(glUniformType::FLOAT, (char*)&ambient, sizeof(float), p1);
	t = 0;
	p1 = "skytex";
	sp_skycube.setUniform(glUniformType::INT, (char*)&t, sizeof(int), p1);


	t = 255;
	glm::mat4 rr;
	BB border;
	border.max = glm::vec3(58.0f, 1000000.0f, 38.0f);
	border.min = glm::vec3(-58.0f, -1000000.0f, -38.0f);
	float store_speed[9] = { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
	ZaxicMathUtil::vec3f v;
	float nyaw, npitch, nroll;
	glm::vec3 npos, neye, nup;
	bool ifF1_press = false, reset = false, ifFirst = false, car_pause = false;
	float spd[8] = {};
	while (!glfwWindowShouldClose(mwnd.window))
	{
		ambient = (sin(glfwGetTime()) + 1.0f) / 2.0f;
		p1 = "rt";
		sp_withShadow.setUniform(glUniformType::FLOAT, (char*)&ambient, sizeof(float), p1);
		if (glfwGetKey(g_wnd->window, GLFW_KEY_SPACE) == GLFW_PRESS)
			ifpause = !ifpause;
		processInput();
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex0);*/

		if ((glfwGetKey(g_wnd->window, GLFW_KEY_F1) == GLFW_PRESS) && (chosenCar != -1) && !animating)
		{
			ifF1_press = !ifF1_press;
			if (ifF1_press)
				ifFirst = true;
			if (!ifF1_press)
				reset = true;
		}

		if ((glfwGetKey(g_wnd->window, GLFW_KEY_SPACE) == GLFW_PRESS) && !animating)
			for (auto i = 0; i < 8; i++)
			{
				store_speed[8] = cars_phy[i].speed;
				cars_phy[i].speed = store_speed[i];
				store_speed[i] = store_speed[8];
			}

		if (ifF1_press && ifFirst)
		{
			//store old cam dir
			up = g_Camera->get(glCameraMember::CameraUpAxis);
			eyelight = g_Camera->get(glCameraMember::CameraEyeLight);
			pos = g_Camera->get(glCameraMember::CameraPosition);
			v = g_Camera->el.v;
			//g_Camera->el.v = ZaxicMathUtil::vec3f();
			g_Camera->ifnorm = false;
			animating = true;
			animateF = 1.0f;
			//calc new angle
			nyaw = -g_Camera->el.yaw / ANIMATE_FRAME_COUNT;
			npitch = -g_Camera->el.pitch / ANIMATE_FRAME_COUNT;
			nroll = -g_Camera->el.roll / ANIMATE_FRAME_COUNT;
			//calc new param
			npos = cars_phy[chosenCar].pos;
			glm::vec3 dir = cars_phy[chosenCar].getDir();
			dir = 5.0f * dir;
			npos.y = 2.0f;
			npos -= dir;
			npos = npos - pos;
			neye = cars_phy[chosenCar].getDir();
			nup = glm::normalize(glm::cross(glm::cross(cars_phy[chosenCar].getDir(), glm::vec3(0.0f, 1.0f, 0.0f)), neye));
			//nup = glm::normalize(glm::cross(dir, neye));
			nup = nup - up;
			neye = neye - eyelight;
			npos = npos / ANIMATE_FRAME_COUNT;
			neye = neye / ANIMATE_FRAME_COUNT;
			nup = nup / ANIMATE_FRAME_COUNT;
			//new pos
			g_Camera->move(npos);
			//new eye
			g_Camera->updatevf(g_Camera->get(glCameraMember::CameraEyeLight) + neye, glCameraMember::CameraEyeLight);
			g_Camera->updatevf(g_Camera->get(glCameraMember::CameraUpAxis) + nup, glCameraMember::CameraUpAxis);
			g_Camera->el.set(EulerMember::yaw, nyaw);
			g_Camera->el.set(EulerMember::pitch, npitch);
			g_Camera->el.set(EulerMember::roll, nroll);
			//set up finish
			//ifF1_press = false;
			ifFirst = false;
			//pause cars
			for (auto i = 0; i < 8; i++)
			{
				store_speed[8] = cars_phy[i].speed;
				cars_phy[i].speed = store_speed[i];
				store_speed[i] = store_speed[8];
			}
		}

		if (reset)
		{
			if (!animating)
			{
				g_Camera->ifnorm = false;
				animateF = 0.0f;
				animating = true;
				nyaw = (v.x - g_Camera->el.yaw) / ANIMATE_FRAME_COUNT;
				npitch = (v.y - g_Camera->el.pitch) / ANIMATE_FRAME_COUNT;
				nroll = (v.z - g_Camera->el.roll) / ANIMATE_FRAME_COUNT;
				npos = pos - g_Camera->get(glCameraMember::CameraPosition);
				neye = eyelight - g_Camera->get(glCameraMember::CameraEyeLight);
				nup = up - g_Camera->get(glCameraMember::CameraUpAxis);
				npos = npos / ANIMATE_FRAME_COUNT;
				neye = neye / ANIMATE_FRAME_COUNT;
				nup = nup / ANIMATE_FRAME_COUNT;
				for (auto i = 0; i < 8; i++)
				{
					store_speed[8] = cars_phy[i].speed;
					cars_phy[i].speed = store_speed[i];
					store_speed[i] = store_speed[8];
				}
			}
			//reset 3rd view
			animateF += 1.0f;
			g_Camera->move(npos);
			g_Camera->updatevf(g_Camera->get(glCameraMember::CameraUpAxis) + nup, glCameraMember::CameraUpAxis);
			g_Camera->updatevf(g_Camera->get(glCameraMember::CameraEyeLight) + neye, glCameraMember::CameraEyeLight);
			g_Camera->el.set(EulerMember::yaw, nyaw);
			g_Camera->el.set(EulerMember::pitch, npitch);
			g_Camera->el.set(EulerMember::roll, nroll);
			//g_Camera->el.v = v;
			if (ZAXIC_FLOAT_SAME(animateF, ANIMATE_FRAME_COUNT))
			{
				animating = false;
				reset = false;
				g_Camera->ifnorm = true;
				for (auto i = 0; i < 8; i++)
				{
					store_speed[8] = cars_phy[i].speed;
					cars_phy[i].speed = store_speed[i];
					store_speed[i] = store_speed[8];
				}
			}
		}

		if (ifF1_press)
		{
			if (!animating)
				g_Camera->updatevf(cars_phy[chosenCar].pos - cars_phy[chosenCar].getDir() * 5.0f + glm::vec3(0.0f, 2.0f, 0.0f), glCameraMember::CameraPosition);
			else
				if (ZAXIC_FLOAT_SAME(animateF, ANIMATE_FRAME_COUNT))
				{
					animating = false;
					g_Camera->ifnorm = true;
					//continue moving
					for (auto i = 0; i < 8; i++)
					{
						store_speed[8] = cars_phy[i].speed;
						cars_phy[i].speed = store_speed[i];
						store_speed[i] = store_speed[8];
					}
				}
				else
				{
					glm::vec3 _npos;
					//new pos
					g_Camera->move(npos);
					//new eye
					g_Camera->updatevf(g_Camera->get(glCameraMember::CameraEyeLight) + neye, glCameraMember::CameraEyeLight);
					//new up
					g_Camera->updatevf(g_Camera->get(glCameraMember::CameraUpAxis) + nup, glCameraMember::CameraUpAxis);
					g_Camera->el.set(EulerMember::yaw, nyaw);
					g_Camera->el.set(EulerMember::pitch, npitch);
					g_Camera->el.set(EulerMember::roll, nroll);
					animateF += 1.0f;
				}
		}

		rr = g_Camera->projMatrix(g_wnd->ratio()) * transZaxicMatToGLM(g_Camera->el.EulerMatrix()) * g_Camera->viewMatrix();
		p1 = "model_pv";
		sp_withShadow.setUniform(glUniformType::MAT4, (char*)glm::value_ptr(rr), sizeof(float) * 16, p1);
		sp_skycube.setUniform(glUniformType::MAT4, (char*)glm::value_ptr(rr), sizeof(float) * 16, p1);
		//sp_model.setUniform(glUniformType::MAT4, (char*)glm::value_ptr(rr), sizeof(float) * 16, p1);
		p1 = "CamPos";
		sp_withShadow.setUniform(glUniformType::VEC3F, (char*)glm::value_ptr(g_Camera->get(glCameraMember::CameraPosition)), sizeof(float) * 3, p1);
		//sp_model.setUniform(glUniformType::VEC3F, (char*)glm::value_ptr(g_Camera->get(glCameraMember::CameraPosition)), sizeof(float) * 3, p1);
		p1 = "model";
		sp_withShadow.setUniform(glUniformType::MAT4, (char*)glm::value_ptr(glm::mat4()), sizeof(float) * 16, p1);
		CreateShadowMap(fbo, &sp_shadowBuf, car, cars_phy, vao[0]);

		//draw ground
		p1 = "chosen";
		int cs = 0;
		sp_withShadow.setUniform(glUniformType::INT, (char*)&cs, sizeof(int), p1);
		glBindVertexArray(vao[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
		//sp_ground.use();
		//glBindVertexArray(vao);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

		//draw cars
		//glBindTexture(GL_TEXTURE_2D, tex1);
		for (auto i = 0; i < 8; i++)
			cars_phy[i].CheckBorder(border);


		//prepare shadow maps


		//draw chosen car
		if (chosenCar != -1)
		{
			/*p1 = "chosen";
			cs = 1;
			sp_withShadow.setUniform(glUniformType::INT, (char*)&cs, sizeof(int), p1);
			p1 = "code";
			sp_withShadow.setUniform(glUniformType::INT, (char*)&chosenCar, sizeof(int), p1);*/
			p1 = "fc";
			sp_withShadow.setUniform(glUniformType::VEC4F, (char*)glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.0f, float(chosenCar) / 255.f)), sizeof(float) * 4, p1);
			/*p1 = "model";
			sp_model.setUniform(glUniformType::MAT4, (char*)glm::value_ptr(cars_phy[chosenCar].newModelMatrix() * scaler), sizeof(float) * 16, p1);*/
			//glCullFace(GL_FRONT);
			//car.Draw(&sp_withShadow);
			//glDisable(GL_DEPTH_TEST);
			//glCullFace(GL_BACK);


			if (glfwGetKey(g_wnd->window, GLFW_KEY_UP) == GLFW_PRESS)
				cars_phy[chosenCar].Turn(Directions::FORWARD);
			if (glfwGetKey(g_wnd->window, GLFW_KEY_DOWN) == GLFW_PRESS)
				cars_phy[chosenCar].Turn(Directions::BACKWARD);
			if (glfwGetKey(g_wnd->window, GLFW_KEY_LEFT) == GLFW_PRESS)
				cars_phy[chosenCar].Turn(Directions::LEFT);
			if (glfwGetKey(g_wnd->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				cars_phy[chosenCar].Turn(Directions::RIGHT);
		}

		//draw cars normal
		for (auto i = 0; i < 8; i++)
		{
			for (auto j = i + 1; j < 8; j++)
				if (cars_phy[i].Collision_Single(cars_phy[j].updateBB()))
					cars_phy[i].Momentum(cars_phy[j]);
			if (!ifpause)
				cars_phy[i].Move(g_wnd->_tm.getdelta());
			p1 = "chosen";
			if (chosenCar == i)
			{
				cs = 1;
				sp_withShadow.setUniform(glUniformType::INT, (char*)&cs, sizeof(int), p1);
			}
			else
			{
				cs = 0;
				sp_withShadow.setUniform(glUniformType::INT, (char*)&cs, sizeof(int), p1);
			}

			p1 = "code";
			sp_withShadow.setUniform(glUniformType::INT, (char*)&i, sizeof(int), p1);
			/*p1 = "fc";
			sp_model.setUniform(glUniformType::VEC4F, (char*)glm::value_ptr(car_colors[i]), sizeof(float) * 4, p1);*/
			p1 = "model";
			sp_withShadow.setUniform(glUniformType::MAT4, (char*)glm::value_ptr(cars_phy[i].newModelMatrix() * scale), sizeof(float) * 16, p1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			car.Draw(&sp_withShadow);
		}

		//draw sky cube
		sp_skycube.use();
		glDepthFunc(GL_LEQUAL);
		glBindVertexArray(vao[1]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);



		glfwPollEvents();
		glfwSwapBuffers(mwnd.window);
	}

	glDeleteBuffers(2, vao);
	glDeleteBuffers(3, vbo);
	glfwTerminate();
}

int main()
{
	//open debug log
	mlog l;

	//mathtest_vec3f();
	//matTest();
	//eulquatTest();
	
	//v1();

	Example app;
	app.Run();

	//glfwTerminate();
	return 0;
}


