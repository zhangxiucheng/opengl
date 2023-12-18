#include"glmath.h"

using namespace ZaxicMathUtil;

mat4f ZaxicMathUtil::GenRotationMatrix(vec3f& axis, const float radians)
{
	mat4f res;
	float cosine = cosf(radians), dcos = 1 - cosine;
	float sine = sinf(radians);
	vec3f norm = axis.norm();
	res[0][0] = cosine + dcos * norm[0] * norm[0];
	res[0][1] = dcos * norm[0] * norm[1] + sine * norm[2];
	res[0][2] = dcos * norm[0] * norm[2] - sine * norm[1];

	res[1][0] = dcos * norm[0] * norm[1] - sine * norm[2];
	res[1][1] = cosine + dcos * norm[1] * norm[1];
	res[1][2] = dcos * norm[2] * norm[1] + sine * norm[0];

	res[2][0] = dcos * norm[0] * norm[2] + sine * norm[1];
	res[2][1] = dcos * norm[1] * norm[2] - sine * norm[0];
	res[2][2] = cosine + dcos * norm[2] * norm[2];
	return res;
}

mat4f ZaxicMathUtil::GenTranslationMatrix(vec3f& mvec)
{
	mat4f res;
	res[3][0] = mvec[0];
	res[3][1] = mvec[1];
	res[3][2] = mvec[2];

	return res;
}

mat4f ZaxicMathUtil::GenScalizeMatrix(vec3f& scale)
{
	mat4f res;
	res[0][0] = scale[0];
	res[1][1] = scale[1];
	res[2][2] = scale[2];
	return res;
}

float ZaxicMathUtil::toRadians(float angle)
{
	return (angle / 180.0f) * ZAXIC_PIf;
}

float ZaxicMathUtil::toTheta(float radians)
{
	return (radians / ZAXIC_PIf) * 180.0f;
}

void glEuler::norm()
{
	auto norm = [](float& t,float bmin,float bmax)
	{
		if (t < bmin)
			t = bmin;
		if (t > bmax)
			t = bmax;
	};

	norm(yaw,-179.0f,179.0f);
	norm(pitch,-89.0f,89.0f);
	norm(roll, -89.0f, 89.0f);
}

glEuler::glEuler(float _yaw, float _pitch, float _roll) :yaw(_yaw), pitch(_pitch), roll(_roll)
{
	norm();
}

glEuler::glEuler(ZaxicMathUtil::vec3f v3)
{
	yaw = v3[0];
	pitch = v3[1];
	roll = v3[2];
	norm();
}


glEuler::glEuler(const glEuler& el)
{
	yaw = el.yaw;
	pitch = el.pitch;
	roll = el.roll;
}

glEuler glEuler::operator=(const glEuler el)
{
	yaw = el.yaw;
	pitch = el.pitch;
	roll = el.roll;
	return *this;
}

glEuler glEuler::operator=(glvector3<float> v)
{
	yaw = v[0];
	pitch = v[1];
	roll = v[2];
	norm();
	return *this;
}

void glEuler::set(EulerMember em, float vl)
{
	switch (em)
	{
	case EulerMember::yaw:
		yaw += vl;
		break;
	case EulerMember::pitch:
		pitch += vl;
		break;
	case EulerMember::roll:
		roll += vl;
		break;
	default:
		break;
	}
	norm();
}

glEuler::glEuler(glMat4<float>& mat)
{
	pitch = toTheta(asinf( - mat.col_2[1]));
	yaw = toTheta(atan2f(mat.col_2[0], mat.col_2[2]));
	roll = toTheta(atan2f(mat.col_0[1],mat.col_1[1]));
}

double glEuler::get(EulerMember em)
{
	switch (em)
	{
	case EulerMember::yaw:
		return toRadians(yaw);
		break;
	case EulerMember::pitch:
		return toRadians(pitch);
		break;
	case EulerMember::roll:
		return toRadians(roll);
		break;
	default:
		return 0.0;
		break;
	}
}

