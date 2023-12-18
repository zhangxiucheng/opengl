#include"camera.h"
//glCamera* g_Camera = nullptr;

glCamera::glCamera(const glm::vec3& _pos, const glm::vec3& _eye, const glm::vec3& _up):el(0.0f,0.0f,0.0f)
{
	/*if (g_Camera != nullptr)
		return;
	else
		g_Camera = this;*/
	CmrPos = _pos;
	CmrEyelight = glm::normalize(_eye);
	CmrUp = glm::normalize(_up);
	speed = DEFAULT_SPEED;
	fov = 45.0f;
	ifnorm = true;

}

glCamera::glCamera(const float posx, const float posy, const float posz, const float ex, const float ey, const float ez, const float upx, const float upy, const float upz):el(0.0f, 0.0f, 0.0f)
{
	/*if (g_Camera != nullptr)
		return;
	else
		g_Camera = this;*/
	CmrPos.x = posx;
	CmrPos.y = posy;
	CmrPos.z = posz;
	CmrEyelight.x = ex;
	CmrEyelight.y = ey;
	CmrEyelight.z = ez;
	CmrUp.x = upx;
	CmrUp.y = upy;
	CmrUp.z = upz;
	speed = DEFAULT_SPEED;
	fov = 45.0f;

	CmrEyelight = glm::normalize(CmrEyelight);
	CmrUp = glm::normalize(CmrUp);
	ifnorm = true;
}

glm::mat4 glCamera::update(float ratio)
{
	return glm::perspective(glm::radians(fov), ratio, 0.1f, 100.0f) * glm::lookAt(CmrPos, CmrEyelight + CmrPos, CmrUp);
}

glm::mat4 glCamera::viewMatrix()
{
	return glm::lookAt(CmrPos, CmrEyelight + CmrPos, CmrUp);
}

glm::mat4 glCamera::projMatrix(float ratio)
{
	return glm::perspective(glm::radians(fov), ratio, 0.1f, 1000.0f);
}

void glCamera::updatevf(const glm::vec3& _pos, glCameraMember cmm)
{
	switch (cmm)
	{
	case glCameraMember::CameraPosition:
		CmrPos = _pos;
		break;
	case glCameraMember::CameraEyeLight:
		if (ifnorm)
			CmrEyelight = glm::normalize(_pos);
		else
			CmrEyelight = _pos;
		break;
	case glCameraMember::CameraUpAxis:
		if (ifnorm)
			CmrUp = glm::normalize(_pos);
		else
			CmrUp = _pos;
		break;
	default:
		break;
	}
}

void glCamera::update3f(const float x, const float y, const float z, glCameraMember cmm)
{
	switch (cmm)
	{
	case glCameraMember::CameraPosition:
		CmrPos.x = x;
		CmrPos.y = y;
		CmrPos.z = z;
		break;
	case glCameraMember::CameraEyeLight:
		CmrEyelight.x = x;
		CmrEyelight.y = y;
		CmrEyelight.z = z;
		if (ifnorm)
			CmrEyelight = glm::normalize(CmrEyelight);
		break;
	case glCameraMember::CameraUpAxis:
		CmrUp.x = x;
		CmrUp.y = y;
		CmrUp.z = z;
		if (ifnorm)
			CmrUp = glm::normalize(CmrUp);
		break;
	default:
		break;
	}
}

void glCamera::move(const glm::vec3& dir)
{
	CmrPos = CmrPos + dir;
}

glm::vec3 glCamera::getZn()
{
	glm::vec3 rt = glm::normalize(-CmrEyelight);
	return rt;
}

glm::vec3 glCamera::getXn()
{
	glm::vec3 rt = glm::normalize(glm::cross(CmrUp, -CmrEyelight));
	return rt;
}

glm::vec3 glCamera::getYn()
{
	glm::vec3 rt = glm::normalize(CmrUp);
	return rt;
}

const glm::vec3 glCamera::get(glCameraMember cmm)
{
	switch (cmm)
	{
	case glCameraMember::CameraPosition:
		return CmrPos;
	case glCameraMember::CameraEyeLight:
		return CmrEyelight;
	case glCameraMember::CameraUpAxis:
		return CmrUp;
	default:
		glm::vec3 z(0, 0, 0);
		return z;
	}
}

float glCamera::cmspeed()
{
	return speed;
}

void glCamera::setCmSpeed(float _s)
{
	speed += _s;
	if (speed < 0.0f)
		speed = 0.0f;
}

void glCamera::Turn(glCameraDirection cd, float val)
{
	glm::mat4 mat;
	switch (cd)
	{
	case glCameraDirection::Up:
		CmrEyelight = CmrEyelight * glm::mat3(glm::rotate(mat, glm::radians(val), getXn()));
		CmrUp = glm::normalize(glm::cross(getXn(), CmrEyelight));
		//el.set(EulerMember::pitch, val);
		break;
	case glCameraDirection::Right:
		CmrEyelight = CmrEyelight * glm::mat3(glm::rotate(mat, glm::radians(val), getYn()));
		//el.set(EulerMember::yaw, val);
		break;
	case glCameraDirection::Front:
		CmrUp = glm::normalize(CmrUp * glm::mat3(glm::rotate(mat, glm::radians(val), getZn())));
		CmrEyelight = glm::normalize(glm::cross(CmrUp, getXn()));
		//el.set(EulerMember::roll, val);
		break;
	default:
		break;
	}
}

glCamera::~glCamera()
{
}
