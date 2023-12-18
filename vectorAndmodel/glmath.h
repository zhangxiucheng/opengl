
#ifndef __GL_MATH__
#define __GL_MATH__
#include<type_traits>
#include<cinttypes>
#include<memory>
#include<immintrin.h>
#include<math.h>

#define ZAXIC_EPSILON (1e-5f)
#define ZAXIC_FLOAT_SAME(a, b) (abs((a)-(b))<=ZAXIC_EPSILON)

const __declspec(align(16)) __m256i row1d4 = _mm256_set_epi32(0, 4, 8, 12, 0, 4, 8, 12);
const __declspec(align(16)) __m256i row2d4 = _mm256_set_epi32(1, 5, 9, 13, 1, 5, 9, 13);
const __declspec(align(16)) __m256i row3d4 = _mm256_set_epi32(2, 6, 10, 14, 2, 6, 10, 14);
const __declspec(align(16)) __m256i row4d4 = _mm256_set_epi32(3, 7, 11, 15, 3, 7, 11, 15);

const __declspec(align(16)) __m256i column124 = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);
const __declspec(align(16)) __m256i column344 = _mm256_set_epi32(8, 9, 10, 11, 12, 13, 14, 15);
#define __SIMD_MASK__ (0b0000000000000000000000011110001)
#define exchange(v1,v2) {\
	(v1) ^= (v2);\
	(v2) ^= (v1);\
	(v1) ^= (v2);\
}

template<typename T> class glvector4 
{
public:
	union
	{
		T num[4];
		struct
		{
			T x, y, z, w;
		};
	};
public:
	//init func
	glvector4();
	glvector4(T _dims);
	glvector4(uint32_t sz,const T* nums);
	glvector4(T n0, T n1, T n2, T n3);
	//copy func
	glvector4(const glvector4& vec);
	glvector4 operator=(const glvector4 vec);
	//operators
	glvector4 operator+(const glvector4 vec);
	glvector4 operator-(const glvector4 vec);
	T operator*(const glvector4 vec);
	T& operator[](const uint32_t iter);

	//vector calculation
	glvector4 scalarMul(const T k);
	glvector4 norm();
	glvector4 proj(glvector4 vec);
	glvector4 crossproduct(const glvector4 vec);
	T veclength();
	//other opts
	T* get();
	void zero();
	void set(uint32_t _dims, const T* nums);

	~glvector4();
};
#endif

#ifndef __GL_MATH_1__
#define __GL_MATH_1__
template<typename T> class glvector3
{
public:
	union 
	{
		T num[3];
		struct
		{
			T x, y, z;
		};
	};
public:
	//init func
	glvector3();
	glvector3(T _dims);
	glvector3(uint32_t sz, const T* nums);
	glvector3(T n1, T n2, T n3);
	//copy func
	glvector3(const glvector3& vec);
	glvector3 operator=(const glvector3 vec);
	//operators
	glvector3 operator+(const glvector3 vec);
	glvector3 operator-(const glvector3 vec);
	T operator*(const glvector3 vec);
	T& operator[](const uint32_t iter);

	//vector calculation
	glvector3 scalarMul(const T k);
	glvector3 norm();
	glvector3 proj(glvector3 vec);
	glvector3 crossproduct(const glvector3 vec);
	T veclength();
	//other opts
	T* get();
	void zero();
	void set(uint32_t sz, const T* nums);

	~glvector3();
};
#endif

#ifndef __GL_MATH_2__
#define __GL_MATH_2__
template<typename M> class glMat4
{
public:
	union
	{
		M dat[16];
		struct
		{
			M x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3;
		};
		struct
		{
			glvector4<M> col_0, col_1, col_2, col_3;
		};
	};
public:
	//identity
	glMat4();
	glMat4(uint32_t sz, M* _dat);
	glMat4(const glMat4<M>& mat);

	M det();

	glMat4<M> transpos();
	glMat4<M> inverse();

	glMat4<M> operator=(const glMat4<M> mat);

	glMat4<M> operator+(const glMat4<M> mat);
	glMat4<M> operator-(const glMat4<M> mat);
	glMat4<M> operator*(glMat4<M> mat);
	glMat4<M> operator*(const M num);
	M* operator[](const uint32_t iter);
	//M* operator&(const glMat4<M>& mat);



	friend glMat4<M> operator*(const M num, glMat4<M> mat)
	{
		glMat4<M> res = mat;
		for (auto i = 0; i < 16; i++)
			res.dat[i] *= num;
		return res;
	}

	~glMat4() {}
};
#endif

