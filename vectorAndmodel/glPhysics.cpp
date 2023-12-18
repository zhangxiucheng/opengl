#include "glPhysics.h"

BB glPhysics::updateBB()
{
	BB _bb = bb;
	glm::mat4 mat;
	mat = glm::translate(mat, pos);
	//glm::mat4 r;
	//r = glm::rotate(r, glm::radians(dirAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	//mat = mat * r;
	glm::vec4 t(bb.max, 1.0f);
	t = mat * t;
	_bb.max = glm::vec3(t);
	t = glm::vec4(bb.min, 1.0f);
	t = mat * t;
	_bb.min = glm::vec3(t);

	return _bb;
}

void glPhysics::AlignedAngle()
{
	if (dirAngle >= 180.0f)
		dirAngle -= 360.0f;

	if (dirAngle <= -180.0f)
		dirAngle += 360.0f;
}

glm::vec3 glPhysics::getDir()
{
	glm::mat4 mat;
	mat = glm::rotate(mat, glm::radians(dirAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat3 _ = glm::mat4(mat);
	return glm::normalize(_ * glm::vec3(0.0f, 0.0f, 1.0f));
}

glPhysics::glPhysics()
{
	speed = DefaultSpeed;
	collisionloss = DefaultCollisionLoss;
	borderloss = DefaultBorderLoss;
	sensitivity = DefaultSensitivity;
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	dirAngle = DefaultDirAngle;
	memset(&bb, 0, sizeof(bb));
}

glPhysics::glPhysics(glm::vec3 p, float dir, const BB& _bb, float sp, float cl, float bl) :speed(sp),collisionloss(cl),borderloss(bl),sensitivity(DefaultSensitivity)
{
	pos = p;
	bb = _bb;
	dirAngle = dir;
}

glPhysics::glPhysics(const glPhysics& t)
{
	speed = t.speed;
	collisionloss = t.collisionloss;
	borderloss = t.borderloss;
	bb = t.bb;
	dirAngle = t.dirAngle;
	sensitivity = t.sensitivity;
}

glPhysics glPhysics::operator=(const glPhysics& t)
{
	speed = t.speed;
	collisionloss = t.collisionloss;
	borderloss = t.borderloss;
	bb = t.bb;
	sensitivity = t.sensitivity;
	dirAngle = t.dirAngle;
	return *this;
}

GLboolean glPhysics::Collision_Single(const BB& _bb)
{
	BB _ = updateBB();
	auto lap = [](float lmin, float lmax, float rmin, float rmax)->bool
	{
		return !((lmax < rmin) || (lmin > rmax));
	};
	return lap(_.min.x, _.max.x, _bb.min.x, _bb.max.x) && lap(_.min.z, _.max.z, _bb.min.z, _bb.max.z) && lap(_.min.y, _.max.y, bb.min.y, bb.max.y);
}

void glPhysics::Move(float deltaT)
{
	pos += getDir() * (speed * deltaT);
	//updateBB();
}

void glPhysics::Turn(Directions d)
{
	switch (d)
	{
	case Directions::LEFT:
		dirAngle += DefaultDetaAngle;
		break;
	case Directions::RIGHT:
		dirAngle -= DefaultDetaAngle;
		break;
	case Directions::FORWARD:
		speed += DefaultDeltaSpeed;
		if (speed > 3.f)
			speed = 3.f;
		break;
	case Directions::BACKWARD:
		speed -= DefaultDeltaSpeed;
		if (speed < 0.f)
			speed = 0;
		break;
	default:
		break;
	}

	AlignedAngle();
}

void glPhysics::Momentum(glPhysics& t)
{
	BB _bb = updateBB(), tbb = t.updateBB();
	float tmp = t.dirAngle;
	t.dirAngle = dirAngle;
	dirAngle = tmp;
	//change position
	Move(0.1f);
	t.Move(0.1f);

	//reset location
	//glm::vec3 c1((_bb.max + _bb.min) * 0.5f), c2((tbb.max + tbb.min) * 0.5f);
	//float dx = fabs(c1.x - _bb.min.x + c2.x - tbb.min.x - c1.x + c2.x) / 2.0f, dz = fabs(c1.z - _bb.min.z + c2.z - tbb.min.z - c1.z - c2.z) / 2.0f;
	//if (c1.x > c2.x)
	//{
	//	c1.x = dx;
	//	c2.x = -dx;
	//}
	//else
	//{
	//	c1.x = -dx;
	//	c2.x = dx;
	//}

	//if (c1.z > c2.z)
	//{
	//	c1.z = dz;
	//	c2.z = -dz;
	//}
	//else
	//{
	//	c1.z = -dz;
	//	c2.z = dz;
	//}
	//c1.y = 0.f;
	//c2.y = 0.f;

	////reset location
	//pos += c1;
	//t.pos += c2;
}

void glPhysics::CheckBorder(const BB& _bb)
{
	BB __ = updateBB();
	if ((__.min.x < _bb.min.x) || (_bb.max.x < __.max.x))
		dirAngle = -dirAngle;
	/*if ((__.min.y < _bb.min.y) || (_bb.max.y < __.min.y))
		dir.y = -dir.y;*/
	if ((__.min.z < _bb.min.z) || (_bb.max.z < __.max.z))
		dirAngle = 180.0f - dirAngle;

}

glm::mat4 glPhysics::newModelMatrix()
{
	glm::mat4 res;
	/*float theta = glm::angle(glm::normalize(dir), glm::vec3(0.0f, 0.0f, 1.0f));
	if (dir.z < 0)
		theta = -theta;*/
	res = glm::rotate(res, ZaxicMathUtil::toRadians(dirAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::vec4 _pos = glm::vec4(pos, 1.0f);
	//_pos = res * _pos;
	glm::mat4 trans;
	trans = glm::translate(trans, pos);
	 return trans * res;
}