glMat4<float> glEuler::EulerMatrix()
{
	mat4f res;
	float cosy = cosf(toRadians(yaw)), siny = sinf(toRadians(yaw)), cosp = cosf(toRadians(pitch)), sinp = sinf(toRadians(pitch));
	float cosr = cosf(toRadians(roll)), sinr = sinf(toRadians(roll));
	//y
	//vec3f v(0.0f, 1.0f, 0.0f);
	//res = GenRotationMatrix(v, toRadians(yaw)) * res;
	////x
	//v[1] = 0.0f;
	//v[0] = 1.0f;
	//res = GenRotationMatrix(v, toRadians(pitch)) * res;
	////z
	//v[0] = 0.0f;
	//v[2] = 1.0f;
	//res = GenRotationMatrix(v, toRadians(roll)) * res;
	res.x0 = cosy * cosr + siny * sinr * sinp;
	res.x1 = sinr * cosp;
	res.x2 = cosy * sinr * sinp - siny * cosr;
	res.y0 = siny * sinp * cosr - cosy * sinr;
	res.y1 = cosp * cosr;
	res.y2 = sinr * siny + cosy * sinp * cosr;
	res.z0 = siny * cosp;
	res.z1 = -sinp;
	res.z2 = cosy * cosp;


	return res;
}

void glQuaternion::_norm()
{
	v4 = v4.norm();
	if (ZAXIC_FLOAT_SAME(w, -0.0f))
		w = 0.0f;
	if (ZAXIC_FLOAT_SAME(x, -0.0f))
		x = 0.0f;
	if (ZAXIC_FLOAT_SAME(y, -0.0f))
		y = 0.0f;
	if (ZAXIC_FLOAT_SAME(z, -0.0f))
		z = 0.0f;
}

glQuaternion::glQuaternion():v4()
{
}

glQuaternion::glQuaternion(glEuler& euler)
{
	vec3f axis(0.0f, 1.0f, 0.0f);
	glQuaternion y(axis, toRadians(euler.yaw));
	axis.x = 1.0f;
	axis.y = 0.0f;
	glQuaternion x(axis, toRadians(euler.pitch));
	axis.z = 1.0f;
	axis.x = 0.0f;
	glQuaternion z(axis, toRadians(euler.roll));
	*this = z * x * y;
	//_norm();
}

glQuaternion::glQuaternion(glMat4<float>& mat)
{
	float _w = sqrtf(mat.col_0[0] + mat.col_1[1] + mat.col_2[2] + 1.0f) / 2.0f, _x = sqrtf(mat.col_0[0] - mat.col_1[1] - mat.col_2[2] + 1.0f) / 2.0f, _y = sqrtf(-mat.col_0[0] + mat.col_1[1] - mat.col_2[2] + 1.0f) / 2.0f, _z = sqrtf( - mat.col_0[0] - mat.col_1[1] + mat.col_2[2] + 1.0f) / 2.0f;
	if ((_w >= _x) && (_w >= _y) && (_w >= _z))
	{
		w = sqrtf(mat.col_0[0] + mat.col_1[1] + mat.col_2[2] + 1.0f) / 2.0f;
		float _4w = 4.0f * w;
		v3 = glvector3<float>((mat.col_1[2] - mat.col_2[1]) / _4w, (mat.col_2[0] - mat.col_0[2]) / _4w, (mat.col_0[1] - mat.col_1[0]) / _4w);
	}
	else if ((_x >= _w) && (_x >= _y) && (_x >= _z))
	{
		x = _x;
		float _4x = 4.0f * x;
		w = (mat.col_1[2] - mat.col_2[1]) / _4x;
		y = (mat.col_0[1] + mat.col_1[0]) / _4x;
		z = (mat.col_2[0] + mat.col_0[2]) / _4x;
	}
	else if ((_y >= _w) && (_y >= _x) && (_y >= _z))
	{
		y = _y;
		float _4y = 4.0f * y;
		w = (mat.col_2[0] - mat.col_0[2]) / _4y;
		x = (mat.col_0[1] + mat.col_1[0]) / _4y;
		z = (mat.col_1[2] + mat.col_2[1]) / _4y;
	}
	else
	{
		z = _z;
		float _4z = 4.0f * z;
		w = (mat.col_0[1] - mat.col_1[0]) / _4z;
		x = (mat.col_2[0] + mat.col_0[2]) / _4z;
		y = (mat.col_1[2] + mat.col_2[1]) / _4z;
	}
	//_norm();
}