#ifndef __GL_MATH_3__
#define __GL_MATH_3__
template<typename M> class glMat3
{
public:
	union
	{
		M dat[9];
		struct
		{
			M x0, y0, z0, x1, y1, z1, x2, y2, z2;
		};
		struct
		{
			glvector3<M> col_0, col_1, col_2;
		};
	};
public:
	//identity
	glMat3();
	glMat3(uint32_t sz, M* _dat);
	glMat3(const glMat4<M>& mat);
	glMat3(const glMat3<M>& mat);

	M det();

	glMat3<M> transpos();
	glMat3<M> inverse();

	glMat3<M> operator=(const glMat3<M> mat);

	glMat3<M> operator+(const glMat3<M> mat);
	glMat3<M> operator-(const glMat3<M> mat);
	glMat3<M> operator*(glMat3<M> mat);
	glMat3<M> operator*(const M num);
	M* operator[](const uint32_t iter);


	friend glMat3<M> operator*(const M num, glMat3<M> mat)
	{
		glMat3<M> res = mat;
		for (auto i = 0; i < 9; i++)
			res.dat[i] *= num;
		return res;
	}

	~glMat3() {}
};
#endif


#ifndef __GL_MATH_4__
#define __GL_MATH_4__
template<typename T>
inline glvector4<T>::glvector4()
{
	for (auto i = 0; i < 4; i++)
		num[i] = 0;
}

template<typename T>
inline glvector4<T>::glvector4(T _dims)
{
	for (auto i = 0; i < 4; i++)
		num[i] = _dims;
}
template<typename T>
inline glvector4<T>::glvector4(uint32_t sz, const  T* nums)
{
	if (sz != 16)
		for (auto i = 0; i < 4; i++)
			num[i] = 0;
	else
		memcpy(num, nums, sz);
}

template<typename T>
inline glvector4<T>::glvector4(T n0, T n1, T n2, T n3)
{
	num[0] = n0;
	num[1] = n1;
	num[2] = n2;
	num[3] = n3;
}

template<typename T>
inline glvector4<T>::glvector4(const glvector4& vec)
{
	/*if (num!=nullptr)
		delete[] num;*/
	memcpy(num, vec.num, sizeof(T) * 4);
}

template<typename T>
inline glvector4<T> glvector4<T>::operator=(const glvector4<T> vec)
{
	memcpy(num, vec.num, sizeof(T) * 4);
	return *this;
}

template<typename T>
inline glvector4<T> glvector4<T>::operator+(const glvector4<T> vec)
{
	glvector4<T> res;
	for (auto i = 0; i < 4; ++i)
	{
		res.num[i] = num[i] + vec.num[i];
	}

	return res;
}

template<typename T>
inline glvector4<T> glvector4<T>::operator-(const glvector4<T> vec)
{
	glvector4<T> res;
	for (auto i = 0; i < 4; ++i)
	{
		res.num[i] = num[i] - vec.num[i];
	}

	return res;
}

template<typename T>
inline T glvector4<T>::operator*(const glvector4 vec)
{
	T res = 0;
	for (auto i = 0; i < 4; i++)
		res = res + num[i] * vec.num[i];

	return res;
}

template<typename T>
inline T& glvector4<T>::operator[](const uint32_t iter)
{
	if (iter < 4)
		return num[iter];
	else
		return num[0];
}


template<typename T>
inline glvector4<T> glvector4<T>::scalarMul(const T k)
{
	glvector4<T> res;
	for (auto i = 0; i < 4; i++)
		res.num[i] = num[i] * k;
	return res;
}

template<typename T>
inline glvector4<T> glvector4<T>::norm()
{
	T l = veclength();
	glvector4<T> res;
	for (auto i = 0; i < 4; i++)
		res.num[i] = num[i] / l;

	return res;
}

template<typename T>
inline glvector4<T> glvector4<T>::proj(glvector4<T> vec)
{
	T res = (*this) * vec;
	glvector4<T> _res = vec.norm();
	_res = _res.scalarMul(res / (vec.veclength()));
	return _res;
}

template<typename T>
inline glvector4<T> glvector4<T>::crossproduct(const glvector4<T> vec)
{
	if (num[3] || vec.num[3])
		return glvector4();

	glvector4<T> res;

	res.num[0] = num[1] * vec.num[2] - num[2] * vec.num[1];
	res.num[1] = num[2] * vec.num[0] - num[0] * vec.num[2];
	res.num[2] = num[0] * vec.num[1] - num[1] * vec.num[0];
	res.num[3] = 0;

	return res;
}

template<typename T>
inline T glvector4<T>::veclength()
{
	T l = 0;
	for (auto i = 0; i < 4; i++)
		l = l + num[i] * num[i];

	return sqrt(static_cast<double>(l));
}

template<typename T>
inline T* glvector4<T>::get()
{
	return num;
}

template<typename T>
inline void glvector4<T>::zero()
{
	memset(num, 0, sizeof(T) * 4);
}

