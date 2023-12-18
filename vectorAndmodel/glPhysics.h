#pragma once
//#include"glMesh.h"
#include"glFrameWork.h"
#include<glm/gtx/vector_angle.hpp>
//defined based on rigid system
#define DefaultCollisionLoss 0.3f
#define DefaultBorderLoss	 0.0f
#define DefaultSpeed		 0.9f
#define DefaultDeltaSpeed    0.1f
#define DefaultSensitivity   0.008f
#define DefaultDirAngle		 0.0f
#define DefaultDetaAngle	 1.0f

//car init direction
const glm::vec3 X(0.0f, 0.0f, 1.0f);

struct BindingBox
{
	glm::vec3 max, min;
};
using BB = struct BindingBox;

enum class Directions
{
	RIGHT,
	LEFT,
	FORWARD,
	BACKWARD
};

class glPhysics
{
public:
	BB updateBB();
	void AlignedAngle();
public:
	float speed, dirAngle, collisionloss, borderloss, sensitivity;
	glm::vec3 pos;
	BB bb;
	bool pause = false;
	glPhysics();
	glPhysics(glm::vec3 p, float dir, const BB& _bb, float sp = DefaultSpeed, float cl = DefaultCollisionLoss, float bl = DefaultBorderLoss);
	glPhysics(const glPhysics& t);
	glPhysics operator=(const glPhysics& t);

	glm::vec3 getDir();
	GLboolean Collision_Single(const BB& _bb);
	void Move(float deltaT);
	void Turn(Directions d);
	void Momentum(glPhysics& t);
	void CheckBorder(const BB& _bb);
	glm::mat4 newModelMatrix();
};



