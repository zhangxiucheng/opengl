#pragma once
#include"glFrameWork.h"
enum class AnimatorType
{
	Translate,
	Scale,
	Rotate,
	Undefined
};

using InterpolationType = unsigned;
class glAnimator
{
private:
	union
	{
		glm::vec3 translate;
		glm::vec3 scale;
		//axis + angle
		glm::vec4 rotate;
	};
	void* target;
	InterpolationType Count;
	AnimatorType type;
public:
	glAnimator(AnimatorType _type, void* _target, void* _end, InterpolationType method);
	glAnimator() = delete;
	glAnimator(const glAnimator&) = delete;

	bool Go();
	//clear mem
	void Reset();
	void Set(AnimatorType _type, void* _target, void* _end, InterpolationType method);
};