template<typename T>
inline void glvector4<T>::set(uint32_t _dims, const T* nums)
{
	if (_dims != 4 * sizeof(T))
		return;
	for (auto i = 0; i < 4; i++)
		num[i] = nums[i];
}

template<typename T>
inline glvector4<T>::~glvector4()
{
}

template<typename T>
glvector4<T> operator-(glvector4<T> v)
{
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
	v.w = -v.w;
	return v;
}

template<typename T>
glvector4<T> operator*(T n, glvector4<T> v)
{
	return v.scalarMul(n);
}

template<typename T>
glvector4<T> operator*(glvector4<T> v, T n)
{
	return v.scalarMul(n);
}

#endif


#ifndef __GL_MATH_5__
#define __GL_MATH_5__
//vec3
template<typename T>
inline glvector3<T>::glvector3()
{
	for (auto i = 0; i < 3; i++)
		num[i] = 0;
}

template<typename T>
inline glvector3<T>::glvector3(T _dims)
{
	for (auto i = 0; i < 3; i++)
		num[i] = _dims;
}
template<typename T>
inline glvector3<T>::glvector3(uint32_t sz, const  T* nums)
{
	if (sz != 12)
		for (auto i = 0; i < 3; i++)
			num[i] = 0;
	else
		memcpy(num, nums, sz);
}

template<typename T>
inline glvector3<T>::glvector3(T n1, T n2, T n3)
{
	num[0] = n1;
	num[1] = n2;
	num[2] = n3;
}

template<typename T>
inline glvector3<T>::glvector3(const glvector3& vec)
{
	/*if (num!=nullptr)
		delete[] num;*/
	memcpy(num, vec.num, sizeof(T) * 3);
}

template<typename T>
inline glvector3<T> glvector3<T>::operator=(const glvector3<T> vec)
{
	memcpy(num, vec.num, sizeof(T) * 3);
	return *this;
}

template<typename T>
inline glvector3<T> glvector3<T>::operator+(const glvector3<T> vec)
{
	glvector3<T> res;
	for (auto i = 0; i < 3; ++i)
	{
		res.num[i] = num[i] + vec.num[i];
	}

	return res;
}

template<typename T>
inline glvector3<T> glvector3<T>::operator-(const glvector3<T> vec)
{
	glvector3<T> res();
	for (auto i = 0; i < 3; ++i)
	{
		res.num[i] = num[i] - vec.num[i];
	}

	return res;
}

template<typename T>
inline T glvector3<T>::operator*(const glvector3<T> vec)
{
	T res = 0;
	for (auto i = 0; i < 3; i++)
		res = res + num[i] * vec.num[i];

	return res;
}

template<typename T>
inline T& glvector3<T>::operator[](const uint32_t iter)
{
	if (iter < 3)
		return num[iter];
	else
		return num[0];
}


template<typename T>
inline glvector3<T> glvector3<T>::scalarMul(const T k)
{
	glvector3<T> res;
	for (auto i = 0; i < 3; i++)
		res.num[i] = num[i] * k;
	return res;
}

template<typename T>
inline glvector3<T> glvector3<T>::norm()
{
	T l = veclength();
	glvector3<T> res;
	for (auto i = 0; i < 3; i++)
		res.num[i] = num[i] / l;

	return res;
}

template<typename T>
inline glvector3<T> glvector3<T>::proj(glvector3<T> vec)
{
	T res = (*this) * vec;
	glvector3<T> _res = vec.norm();
	_res = _res.scalarMul(res / (vec.veclength()));
	return _res;
}

template<typename T>
inline glvector3<T> glvector3<T>::crossproduct(const glvector3<T> vec)
{

	glvector3<T> res;

	res.num[0] = num[1] * vec.num[2] - num[2] * vec.num[1];
	res.num[1] = num[2] * vec.num[0] - num[0] * vec.num[2];
	res.num[2] = num[0] * vec.num[1] - num[1] * vec.num[0];

	return res;
}

template<typename T>
inline T glvector3<T>::veclength()
{
	T l = 0;
	for (auto i = 0; i < 3; i++)
		l = l + num[i] * num[i];

	return sqrt(static_cast<double>(l));
}

template<typename T>
inline T* glvector3<T>::get()
{
	return num;
}

template<typename T>
inline void glvector3<T>::zero()
{
	memset(num, 0, sizeof(T) * 3);
}

template<typename T>
inline void glvector3<T>::set(uint32_t _dims, const T* nums)
{
	if (_dims != 3 * sizeof(T))
		return;
	for (auto i = 0; i < 3; i++)
		num[i] = nums[i];
}

template<typename T>
inline glvector3<T>::~glvector3()
{
}

template<typename T>
glvector3<T> operator-(glvector3<T> v)
{
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
	return v;
}

