#include "glAnimator.h"

glAnimator::glAnimator(AnimatorType _type, void* _target, void* _end, InterpolationType method)
{
	Set(_type, _target, _end, method);
}

bool glAnimator::Go()
{
	if (Count)
	{
		Count--;
		switch (type)
		{
		case AnimatorType::Translate:
			*(glm::vec3*)target += translate;
			return true;
			break;
		case AnimatorType::Scale:
			*(glm::vec3*)target += scale;
			return true;
			break;
		case AnimatorType::Rotate:
			(*(glm::vec4*)target).w += rotate.w;
			return true;
			break;
		default:
			return false;
			break;
		}
	}
	return false;
}

void glAnimator::Reset()
{
	target = nullptr;
	Count = 0;
	rotate = glm::vec4(0.);
	type = AnimatorType::Undefined;
}

void glAnimator::Set(AnimatorType _type, void* _target, void* _end, InterpolationType method)
{
	type = _type;
	target = _target;
	Count = method;
	switch (type)
	{
	case AnimatorType::Translate:
		translate = *(glm::vec3*)_end - *(glm::vec3*)target;
		for (auto i = 0; i < 3; i++)
			translate[i] /= (float)Count;
		break;
	case AnimatorType::Scale:
		scale = *(glm::vec3*)_end - *(glm::vec3*)target;
		for (auto i = 0; i < 3; i++)
			scale[i] /= (float)Count;
		break;
	case AnimatorType::Rotate:
		rotate = *(glm::vec4*)_end - *(glm::vec4*)target;
		rotate.w /= float(Count);
		break;
	default:
		break;
	}
}
