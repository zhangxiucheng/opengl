#include "ZaxicEngine.h"
bool Example::m_signal[8];

void OnResize(GLFWwindow* wnd, int width, int height)
{
    //reset viewport
    glViewport(0, 0, width, height);

    //glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.01f, 100.0f);
    auto mwnd = last->GetWindow();
    mwnd->width = width;
    mwnd->height = height;
    auto shader = last->GetShaderByName("Character");
    shader->setUniform(glUniformType::MAT4, (char*)glm::value_ptr(glm::ortho(0.0f, float(mwnd->width), 0.0f, float(mwnd->height))), sizeof(float) * 16, "projection");
    //update projection matrix
}

//void OnMouseClick(GLFWwindow* window, int button, int action, int targ)
//{
//    
//    unsigned char buffer;
//    unsigned tg;
//    if (last->m_signal[2])
//        return;
//    if (action == GLFW_PRESS)
//    {
//        double posx, posy;
//        GLint view[4];
//        glGetIntegerv(GL_VIEWPORT, view);
//        glfwGetCursorPos(window, &posx, &posy);
//        //std::cout << posx << ',' << posy << std::endl;
//        switch (button)
//        {
//        case GLFW_MOUSE_BUTTON_LEFT:
//            //process left button click
//            glReadPixels(posx, static_cast<double>(view[3]) - posy, 1, 1, GL_ALPHA, GL_UNSIGNED_BYTE, &buffer);
//            tg = int(buffer);
//            tg = 255 - tg;
//            last->SetChosenCar(tg);
//            //std::cout << tg << std::endl;
//            break;
//        case GLFW_MOUSE_BUTTON_RIGHT:
//            break;
//        default:
//            break;
//        }
//    }
//}

bool ZaxicEngine::EngineInit()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!" << std::endl;
        return false;
    }
    glEnable(GL_MULTISAMPLE);
    /*LONG_PTR res = SetWindowLongPtr(m_hwnd, GWLP_WNDPROC, (LONG_PTR)func);
    if (!res)
        ProcessWINError(m_hwnd);
    res = SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);*/
    return true;
}

void ZaxicEngine::AnimatorGo()
{
    for (auto i = m_animators.begin(); i != m_animators.end();)
        if ((*i)->Go())
            i++;
        else
            m_animators.erase(i);
}

void ZaxicEngine::glCreateWindow(uint32_t width, uint32_t height, bool ifSMAA, int SMAAlevel)
{
    if (m_window!=nullptr)
        return;
    m_window = std::make_shared<glwindow>(width, height, ifSMAA, SMAAlevel);
}

void ZaxicEngine::glCreateCamera(vec3f campos, vec3f eyelight, vec3f up)
{
    if (m_camera!=nullptr)
        return;
    glm::vec3 pos = transZaxicVecToGLM(campos), eye = transZaxicVecToGLM(eyelight), _up = transZaxicVecToGLM(up);
    m_camera = std::make_shared<glCamera>(pos, eye, _up);
}

void ZaxicEngine::CreateAnimator(AnimatorType type, void* targ, void* end, InterpolationType count)
{
    auto anime = std::make_shared<glAnimator>(type, targ, end, count);
    m_animators.push_back(anime);
}

void ZaxicEngine::LoadModel(std::string name, std::string path)
{
    auto model = std::make_shared<glModel>(path);
    m_models.insert(mModel(name, model));
}

void ZaxicEngine::Loadshader(std::string name, std::string vspath, std::string fspath)
{
    auto sp = std::make_shared<shaderProgram>(vspath, fspath);
    m_shaders.insert(mProgram(name, sp));
}

shaderProgram* ZaxicEngine::GetShaderByName(std::string name)
{
    auto res = m_shaders.find(name);
    if (res != m_shaders.end())
        return (res->second).get();
    else
        return nullptr;
}

glModel* ZaxicEngine::GetResourceByName(std::string name)
{
    auto res = m_models.find(name);
    if (res != m_models.end())
        return (res->second).get();
    else
        return nullptr;
}