template<typename T>
glvector3<T> operator*(T n, glvector3<T> v)
{
	return v.scalarMul(n);
}

template<typename T>
glvector3<T> operator*(glvector3<T> v, T n)
{
	return v.scalarMul(n);
}

#endif

#ifndef __GL_MATH_6__
#define __GL_MATH_6__
template<typename M>
inline glMat4<M>::glMat4()
{
	memset(dat, 0, sizeof(dat));
	dat[0] = 1.0f;
	dat[5] = 1.0f;
	dat[10] = 1.0f;
	dat[15] = 1.0f;
}

template<typename M>
inline glMat4<M>::glMat4(uint32_t sz, M* _dat)
{
	if (sz != 64)
		memset(dat, 0, sizeof(dat));
	else
		memcpy(dat, _dat, sz);
}

template<typename M>
inline glMat4<M>::glMat4(const glMat4<M>& mat)
{
	memcpy(dat, mat.dat, sizeof(dat));
}

template<typename M>
inline M glMat4<M>::det()
{
	M res = 0;
	res += dat[0] * (dat[5] * (dat[10] * dat[15] - dat[11] * dat[14]) - dat[9] * (dat[6] * dat[15] - dat[7] * dat[14]) + dat[13] * (dat[6] * dat[11] - dat[7] * dat[10]));
	res -= dat[4] * (dat[1] * (dat[10] * dat[15] - dat[11] * dat[14]) - dat[9] * (dat[2] * dat[15] - dat[3] * dat[14]) + dat[13] * (dat[2] * dat[11] - dat[3] * dat[10]));
	res += dat[8] * (dat[1] * (dat[6] * dat[15] - dat[7] * dat[14]) - dat[5] * (dat[2] * dat[15] - dat[3] * dat[14]) + dat[13] * (dat[2] * dat[7] - dat[3] * dat[6]));
	res -= dat[12] * (dat[1] * (dat[6] * dat[11] - dat[7] * dat[10]) - dat[5] * (dat[2] * dat[11] - dat[3] * dat[10]) + dat[9] * (dat[2] * dat[7] - dat[3] * dat[6]));
	return res;
}

template<typename M>
inline glMat4<M> glMat4<M>::transpos()
{
	glMat4<M> res = *this;
	exchange(res.dat[1], res.dat[4]);
	exchange(res.dat[2], res.dat[8]);
	exchange(res.dat[3], res.dat[12]);
	exchange(res.dat[6], res.dat[9]);
	exchange(res.dat[7], res.dat[13]);
	exchange(res.dat[11], res.dat[14]);
	return res;
}

template<typename M>
inline glMat4<M> glMat4<M>::inverse()
{
	glMat4<M> res;
	M _det = det();
	if (!_det)
		return res;
	auto det3 = [](M m00, M m10, M m20, M m01, M m11, M m21, M m02, M m12, M m22)->M
	{
		return (m00 * m11 * m22 + m01 * m12 * m20 + m10 * m21 * m02 - m02 * m11 * m20 - m10 * m01 * m22 - m00 * m21 * m12);
	};

	res.dat[0] = det3(dat[5], dat[6], dat[7], dat[9], dat[10], dat[11], dat[13], dat[14], dat[15]) / _det;
	res.dat[1] = -det3(dat[1], dat[2], dat[3], dat[9], dat[10], dat[11], dat[13], dat[14], dat[15]) / _det;
	res.dat[2] = det3(dat[1], dat[2], dat[3], dat[5], dat[6], dat[7], dat[13], dat[14], dat[15]) / _det;
	res.dat[3] = -det3(dat[1], dat[2], dat[3], dat[5], dat[6], dat[7], dat[9], dat[10], dat[11]) / _det;

	res.dat[4] = -det3(dat[4], dat[6], dat[7], dat[8], dat[10], dat[11], dat[12], dat[14], dat[15]) / _det;
	res.dat[5] = det3(dat[0], dat[2], dat[3], dat[8], dat[10], dat[11], dat[12], dat[14], dat[15]) / _det;
	res.dat[6] = -det3(dat[0], dat[2], dat[3], dat[4], dat[6], dat[7], dat[12], dat[14], dat[15]) / _det;
	res.dat[7] = det3(dat[0], dat[2], dat[3], dat[4], dat[6], dat[7], dat[8], dat[10], dat[11]) / _det;

	res.dat[8] = det3(dat[4], dat[5], dat[7], dat[8], dat[9], dat[11], dat[12], dat[13], dat[15]) / _det;
	res.dat[9] = -det3(dat[0], dat[1], dat[3], dat[8], dat[9], dat[11], dat[12], dat[13], dat[15]) / _det;
	res.dat[10] = det3(dat[0], dat[1], dat[3], dat[4], dat[5], dat[7], dat[12], dat[13], dat[15]) / _det;
	res.dat[11] = -det3(dat[0], dat[1], dat[3], dat[4], dat[5], dat[7], dat[8], dat[9], dat[11]) / _det;

	res.dat[12] = -det3(dat[4], dat[5], dat[6], dat[8], dat[9], dat[10], dat[12], dat[13], dat[14]) / _det;
	res.dat[13] = det3(dat[0], dat[1], dat[2], dat[8], dat[9], dat[10], dat[12], dat[13], dat[14]) / _det;
	res.dat[14] = -det3(dat[0], dat[1], dat[2], dat[4], dat[5], dat[6], dat[12], dat[13], dat[14]) / _det;
	res.dat[15] = det3(dat[0], dat[1], dat[2], dat[4], dat[5], dat[6], dat[8], dat[9], dat[10]) / _det;

	//standardization
	for (auto i = 0; i < 16; i++)
		if (res.dat[i] == -0.0f)
			res.dat[i] = 0.0f;

	return res;
}