glQuaternion::glQuaternion(const glQuaternion& quat)
{
	v4 = quat.v4;
	//_norm();
}

glQuaternion::glQuaternion(glvector4<float> v)
{
	v4 = v;
	//_norm();
}

glQuaternion::glQuaternion(float _w, glvector3<float> v)
{
	w = _w;
	v3 = v;
	//_norm();
}

glQuaternion::glQuaternion(glvector3<float>& axis, float radians)
{
	radians = radians / 2.0f;
	w = cos(radians);
	v3.x = axis.x * sin(radians);
	v3.y = axis.y * sin(radians);
	v3.z = axis.z * sin(radians);
	//_norm();
}

glQuaternion glQuaternion::operator=(glQuaternion quat)
{
	v4 = quat.v4;
	//_norm();
	return *this;
}


glQuaternion glQuaternion::norm()
{
	return v4.norm();
}

glQuaternion glQuaternion::operator*(glQuaternion quat)
{
	glQuaternion res;
	res.w = w * quat.w - v3 * quat.v3;
	res.v3 =quat.v3.crossproduct(v3) + w * quat.v3 + quat.w * v3;
	
	return res;
}

glQuaternion glQuaternion::operator-(glQuaternion quat)
{
	glQuaternion res = inverse() * quat;
	return res;
}

glMat4<float> glQuaternion::toMatrix()
{
	glMat4<float> mat;
	float wz = 2.0f * w * z;
	float xy = 2.0f * x * y;
	float xz = 2.0f * x * z;
	float wy = 2.0f * w * y;
	float wx = 2.0f * x * w;
	float yz = 2.0f * y * z;
	float xx = 2.0f * x * x;
	float yy = 2.0f * y * y;
	float zz = 2.0f * z * z;
	mat.x0 = 1.0f - (yy + zz);
	mat.x1 = xy + wz;
	mat.x2 = xz - wy;
	mat.y0 = xy - wz;
	mat.y1 = 1.0f - (xx + zz);
	mat.y2 = yz + wx;
	mat.z0 = xz + wy;
	mat.z1 = yz - wx;
	mat.z2 = 1.0f - (xx + yy);
	
	return mat;
}

glQuaternion glQuaternion::inverse()
{
	float l = v4.x * v4.x + v4.y * v4.y + v4.z * v4.z + v4.w * v4.w;
	l = 1.0f / l;
	glQuaternion res(w * l, -v3 * l);
	return res;
}

float glQuaternion::len()
{
	return v4.veclength();
}

float glQuaternion::dot(glQuaternion quat)
{
	return v4 * quat.v4;
}




glQuaternion SLERP(glQuaternion q1, glQuaternion q2, float alpha)
{
	glQuaternion res;
	float omega = acosf(q1.dot(q2));
	res.v4 =  (q1.v4 * sinf((1 - alpha) * omega)  +  q2.v4 * sinf(omega * alpha)) * (1.0f / sinf(omega));
	return res;
}

glQuaternion operator*(float n, glQuaternion q)
{
	return glQuaternion(q.v4 * n);
}

glQuaternion operator*(glQuaternion q, float n)
{
	return glQuaternion(q.v4 * n);
}

glEuler toglEuler(glQuaternion& quat)
{
	float yaw, pitch, roll;
	pitch = asinf(2.0f * (quat.x * quat.w - quat.y * quat.z));
	float cosp = cosf(pitch);
	if (ZAXIC_FLOAT_SAME(cosp, 0.0f))
		yaw = atan2f(2.0f * (quat.w * quat.y - quat.z * quat.x), 1.0f - 2.0f * (quat.z * quat.z + quat.y * quat.y));
	else
		yaw = atan2f(2.0f * (quat.z * quat.x + quat.w * quat.y), 1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y));

	if (ZAXIC_FLOAT_SAME(cosp, 0.0f))
		roll = 0.0f;
	else
		roll = atan2f(2.0f * (quat.y * quat.x + quat.w * quat.z), 1.0f - 2.0f * (quat.x * quat.x + quat.z * quat.z));

	pitch = toTheta(pitch);
	yaw = toTheta(yaw);
	roll = toTheta(roll);

	return glEuler(yaw, pitch, roll);
}