glID ZaxicEngine::GetTextureByName(std::string name)
{
    auto iter = m_textures.find(name);
    if (iter != m_textures.end())
        return iter->second;
    return 0;
}

glID ZaxicEngine::GetVAOByName(std::string name)
{
    auto iter = m_VAOs.find(name);
    if (iter != m_VAOs.end())
        return iter->second;
    return 0;
}

glID ZaxicEngine::GetVBOByName(std::string name)
{
    auto iter = m_VBOs.find(name);
    if (iter != m_VBOs.end())
        return iter->second;
    return 0;
}

glID ZaxicEngine::GetFBOByName(std::string name)
{
    auto iter = m_FBOs.find(name);
    if (iter != m_FBOs.end())
        return iter->second;
    return 0;
}

glwindow* ZaxicEngine::GetWindow()
{
    return m_window.get();
}

glCamera* ZaxicEngine::GetCamera()
{
    return m_camera.get();
}

void ProcessWINError(HWND hwnd)
{
    DWORD res = GetLastError();
    HLOCAL hlocal = NULL;
    DWORD syslocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
    BOOL fres = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        NULL,
        res,
        syslocale,
        (PTSTR)&hlocal,
        0,
        NULL);

    MessageBox(hwnd, (PCTSTR)hlocal, L"Win32 Error", MB_OK);
}

//void Example::CreateGeo(ZaxicGeoType type, std::string name, uint32_t ringCount, uint32_t layerCount, float topX, float topY, float topZ, float R1, bool ifDiff, float R2)
//{
//    std::vector<mVertex> mpoints;
//    switch (type)
//    {
//    case ZaxicGeoType::Cylinder:
//        break;
//    case ZaxicGeoType::Sphere:
//        
//        break;
//    case ZaxicGeoType::Cone:
//        break;
//    default:
//        break;
//    }
//}