template<typename M>
inline glMat4<M> glMat4<M>::operator=(const glMat4<M> mat)
{
	memcpy(dat, mat.dat, sizeof(M) * 16);
	return *this;
}

template<typename M>
inline glMat4<M> glMat4<M>::operator+(const glMat4<M> mat)
{
	glMat4<M> res;
	for (auto i = 0; i < 16; i++)
		res.dat[i] = dat[i] + mat.dat[i];
	return res;
}

template<typename M>
inline glMat4<M> glMat4<M>::operator-(const glMat4<M> mat)
{
	glMat4<M> res;
	for (auto i = 0; i < 16; i++)
		res.dat[i] = dat[i] - mat.dat[i];
	return res;
}

template<typename M>
inline glMat4<M> glMat4<M>::operator*(glMat4<M> mat)
{
	glMat4<M> res;
	//when M == float
	//use __mm256 accelerate calculation
	//using AVX & AVX2
	if (std::is_same<std::decay<M>::type, float>::value)
		//float
	{
		__declspec(align(16)) __m256 r11, r22, r33, r44, c12, c34, tmp;
		r11 = _mm256_i32gather_ps((float*)dat, row1d4, sizeof(float));
		r22 = _mm256_i32gather_ps((float*)dat, row2d4, sizeof(float));
		r33 = _mm256_i32gather_ps((float*)dat, row3d4, sizeof(float));
		r44 = _mm256_i32gather_ps((float*)dat, row4d4, sizeof(float));

		c12 = _mm256_i32gather_ps((float*)(mat.dat), column124, sizeof(float));
		c34 = _mm256_i32gather_ps((float*)(mat.dat), column344, sizeof(float));
		
		tmp = _mm256_dp_ps(r11, c12, __SIMD_MASK__);
		res.dat[0] = tmp.m256_f32[4];
		res.dat[4] = tmp.m256_f32[0];
		tmp = _mm256_dp_ps(r11, c34, __SIMD_MASK__);
		res.dat[8] = tmp.m256_f32[4];
		res.dat[12] = tmp.m256_f32[0];
		tmp = _mm256_dp_ps(r22, c12, __SIMD_MASK__);
		res.dat[1] = tmp.m256_f32[4];
		res.dat[5] = tmp.m256_f32[0];
		tmp = _mm256_dp_ps(r22, c34, __SIMD_MASK__);
		res.dat[9] = tmp.m256_f32[4];
		res.dat[13] = tmp.m256_f32[0];
		tmp = _mm256_dp_ps(r33, c12, __SIMD_MASK__);
		res.dat[2] = tmp.m256_f32[4];
		res.dat[6] = tmp.m256_f32[0];
		tmp = _mm256_dp_ps(r33, c34, __SIMD_MASK__);
		res.dat[10] = tmp.m256_f32[4];
		res.dat[14] = tmp.m256_f32[0];
		tmp = _mm256_dp_ps(r44, c12, __SIMD_MASK__);
		res.dat[3] = tmp.m256_f32[4];
		res.dat[7] = tmp.m256_f32[0];
		tmp = _mm256_dp_ps(r44, c34, __SIMD_MASK__);
		res.dat[11] = tmp.m256_f32[4];
		res.dat[15] = tmp.m256_f32[0];
	}
	else
	{
		res.dat[0] = dat[0] * mat.dat[0] + dat[4] * mat.dat[1] + dat[8] * mat.dat[2] + dat[12] * mat.dat[3];
		res.dat[1] = dat[1] * mat.dat[0] + dat[5] * mat.dat[1] + dat[9] * mat.dat[2] + dat[13] * mat.dat[3];
		res.dat[2] = dat[2] * mat.dat[0] + dat[6] * mat.dat[1] + dat[10] * mat.dat[2] + dat[14] * mat.dat[3];
		res.dat[3] = dat[3] * mat.dat[0] + dat[7] * mat.dat[1] + dat[11] * mat.dat[2] + dat[15] * mat.dat[3];

		res.dat[4] = dat[0] * mat.dat[4] + dat[4] * mat.dat[5] + dat[8] * mat.dat[6] + dat[12] * mat.dat[7];
		res.dat[5] = dat[1] * mat.dat[4] + dat[5] * mat.dat[5] + dat[9] * mat.dat[6] + dat[13] * mat.dat[7];
		res.dat[6] = dat[2] * mat.dat[4] + dat[6] * mat.dat[5] + dat[10] * mat.dat[6] + dat[14] * mat.dat[7];
		res.dat[7] = dat[3] * mat.dat[4] + dat[7] * mat.dat[5] + dat[11] * mat.dat[6] + dat[15] * mat.dat[7];

		res.dat[8] = dat[0] * mat.dat[8] + dat[4] * mat.dat[9] + dat[8] * mat.dat[10] + dat[12] * mat.dat[11];
		res.dat[9] = dat[1] * mat.dat[8] + dat[5] * mat.dat[9] + dat[9] * mat.dat[10] + dat[13] * mat.dat[11];
		res.dat[10] = dat[2] * mat.dat[8] + dat[6] * mat.dat[9] + dat[10] * mat.dat[10] + dat[14] * mat.dat[11];
		res.dat[11] = dat[3] * mat.dat[8] + dat[7] * mat.dat[9] + dat[11] * mat.dat[10] + dat[15] * mat.dat[11];

		res.dat[12] = dat[0] * mat.dat[12] + dat[4] * mat.dat[13] + dat[8] * mat.dat[14] + dat[12] * mat.dat[15];
		res.dat[13] = dat[1] * mat.dat[12] + dat[5] * mat.dat[13] + dat[9] * mat.dat[14] + dat[13] * mat.dat[15];
		res.dat[14] = dat[2] * mat.dat[12] + dat[6] * mat.dat[13] + dat[10] * mat.dat[14] + dat[14] * mat.dat[15];
		res.dat[15] = dat[3] * mat.dat[12] + dat[7] * mat.dat[13] + dat[11] * mat.dat[14] + dat[15] * mat.dat[15];
	}

	return res;
}

