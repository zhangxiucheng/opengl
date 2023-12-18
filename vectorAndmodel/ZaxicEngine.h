#pragma once
#include"glFrameWork.h"
#include"camera.h"
#include"window.h"
#include"shader.h"
#include"glMesh.h"
#include"glgametimer.h"
#include"glmath.h"
#include"glPhysics.h"
#include"glAnimator.h"

#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<wrl.h>

#define AutoPtr Microsoft::WRL::ComPtr
using ZaxicMathUtil::vec3f;
void OnResize(GLFWwindow* wnd, int width, int height);
void OnMouseClick(GLFWwindow* window, int button, int action, int targ);

struct EngineSetting
{
	uint32_t wnd_width, wnd_height;
	vec3f camera_pos, eyelight, upAxis;
	uint32_t SMAA_level;
};

enum class ZaxicGeoType
{
	Cylinder,
	Sphere,
	Cone
};

void ProcessWINError(HWND hwnd);

class ZaxicEngine
{
#define mID  std::pair<std::string, glID>
#define mProgram std::pair<std::string, std::shared_ptr<shaderProgram>>
#define mModel std::pair<std::string, std::shared_ptr<glModel>>
protected:
	virtual bool EngineInit();
	virtual void loadResource() = 0;
	virtual void OnRender() = 0;
	virtual void processInput() = 0;
	virtual void Update() = 0;

	void AnimatorGo();
public:
	void glCreateWindow(uint32_t width, uint32_t height, bool ifSMAA, int SMAAlevel);
	void glCreateCamera(vec3f campos, vec3f eyelight, vec3f up);
	void CreateAnimator(AnimatorType type, void* targ, void* end, InterpolationType count);
	void LoadModel(std::string name, std::string path);
	void Loadshader(std::string name, std::string vspath, std::string fspath);

	shaderProgram* GetShaderByName(std::string name);
	glModel* GetResourceByName(std::string name);
	glID GetTextureByName(std::string name);
	glID GetVAOByName(std::string name);
	glID GetVBOByName(std::string name);
	glID GetFBOByName(std::string name);

	glwindow* GetWindow();
	glCamera* GetCamera();
protected:
	std::shared_ptr<glCamera> m_camera;
	std::shared_ptr<glwindow> m_window;
	std::map<std::string, glID> m_textures, m_VAOs, m_VBOs, m_FBOs;
	std::map<std::string, std::shared_ptr<shaderProgram>> m_shaders;
	std::map<std::string, std::shared_ptr<glModel>> m_models;
	std::list<std::shared_ptr<glAnimator>>	m_animators;
};

class Example : public ZaxicEngine
{
protected:
	virtual void OnRender() override;
	virtual void Update() override;
	virtual void loadResource() override {};
	virtual void processInput() override;
	virtual bool EngineInit() override;
	//void CreateGeo(ZaxicGeoType type, std::string name, uint32_t ringCount, uint32_t layerCount, float topX, float topY, float topZ, float R1, bool ifDiff = false, float R2 = 0.0f);
public:
	void Run();
	//void SetChosenCar(int tg);
	static bool m_signal[8];
	//~Example();
private:
	int m_player[2] = { -1,-1 };
	unsigned scores[2] = { 0,0 };
	std::vector<glPhysics> m_phys;
	glm::vec3 lit_pos, lit_eyelight, lit_up;
	//std::map<std::string, std::shared_ptr<glMesh>> m_meshes;
	//0:view control method
	//1:if game pause
	//2:if animators exist
	//3:car stop
	//4:ifFirst
	//5:character lib loaded
	
	BB m_border;
	std::map<char, Character> m_Characters;
	void CharacterInit();
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

static Example* last = nullptr;