void Example::OnRender()
{
    //update local data
    Update();
    //flush local buffer
    glEnable(GL_DEPTH_TEST);
    //glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    //Create new shadow map
    glID fbo = GetFBOByName("shadowFBO");
    glID depthMap = GetTextureByName("shadowFBO_depthMap");
    glID ground = GetVAOByName("groundVAO");
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, 20480, 10240);
    glClear(GL_DEPTH_BUFFER_BIT);
    auto sp_framebuf = GetShaderByName("FrameBuffer");
    //set model
    sp_framebuf->setUniform(glUniformType::MAT4, (char*)glm::value_ptr(glm::mat4()), sizeof(float) * 16, "model");
    //ground
    glBindVertexArray(ground);
    {
        auto loli = GetTextureByName("texture_loli8");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, loli);
    }
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //cars
    static glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(.05f, .05f, .05f));
    auto mod = GetResourceByName("car");
    for (auto i = 0; i < 8; i++)
    {
        sp_framebuf->setUniform(glUniformType::MAT4, (char*)glm::value_ptr(m_phys[i].newModelMatrix() * scale), 16 * sizeof(float), "model");
        mod->Draw(sp_framebuf);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //reset viewport
    glViewport(0, 0, m_window->width, m_window->height);

    //usual draw
    auto sp_shadow = GetShaderByName("Shadow");
    auto sp_skycube = GetShaderByName("SkyCube");
    assert(sp_shadow != nullptr && "error to find shadow shader!");
    assert(sp_skycube != nullptr && "error to find skycube shader!");
    glm::mat4 pv = m_camera->projMatrix(m_window->ratio()) * transZaxicMatToGLM(m_camera->el.EulerMatrix()) * m_camera->viewMatrix();
    sp_shadow->setUniform(glUniformType::MAT4, (char*)glm::value_ptr(pv), sizeof(float) * 16, "model_pv");
    sp_skycube->setUniform(glUniformType::MAT4, (char*)glm::value_ptr(pv), sizeof(float) * 16, "model_pv");
    sp_shadow->setUniform(glUniformType::VEC3F, (char*)glm::value_ptr(m_camera->get(glCameraMember::CameraPosition)), sizeof(float) * 3, "CamPos");
    sp_shadow->setUniform(glUniformType::MAT4, (char*)glm::value_ptr(glm::mat4()), sizeof(float) * 16, "model");
    //draw ground
    int cs = 0;
    sp_shadow->setUniform(glUniformType::INT, (char*)&cs, sizeof(int), "chosen");
    cs = 255;
    sp_shadow->setUniform(glUniformType::INT, (char*)&cs, sizeof(int), "code");
    glBindVertexArray(ground);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //auto loli = GetTextureByName("texture_loli");
    for (auto i = 0; i < 8; i++)
    {
        auto loli = GetTextureByName("texture_loli" + std::to_string(i));
        //model movements should be moved into Update()
        /*for (auto j = i + 1; j < 8; j++)
            if (m_phys[i].Collision_Single(cars_phy[j].updateBB()))
                cars_phy[i].Momentum(cars_phy[j]);
        if (!ifpause)
            cars_phy[i].Move(g_wnd->_tm.getdelta());*/
        if (m_player[0] == i)
        {
            cs = 1;
            sp_shadow->setUniform(glUniformType::INT, (char*)&cs, sizeof(int), "chosen");
            sp_shadow->setUniform(glUniformType::VEC4F, (char*)glm::value_ptr(glm::vec4(1.0f, 0.f, (glm::sin(glm::radians(glfwGetTime())) + 1.f) / 2.f, 1.f)), sizeof(float) * 4, "fc");
        }
        else if (m_player[1] == i)
        {
            cs = 1;
            sp_shadow->setUniform(glUniformType::INT, (char*)&cs, sizeof(int), "chosen");
            sp_shadow->setUniform(glUniformType::VEC4F, (char*)glm::value_ptr(glm::vec4(0.f, 1.0f, (glm::cos(glm::radians(glfwGetTime())) + 1.f) / 2.f, 1.f)), sizeof(float) * 4, "fc");
        }
        else
        {
            cs = 0;
            sp_shadow->setUniform(glUniformType::INT, (char*)&cs, sizeof(int), "chosen");
        }
        /*cs = 255 - i;
        sp_shadow->setUniform(glUniformType::INT, (char*)&cs, sizeof(int), "code");*/
        /*p1 = "fc";
        sp_model.setUniform(glUniformType::VEC4F, (char*)glm::value_ptr(car_colors[i]), sizeof(float) * 4, p1);*/
        sp_shadow->setUniform(glUniformType::MAT4, (char*)glm::value_ptr(m_phys[i].newModelMatrix() * scale), sizeof(float) * 16, "model");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, loli);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        mod->Draw(sp_shadow);
    }

    sp_skycube->use();
    auto cube = GetVAOByName("skycubeVAO");
    auto cubetex = GetTextureByName("texture_cube");
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(cube);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubetex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

    RenderText("player1: " + std::to_string(scores[0]), float(m_window->width - WNDOFFSET), float(m_window->height - WNDOFFSET), 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
    RenderText("player2: " + std::to_string(scores[1]), float(m_window->width - WNDOFFSET), float(m_window->height - (WNDOFFSET + 100.f)), 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
}

void Example::Update()
{
    AnimatorGo();
    if (m_animators.size() > 0)
        m_signal[2] = true;
    else
        m_signal[2] = false;
    auto sp_shadow = GetShaderByName("Shadow");
    auto sp_map = GetShaderByName("FrameBuffer");
    lit_pos = glm::vec3(glm::cos(glfwGetTime()/20.f) * 50.f, 3.f, glm::sin(glfwGetTime()/20.f) * 50.f);
    sp_shadow->setUniform(glUniformType::VEC3F, (char*)glm::value_ptr(lit_pos), sizeof(lit_pos), "lightPos");
    sp_shadow->setUniform(glUniformType::MAT4, (char*)glm::value_ptr(glm::ortho(-1000.f, 1000.f, -1000.f, 1000.f, .1f, 200.f) * glm::lookAt(lit_pos, lit_eyelight, lit_up)), sizeof(float) * 16, "lt_pv");
    sp_map->setUniform(glUniformType::MAT4, (char*)glm::value_ptr(glm::ortho(-1000.f, 1000.f, -1000.f, 1000.f, .1f, 200.f) * glm::lookAt(lit_pos, lit_eyelight, lit_up)), sizeof(float) * 16, "vp");
    float rt = (sin(glfwGetTime()) + 1.f) / 2.f;
    sp_shadow->setUniform(glUniformType::FLOAT, (char*)&rt, sizeof(float), "rt");
    //m_window->_tm.tick();
    for (auto i = 0; i < 8; i++)
        m_phys[i].CheckBorder(m_border);
    for (auto i = 0; i < m_phys.size(); i++)
    {
        if (m_phys[i].pause)
            continue;
        for (auto j = i + 1; j < 8; j++)
            if (m_phys[i].Collision_Single(m_phys[j].updateBB()))
            {
                if (i == m_player[0] || j == m_player[0])
                    scores[0]++;
                if (i == m_player[1] || j == m_player[1])
                    scores[1]++;
                m_phys[i].Momentum(m_phys[j]);
            }
        if (!m_signal[1])
            m_phys[i].Move(m_window->_tm.getdelta());
    }
}

void Example::processInput()
{
    m_window->_tm.tick();
    /*if (animating)
        return;*/
    bool ifangel = false;
    if (glfwGetKey(m_window->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->window, true);


    float cmspeed = m_camera->cmspeed() * m_window->_tm.getdelta();
    float delta = m_window->sensitivity * 3.0f;
    glm::mat4 mat = transZaxicMatToGLM(m_camera->el.EulerMatrix());
    glm::mat3 _mat = glm::mat3(mat);

    if (glfwGetKey(m_window->window, GLFW_KEY_8) == GLFW_PRESS)
        m_signal[0] = true;
    if (glfwGetKey(m_window->window, GLFW_KEY_9) == GLFW_PRESS)
        m_signal[0] = false;

    //set player1
    
    auto func_pl0 = [wnd = m_window->window,pl = m_player](int key) {
        if (glfwGetKey(wnd, key + GLFW_KEY_0) == GLFW_PRESS)
            if (pl[1] != key && pl[0] == -1)
                pl[0] = key;
    };
    if (m_player[0] < 0)
        for (auto i = 0; i < 8; i++)
            func_pl0(i);
    else
    {
        if (glfwGetKey(m_window->window, GLFW_KEY_UP) == GLFW_PRESS)
            m_phys[m_player[0]].Turn(Directions::FORWARD);
        if (glfwGetKey(m_window->window, GLFW_KEY_DOWN) == GLFW_PRESS)
            m_phys[m_player[0]].Turn(Directions::BACKWARD);
        if (glfwGetKey(m_window->window, GLFW_KEY_LEFT) == GLFW_PRESS)
            m_phys[m_player[0]].Turn(Directions::LEFT);
        if (glfwGetKey(m_window->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            m_phys[m_player[0]].Turn(Directions::RIGHT);
    }

    //set player2
    auto func_pl1 = [wnd = m_window->window, pl = m_player](int key) {
        if (glfwGetKey(wnd, GLFW_KEY_F1) == GLFW_PRESS && glfwGetKey(wnd, key + GLFW_KEY_0) == GLFW_PRESS)
            if (pl[0] != key && pl[1] == -1)
                pl[1] = key;
    };
    if (m_player[1] < 0)
        for (auto i = 0; i < 8; i++)
            func_pl1(i);
    else
    {
        if (glfwGetKey(m_window->window, GLFW_KEY_W) == GLFW_PRESS)
            m_phys[m_player[1]].Turn(Directions::FORWARD);
        if (glfwGetKey(m_window->window, GLFW_KEY_S) == GLFW_PRESS)
            m_phys[m_player[1]].Turn(Directions::BACKWARD);
        if (glfwGetKey(m_window->window, GLFW_KEY_A) == GLFW_PRESS)
            m_phys[m_player[1]].Turn(Directions::LEFT);
        if (glfwGetKey(m_window->window, GLFW_KEY_D) == GLFW_PRESS)
            m_phys[m_player[1]].Turn(Directions::RIGHT);
    }

    if (glfwGetKey(m_window->window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        for (auto& i : m_phys)
            i.pause = !i.pause;
    }

    //view control
    //cancel view control
    if (glfwGetKey(m_window->window, GLFW_KEY_M) == GLFW_PRESS)
        m_camera->move(m_camera->getZn() * _mat * cmspeed);
    if (glfwGetKey(m_window->window, GLFW_KEY_N) == GLFW_PRESS)
        m_camera->move(-m_camera->getZn() * _mat * cmspeed);
    /*if (glfwGetKey(m_window->window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera->move(-m_camera->getXn() * _mat * cmspeed);
    if (glfwGetKey(m_window->window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera->move(m_camera->getXn() * _mat * cmspeed);
    if (glfwGetKey(m_window->window, GLFW_KEY_Q) == GLFW_PRESS)
        m_camera->move(-m_camera->getZn() * _mat * cmspeed);
    if (glfwGetKey(m_window->window, GLFW_KEY_E) == GLFW_PRESS)
        m_camera->move(m_camera->getZn() * _mat * cmspeed);*/
    if (m_signal[0])
    {
        if (glfwGetKey(m_window->window, GLFW_KEY_I) == GLFW_PRESS)
            m_camera->Turn(glCameraDirection::Up, -delta);
        if (glfwGetKey(m_window->window, GLFW_KEY_K) == GLFW_PRESS)
            m_camera->Turn(glCameraDirection::Up, delta);
        if (glfwGetKey(m_window->window, GLFW_KEY_J) == GLFW_PRESS)
            m_camera->Turn(glCameraDirection::Right, -delta);
        if (glfwGetKey(m_window->window, GLFW_KEY_L) == GLFW_PRESS)
            m_camera->Turn(glCameraDirection::Right, delta);
        if (glfwGetKey(m_window->window, GLFW_KEY_U) == GLFW_PRESS)
            m_camera->Turn(glCameraDirection::Front, -delta);
        if (glfwGetKey(m_window->window, GLFW_KEY_O) == GLFW_PRESS)
            m_camera->Turn(glCameraDirection::Front, delta);
    }
    else
    {
        if (glfwGetKey(m_window->window, GLFW_KEY_I) == GLFW_PRESS)
        {
            glm::mat4 mat;
            m_camera->updatevf(glm::mat3(glm::rotate(mat, glm::radians(delta), m_camera->getXn())) * m_camera->get(glCameraMember::CameraEyeLight), glCameraMember::CameraEyeLight);
            m_camera->updatevf(glm::cross(m_camera->getXn(), m_camera->get(glCameraMember::CameraEyeLight)), glCameraMember::CameraUpAxis);
        }
        if (glfwGetKey(m_window->window, GLFW_KEY_K) == GLFW_PRESS)
        {
            glm::mat4 mat;
            m_camera->updatevf(glm::mat3(glm::rotate(mat, glm::radians(-delta), m_camera->getXn())) * m_camera->get(glCameraMember::CameraEyeLight), glCameraMember::CameraEyeLight);
            m_camera->updatevf(glm::cross(m_camera->getXn(), m_camera->get(glCameraMember::CameraEyeLight)), glCameraMember::CameraUpAxis);
        }
        if (glfwGetKey(m_window->window, GLFW_KEY_J) == GLFW_PRESS)
        {
            glm::mat4 mat;
            m_camera->updatevf(glm::mat3(glm::rotate(mat, glm::radians(delta), m_camera->getYn())) * m_camera->get(glCameraMember::CameraEyeLight), glCameraMember::CameraEyeLight);
        }
        if (glfwGetKey(m_window->window, GLFW_KEY_L) == GLFW_PRESS)
        {
            glm::mat4 mat;
            m_camera->updatevf(glm::mat3(glm::rotate(mat, glm::radians(-delta), m_camera->getYn())) * m_camera->get(glCameraMember::CameraEyeLight), glCameraMember::CameraEyeLight);
        }
        if (glfwGetKey(m_window->window, GLFW_KEY_U) == GLFW_PRESS)
        {
            glm::mat4 mat;
            m_camera->updatevf(glm::mat3(glm::rotate(mat, glm::radians(delta), m_camera->getZn())) * m_camera->get(glCameraMember::CameraUpAxis), glCameraMember::CameraUpAxis);
            m_camera->updatevf(glm::cross(m_camera->get(glCameraMember::CameraUpAxis), m_camera->getXn()), glCameraMember::CameraEyeLight);
        }
        if (glfwGetKey(m_window->window, GLFW_KEY_O) == GLFW_PRESS)
        {
            glm::mat4 mat;
            m_camera->updatevf(glm::mat3(glm::rotate(mat, glm::radians(-delta), m_camera->getZn())) * m_camera->get(glCameraMember::CameraUpAxis), glCameraMember::CameraUpAxis);
            m_camera->updatevf(glm::cross(m_camera->get(glCameraMember::CameraUpAxis), m_camera->getXn()), glCameraMember::CameraEyeLight);
        }
    }


    if (glfwGetKey(m_window->window, GLFW_KEY_F2) == GLFW_PRESS)
        glfwSetInputMode(m_window->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwGetKey(m_window->window, GLFW_KEY_F3) == GLFW_PRESS)
        glfwSetInputMode(m_window->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    //view changing
    
}

bool Example::EngineInit()
{
    memset(m_signal, 0, sizeof(m_signal));
    glCreateWindow(1280, 960, true, 16);
    m_window->setWndCallBack(WndCallBackType::FrameBufferResize, OnResize);
    //m_window->setWndCallBack(WndCallBackType::CursorClick, OnMouseClick);
    glfwMakeContextCurrent(m_window->window);
    bool res = ZaxicEngine::EngineInit();
    if (res)
    {
        CharacterInit();

        m_border.max = glm::vec3(58.0f, 1000000.0f, 38.0f);
        m_border.min = glm::vec3(-58.0f, -1000000.0f, -38.0f);
        glCreateCamera(vec3f(0.f, 160.f, 0.f), vec3f(0.f,-1.f,0.f), vec3f(0.f,0.f,-1.f));
        LoadModel("car", "./model/bmw_pro/bmw-2002.obj");
        glModel* mcar = GetResourceByName("car");
        srand(::GetTickCount64());
        for (auto i = 0; i < 8; i++)
        {
            m_phys.push_back(mcar->mp);
            m_phys[i].dirAngle = glm::radians(static_cast<float>(rand()) / 2.3f);
            m_phys[i].bb.max *= 0.055f;
            m_phys[i].bb.min *= 0.055f;
        }
        m_phys[0].pos = glm::vec3(-20.0f, 0.0f, 30.0f);
        m_phys[1].pos = glm::vec3(10.0f, 0.0f, 15.0f);
        m_phys[2].pos = glm::vec3(-10.0f, 0.0f, 20.0f);
        m_phys[3].pos = glm::vec3(10.0f, 0.0f, -20.0f);
        m_phys[4].pos = glm::vec3(25.0f, 0.0f, -5.0f);
        m_phys[5].pos = glm::vec3(15.0f, 0.0f, -10.0f);
        m_phys[6].pos = glm::vec3(-35.0f, 0.0f, 5.0f);
        m_phys[7].pos = glm::vec3(-15.0f, 0.0f, -5.0f);


        Loadshader("Shadow", ".\\shader\\vs_shadow.shader", ".\\shader\\fs_shadow.shader");
        Loadshader("FrameBuffer", ".\\shader\\shadowVS.shader", ".\\shader\\shadowFS.shader");
        Loadshader("SkyCube", ".\\shader\\simplevs.shader", ".\\shader\\simplefs.shader");
        //load textures
        for (auto i = 1; i < 9; i++)
        {
            auto id = TextureFromFile(std::to_string(i) + ".jpg", ".\\texture");
            m_textures.insert(mID("texture_loli" + std::to_string(i-1), id));
        }
        //ground texture
        {
            auto id = TextureFromFile("1.png", ".\\texture");
            m_textures.insert(mID("texture_loli8", id));
        }


        float ground[] =
        {
            -60.0f, 0.0f, -40.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
             60.0f, 0.0f, -40.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
             60.0f, 0.0f,  40.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -60.0f, 0.0f,  40.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
        };
        int indices[] =
        {
            0, 1, 2,
            0, 2, 3
        };
        float skycube[] =
        {
            -200.f, 200.f, 200.f,
            200.f, 200.f, 200.f,
            200.f, -200.f, 200.f,
            -200.f, -200.f, 200.f,
            -200.f, -200.f, -200.f,
            200.f, -200.f, -200.f,
            200.f, 200.f, -200.f,
            -200.f, 200.f, -200.f
        };

        int cubeindex[] =
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

        std::string name = "ground";
        glID vao[2], vbo[4];
        glGenVertexArrays(2, vao);
        glGenBuffers(4, vbo);
        glBindVertexArray(vao[0]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ground), static_cast<void*>(ground), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), static_cast<void*>(indices), GL_STATIC_DRAW);
        //store resource
        m_VAOs.insert(mID(name + "VAO", vao[0]));
        m_VBOs.insert(mID(name + "VBO_vertices", vbo[0]));
        m_VBOs.insert(mID(name + "VBO_indices", vbo[1]));

        name = "skycube";
        glBindVertexArray(vao[1]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skycube), static_cast<void*>(skycube), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeindex), cubeindex, GL_STATIC_DRAW);

        m_VAOs.insert(mID(name + "VAO", vao[1]));
        m_VBOs.insert(mID(name + "VBO_vertices", vbo[2]));
        m_VBOs.insert(mID(name + "VBO_indices", vbo[3]));

        //depth map
        glID fbo, depthMap;
        name = "shadow";
        glGenFramebuffers(1, &fbo);
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 20480, 10240, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_FBOs.insert(mID(name + "FBO", fbo));
        m_textures.insert(mID(name + "FBO_depthMap", depthMap));

        //cubeTexture
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
        m_textures.insert(mID("texture_cube", cubemap));

        //set shader uniforms
        name = "vp";
        lit_pos = glm::vec3(50.0f, 2.0f, 0.f);
        lit_eyelight = glm::vec3(0.f, 0.f, 0.f);
        lit_up = glm::vec3(0.f, 1.f, 0.f);
        shaderProgram* sp_shadowBuf = GetShaderByName("FrameBuffer");
        shaderProgram* sp_withShadow = GetShaderByName("Shadow");
        shaderProgram* sp_skycube = GetShaderByName("SkyCube");
        sp_shadowBuf->setUniform(glUniformType::MAT4, (char*)glm::value_ptr(glm::ortho(-1000.f, 1000.f, -1000.f, 1000.f, .1f, 200.f) * glm::lookAt(lit_pos, lit_eyelight, lit_up)), sizeof(float) * 16, name);
        name = "lt_pv";
        sp_withShadow->setUniform(glUniformType::MAT4, (char*)glm::value_ptr(glm::ortho(-1000.f, 1000.f, -1000.f, 1000.f, .1f, 200.f) * glm::lookAt(lit_pos, lit_eyelight, lit_up)), sizeof(float) * 16, name);
        //p1 = "./shader/simplevs.shader";
        //p2 = "./shader/simplefs.shader";
        //shaderProgram sp_simple(p1, p2);

        //config global shader for callback functions
        //g_sp_ptr = &sp_simple;
        //config uniforms
        name = "model";
        sp_shadowBuf->setUniform(glUniformType::MAT4, (char*)glm::value_ptr(glm::mat4()), sizeof(float) * 16, name);
        name = "tx";
        int t = 0;
        sp_withShadow->setUniform(glUniformType::INT, (char*)&t, sizeof(int), name);
        t++;
        name = "shadowMap";
        sp_withShadow->setUniform(glUniformType::INT, (char*)&t, sizeof(int), name);
        name = "lightPos";
        sp_withShadow->setUniform(glUniformType::VEC3F, (char*)glm::value_ptr(lit_pos), sizeof(float) * 3, name);
        //sp_model.setUniform(glUniformType::VEC3F, (char*)glm::value_ptr(glm::vec3(10000.0f, 10000.0f, 10000.0f)), sizeof(float) * 3, p1);
        name = "ambient";
        float ambient = 0.3f;
        sp_withShadow->setUniform(glUniformType::FLOAT, (char*)&ambient, sizeof(float), name);
        t = 0;
        name = "skytex";
        sp_skycube->setUniform(glUniformType::INT, (char*)&t, sizeof(int), name);
    }
    else
        glfwDestroyWindow(m_window->window);
    return res;
}

void Example::Run()
{
    if (EngineInit())
    {
        loadResource();
        if (last)
        {
            while (last)
                Sleep(100);
            last = this;
        }
        else
            last = this;
        while (!glfwWindowShouldClose(m_window->window))
        {
            //process user input
            processInput();
            //update local resource
            Update();
            //render
            OnRender();

            glfwPollEvents();
            glfwSwapBuffers(m_window->window);
        }
        glfwTerminate();
        last = nullptr;
    }
}

//void Example::SetChosenCar(int tg)
//{
//    if (tg > -1 && tg < m_phys.size())
//        m_chosenCar = tg;
//    else
//        return;
//}

void Example::CharacterInit()
{
    //Init freetype lib
    FT_Library lib;
    if (FT_Init_FreeType(&lib))
    {
        std::string log = "ERROR::FREETYPE: Could not init FreeType Library";
        g_log->newlog(log);
        m_signal[5] = false;
        return;
    }
    FT_Face face;
    if (FT_New_Face(lib, "fonts/arial.ttf", 0, &face))
    {
        std::string log = "ERROR::FREETYPE: Failed to load font";
        g_log->newlog(log);
        m_signal[5] = false;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用字节对齐限制
    //load 128 ASC2 code
    for (GLubyte c = 0; c < 128; c++)
    {
        // 加载字符的字形 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::string log = "ERROR::FREETYTPE: Failed to load Glyph";
            g_log->newlog(log);
            continue;
        }
        // 生成纹理
        glID texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // 设置纹理选项
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 储存字符供之后使用
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        m_Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(lib);

    glID VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    m_VAOs.insert(mID("Character_VAO", VAO));
    m_VBOs.insert(mID("Character_VBO", VBO));

    Loadshader("Character", ".\\shader\\vs_character.shader", ".\\shader\\fs_character.shader");
    auto shader = GetShaderByName("Character");
    glm::mat4 projection = glm::ortho(0.0f, float(m_window->width), 0.0f, float(m_window->height));
    shader->setUniform(glUniformType::MAT4, (char*)glm::value_ptr(projection), sizeof(projection), "projection");
}

void Example::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // 激活对应的渲染状态
    auto shader = GetShaderByName("Character");
    auto VAO = GetVAOByName("Character_VAO");
    auto VBO = GetVBOByName("Character_VBO");
    shader->setUniform(glUniformType::VEC3F, (char*)glm::value_ptr(color), sizeof(color), "textColor");
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // 遍历文本中所有的字符
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = m_Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // 对每个字符更新VBO
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // 在四边形上绘制字形纹理
        glBindTexture(GL_TEXTURE_2D, ch.texID);
        // 更新VBO内存的内容
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // 绘制四边形
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // 更新位置到下一个字形的原点，注意单位是1/64像素
        x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