template<typename M>
inline glMat4<M> glMat4<M>::operator*(const M num)
{
	glMat4<M> res = *this;
	for (auto i = 0; i < 16; i++)
		res.dat[i] *= num;
	return res;
}

template<typename M>
glMat4<M> operator*(const M num, glMat4<M> mat)
{
	return mat * num;
}

template<typename M>
inline M* glMat4<M>::operator[](const uint32_t iter)
{
	if (iter < 4u)
		return dat + iter * 4u;
	else
		return nullptr;
}



template<typename M>
glvector4<M> operator*(glMat4<M> mat, glvector4<M> vec4)
{
	glvector4<M> res;
	res[0] = mat[0][0] * vec4[0] + mat[1][0] * vec4[1] + mat[2][0] * vec4[2] + mat[3][0] * vec4[3];
	res[1] = mat[0][1] * vec4[0] + mat[1][1] * vec4[1] + mat[2][1] * vec4[2] + mat[3][1] * vec4[3];
	res[2] = mat[0][2] * vec4[0] + mat[1][2] * vec4[1] + mat[2][2] * vec4[2] + mat[3][2] * vec4[3];
	res[3] = mat[0][3] * vec4[0] + mat[1][3] * vec4[1] + mat[2][3] * vec4[2] + mat[3][3] * vec4[3];
	return res;
}

template<typename M>
glvector4<M> operator*(glvector4<M> vec4, glMat4<M> mat)
{
	glvector4<M> res;
	res[0] = mat[0][0] * vec4[0] + mat[0][1] * vec4[1] + mat[0][2] * vec4[2] + mat[0][3] * vec4[3];
	res[1] = mat[1][0] * vec4[0] + mat[1][1] * vec4[1] + mat[1][2] * vec4[2] + mat[1][3] * vec4[3];
	res[2] = mat[2][0] * vec4[0] + mat[2][1] * vec4[1] + mat[2][2] * vec4[2] + mat[2][3] * vec4[3];
	res[3] = mat[3][0] * vec4[0] + mat[3][1] * vec4[1] + mat[3][2] * vec4[2] + mat[3][3] * vec4[3];
	return res;
}

template<typename M>
glMat4<M> operator-(glMat4<M> mat)
{
	mat.col_1 = - mat.col_1;
	mat.col_2 = - mat.col_2;
	mat.col_3 = - mat.col_3;
	mat.col_0 = - mat.col_0;
}

