#ifndef __GL_CAMERA__
#define __GL_CAMERA__
#include"glFrameWork.h"
#include"window.h"
enum class glCameraMember
{
	CameraPosition,
	CameraEyeLight,
	CameraUpAxis
};

enum  class glCameraDirection
{
	Up,
	Right,
	Front
};

#define DEFAULT_SPEED 20.0f
#define DEFAULT_SPEED_DELTA 0.5f
class glCamera 
{
private:
	glm::vec3 CmrPos, CmrEyelight, CmrUp;
	float speed;
public:
	glEuler el;
	float fov;
	bool ifnorm;

	glCamera() = delete;
	glCamera(const glm::vec3& _pos, const glm::vec3& _eye, const glm::vec3& _up);
	glCamera(const float posx, const float posy, const float posz, const float tgtx, const float tgty, const float tgtz, const float upx, const float upy, const float upz);
	
	glCamera(const glCamera& cmr) = delete;
	glCamera& operator=(const glCamera& cmr) = delete;

	glm::mat4 update(float ratio);
	glm::mat4 viewMatrix();
	glm::mat4 projMatrix(float ratio);

	void updatevf(const glm::vec3& _pos,glCameraMember cmm);
	void update3f(const float x, const float y, const float z, glCameraMember cmm);

	void move(const glm::vec3& dir);
	glm::vec3 getZn();
	glm::vec3 getXn();
	glm::vec3 getYn();
	const glm::vec3 get(glCameraMember cmm);
	float cmspeed();
	void setCmSpeed(float _s);

	void Turn(glCameraDirection cd, float val);

	~glCamera();
};
//extern glCamera* g_Camera;
#endif