#endif


#ifndef __GL_MATH_7__
#define __GL_MATH_7__
template<typename M>
inline glMat3<M>::glMat3()
{
	memset(dat, 0, sizeof(dat));
	dat[0] = 1.0f;
	dat[4] = 1.0f;
	dat[8] = 1.0f;
}

template<typename M>
inline glMat3<M>::glMat3(uint32_t sz, M* _dat)
{
	if (sz != 36)
		memset(dat, 0, sizeof(dat));
	else
		memcpy(dat, _dat, sz);
}

template<typename M>
inline glMat3<M>::glMat3(const glMat4<M>& mat)
{
	for (auto i = 0; i < 9; i++)
		dat[i] = mat[i / 3][i % 3];
}

template<typename M>
inline glMat3<M>::glMat3(const glMat3<M>& mat)
{
	memcpy(dat, mat.dat, sizeof(dat));
}

template<typename M>
inline M glMat3<M>::det()
{
	M res = dat[0] * (dat[4] * dat[8] - dat[5] * dat[7]) - dat[3] * (dat[1] * dat[8] - dat[2] * dat[7]) + dat[6] * (dat[1] * dat[5] - dat[2] * dat[4]);
	return res;
}

template<typename M>
inline glMat3<M> glMat3<M>::transpos()
{
	glMat3<M> res = *this;
	exchange(res.dat[1], res.dat[3]);
	exchange(res.dat[2], res.dat[6]);
	exchange(res.dat[5], res.dat[7]);
	return res;
}

template<typename M>
inline glMat3<M> glMat3<M>::inverse()
{
	glMat3<M> res;
	M _det = det();
	if (!_det)
		return res;

	auto det2 = [](M m00, M m10, M m01, M m11)->M {return (m00 * m11 - m01 * m10); };

	res.dat[0] = det2(dat[4], dat[5], dat[7], dat[8]) / _det;
	res.dat[1] = det2(dat[1], dat[2], dat[7], dat[8]) / _det;
	res.dat[2] = det2(dat[1], dat[2], dat[4], dat[5]) / _det;

	res.dat[3] = det2(dat[3], dat[5], dat[6], dat[8]) / _det;
	res.dat[4] = det2(dat[0], dat[2], dat[6], dat[8]) / _det;
	res.dat[5] = det2(dat[0], dat[1], dat[6], dat[7]) / _det;

	res.dat[6] = det2(dat[3], dat[4], dat[6], dat[7]) / _det;
	res.dat[7] = det2(dat[0], dat[1], dat[6], dat[7]) / _det;
	res.dat[8] = det2(dat[0].dat[1], dat[3], dat[4]) / _det;

	return res;
}

template<typename M>
inline glMat3<M> glMat3<M>::operator=(const glMat3<M> mat)
{
	memcpy(dat, mat.dat, 9 * sizeof(M));
	return *this;
}

template<typename M>
inline glMat3<M> glMat3<M>::operator+(const glMat3<M> mat)
{
	glMat3<M> res;
	for (auto i = 0; i < 9; i++)
		res.dat[i] = dat[i] + mat.dat[i];
	return res;
}

template<typename M>
inline glMat3<M> glMat3<M>::operator-(const glMat3<M> mat)
{
	glMat3<M> res;
	for (auto i = 0; i < 9; i++)
		res.dat[i] = dat[i] - mat.dat[i];
	return res;
}

template<typename M>
inline glMat3<M> glMat3<M>::operator*(glMat3<M> mat)
{
	glMat3<M> res;

	res.dat[0] = dat[0] * mat.dat[0] + dat[3] * mat.dat[1] + dat[6] * mat.dat[2];
	res.dat[1] = dat[1] * mat.dat[0] + dat[4] * mat.dat[1] + dat[7] * mat.dat[2];
	res.dat[2] = dat[2] * mat.dat[0] + dat[5] * mat.dat[1] + dat[8] * mat.dat[2];

	res.dat[3] = dat[0] * mat.dat[3] + dat[3] * mat.dat[4] + dat[6] * mat.dat[5];
	res.dat[4] = dat[1] * mat.dat[3] + dat[4] * mat.dat[4] + dat[7] * mat.dat[5];
	res.dat[5] = dat[2] * mat.dat[3] + dat[5] * mat.dat[4] + dat[8] * mat.dat[5];

	res.dat[6] = dat[0] * mat.dat[6] + dat[3] * mat.dat[7] + dat[6] * mat.dat[8];
	res.dat[7] = dat[1] * mat.dat[6] + dat[4] * mat.dat[7] + dat[7] * mat.dat[8];
	res.dat[8] = dat[2] * mat.dat[6] + dat[5] * mat.dat[7] + dat[8] * mat.dat[8];

	return res;
}

template<typename M>
inline glMat3<M> glMat3<M>::operator*(const M num)
{
	glMat3<M> res = *this;
	for (auto i = 0; i < 9; i++)
		res.dat[i] *= num;
	return res;
}

template<typename M>
glMat3<M> operator*(const M num, glMat3<M> mat)
{
	return mat * num;
}

template<typename M>
inline M* glMat3<M>::operator[](const uint32_t iter)
{
	if (iter < 4)
		return dat + iter * 3;
	else
		return nullptr;
}

template<typename M>
glvector3<M> operator*(glMat3<M> mat, glvector3<M> vec3)
{
	glvector3<M> res;
	res[0] = mat[0][0] * vec3[0] + mat[1][0] * vec3[1] + mat[2][0] * vec3[2];
	res[1] = mat[0][1] * vec3[0] + mat[1][1] * vec3[1] + mat[2][1] * vec3[2];
	res[2] = mat[0][2] * vec3[0] + mat[1][2] * vec3[1] + mat[2][2] * vec3[2];
	return res;
}
template<typename M>
glvector3<M> operator*(glvector3<M> vec3, glMat3<M> mat)
{
	glvector3<M> res;
	res[0] = mat[0][0] * vec3[0] + mat[0][1] * vec3[1] + mat[0][2] * vec3[2];
	res[1] = mat[1][0] * vec3[0] + mat[1][1] * vec3[1] + mat[1][2] * vec3[2];
	res[2] = mat[2][0] * vec3[0] + mat[2][1] * vec3[1] + mat[2][2] * vec3[2];
	return res;
}

template<typename M>
glMat3<M> operator-(glMat3<M> mat)
{
	mat.col_1 = -mat.col_1;
	mat.col_2 = -mat.col_2;
	mat.col_0 = -mat.col_0;

	return mat;
}

#endif


#ifndef __GL_MATH_UTIL__
#define __GL_MATH_UTIL__
namespace ZaxicMathUtil
{
#define ZAXIC_PIf 3.1416f
#define ZAXIC_PI  3.1416

	using mat4f = glMat4<float>;
	using mat4d = glMat4<double>;
	using mat3f = glMat3<float>;
	using mat3d = glMat3<double>;

	using vec4f = glvector4<float>;
	using vec4d = glvector4<double>;
	using vec3f = glvector3<float>;
	using vec3d = glvector3<double>;

	mat4f GenRotationMatrix(vec3f& axis, const float radians);
	mat4f GenTranslationMatrix(vec3f& mvec);
	mat4f GenScalizeMatrix(vec3f& scale);

	float toRadians(float angle);
	float toTheta(float radians);
}

#endif

#ifndef __GL_MATH_EULER__
#define __GL_MATH_EULER__

enum class EulerMember
{
	yaw,
	pitch,
	roll
};

class glEuler
{
public:
	union
	{
		glvector3<float> v;
		struct
		{
			float yaw, pitch, roll;
		};
	};
private:
	void norm();
public:
	glEuler(float _yaw, float _pitch, float _roll);
	glEuler(ZaxicMathUtil::vec3f v3);
	glEuler(const glEuler& el);
	glEuler(glMat4<float>& mat);

	glEuler operator=(const glEuler el);
	glEuler operator=(glvector3<float> v);

	void set(EulerMember em, float vl);
	double get(EulerMember em);

	glMat4<float> EulerMatrix();

	~glEuler() {}
};

#endif

#ifndef __GL_MATH_QUATERNION__
#define __GL_MATH_QUATERNION__
class glQuaternion
{
private:
	void _norm();
public:
	union 
	{
		glvector4<float> v4;
		struct
		{
			float w;
			glvector3<float> v3;
		};
		struct
		{
			float w, x, y, z;
		};
	};

	glQuaternion();
	
	glQuaternion(const glQuaternion& quat);
	glQuaternion(glEuler& euler);
	glQuaternion(glMat4<float>& mat);
	glQuaternion(glvector4<float> v);
	glQuaternion(float _w, glvector3<float> v);
	glQuaternion(glvector3<float>& axis, float radians);
	glQuaternion operator=(glQuaternion quat);

	glQuaternion norm();

	glQuaternion operator*(glQuaternion quat);
	glQuaternion operator-(glQuaternion quat);
	glQuaternion inverse();
	glMat4<float> toMatrix();
	
	float len();

	float dot(glQuaternion quat);
	
	~glQuaternion() {}
};


glQuaternion SLERP(glQuaternion q1, glQuaternion q2, float alpha);
glQuaternion operator*(float n, glQuaternion q);
glQuaternion operator*(glQuaternion q, float n);
glEuler toglEuler(glQuaternion& quat);
#endif