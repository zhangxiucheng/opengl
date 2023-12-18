#define __ZAXIC_GL_DATA__
#define __ZAXIC_GL_ERROR_PROC__
#include"glFrameWork.h"
mlog* g_log = nullptr;
#include<glm/gtc/quaternion.hpp>
#include<glm/gtx/quaternion.hpp>

void mlog::loadprofile()
{
	std::ifstream ifs("./profile/log.profile", std::ios_base::in);
	if (!ifs.is_open())
	{
		//openerror
		std::cout << "Error!\n" << "./profile/log.profile" << " open error!" << std::endl;
		exit(-1);
	}
	ifs >> seq >> count >> logpath;
	ifs.close();
}

void mlog::_newlog()
{
	std::string t = logpath.c_str() + std::to_string(++seq);
	count = 0;
	if (ofs.is_open())
		ofs.close();
	ofs.open(t.c_str(), std::ios_base::app);
	if (!ofs.is_open())
	{
		//openerror
		std::cout << "Error!\n" << t << " open error!" << std::endl;
		exit(-1);
	}
}

mlog::mlog()
{
	if (g_log != nullptr)
		return;
	else
		g_log = this;

	loadprofile();
	if (count >= MAXLOGCOUNT)
		_newlog();
	else
	{
		std::string t = logpath.c_str() + std::to_string(seq);
		ofs.open(t.c_str(), std::ios_base::app);
		if (!ofs.is_open())
		{
			//openerror
			std::cout << "Error!\n" << t << " open error!" << std::endl;
			exit(-1);
		}
	}
}

void mlog::check(glComponentType tp, GLenum pname, glID pid)
{
	char errinfo[512];
	int success;
	if (tp == glComponentType::SHADER)
	{
		glGetShaderiv(pid, pname, &success);
		if (!success)
		{
			glGetShaderInfoLog(pid, 512, NULL, errinfo);
			std::string s(errinfo);
			newlog(s);
			//exit(-1);
		}
	}
	else 
	{
		glGetProgramiv(pid, pname, &success);
		if (!success)
		{
			glGetProgramInfoLog(pid, 512, NULL, errinfo);
			std::string s(errinfo);
			newlog(s);
			//exit(-1);
		}
	}
}

void mlog::newlog(std::string& content)
{
	if (ofs.is_open())
	{
		++count;
		ofs << content.c_str() << getTime() << '\n';
		if (count >= MAXLOGCOUNT)
			_newlog();
	}
}

mlog::~mlog()
{
	if (ofs.is_open())
		ofs.close();
	ofs.open("./profile/log.profile", std::ios_base::out);
	if (!ofs.is_open())
	{
		//openerror
		std::cout << "Error!\n" << "./profile/log.profile" << " open error!" << std::endl;
		exit(-1);
	}
	ofs << seq << '\n' << count << '\n' << logpath;
	ofs.close();
}

std::string getTime()
{
	time_t _tm;
	time(&_tm);
	char buf[64];
	strftime(buf, 64, "%Y-%m-%d %H:%M:%S", localtime(&_tm));
	return buf;
}

std::vector<glm::vec3> glGenTrajectory(glTrajectoryType tt, uint32_t count, glm::vec3 st, glm::vec3 ed)
{

	std::vector<glm::vec3> vertices;
	glm::vec3 p[2];
	float r = ZaxicMathUtil::vec3f(3*sizeof(float), glm::value_ptr(st - ed)).veclength();
	glm::vec3 v1(st.x, st.y, ed.z), v2(ed.x, st.y, st.z);
	float delta, delta_x, delta_z;
	auto iter = glIterFirst;
	switch (tt)
	{
	case glTrajectoryType::RING:
		delta = 360.0f / float(count);
		iter = glIterFirst;
		p[iter] = glm::vec3(r + st.x, st.y, st.z);
		for (auto i = 1; i < count; i++)
		{
			p[glAnotherIter(iter)] = glm::vec3(cos(glm::radians(delta * i)) * r + st.x, st.y, sin(glm::radians(delta * i)) * r + st.z);
			vertices.push_back(p[iter]);
			vertices.push_back(p[glSwitchIter(iter)]);
		}
		vertices.push_back(p[iter]);
		vertices.push_back(glm::vec3(r + st.x, st.y, st.z));
		break;
	case glTrajectoryType::RECTANGLE:
		iter = glIterFirst;
		p[iter] = st;
		delta = (v1.z - st.z) / float(count);
		for (auto i = 1; i < count; i++)
		{
			p[glAnotherIter(iter)] = glm::vec3(st.x, st.y, st.z + i * delta);
			vertices.push_back(p[iter]);
			vertices.push_back(p[glSwitchIter(iter)]);
		}
		vertices.push_back(p[iter]);
		vertices.push_back(v1);

		iter = glIterFirst;
		p[iter] = v1;
		delta = (ed.x - v1.x) / float(count);
		for (auto i = 1; i < count; i++)
		{
			p[glAnotherIter(iter)] = glm::vec3(v1.x + i * delta, st.y, v1.z);
			vertices.push_back(p[iter]);
			vertices.push_back(p[glSwitchIter(iter)]);
		}
		vertices.push_back(p[iter]);
		vertices.push_back(ed);

		iter = glIterFirst;
		p[iter] = ed;
		delta = (v2.z - ed.z) / float(count);
		for (auto i = 1; i < count; i++)
		{
			p[glAnotherIter(iter)] = glm::vec3(ed.x, st.y, ed.z + i * delta);
			vertices.push_back(p[iter]);
			vertices.push_back(p[glSwitchIter(iter)]);
		}
		vertices.push_back(p[iter]);
		vertices.push_back(v2);

		iter = glIterFirst;
		p[iter] = v2;
		delta = (st.x - v2.x) / float(count);
		for (auto i = 1; i < count; i++)
		{
			p[glAnotherIter(iter)] = glm::vec3(v2.x + i * delta, st.y, v2.z);
			vertices.push_back(p[iter]);
			vertices.push_back(p[glSwitchIter(iter)]);
		}
		vertices.push_back(p[iter]);
		vertices.push_back(st);

		break;
	case glTrajectoryType::LINE:
		delta_x = (ed.x - st.x) / float(count);
		delta_z = (ed.z - st.z) / float(count);
		iter = glIterFirst;
		p[iter] = st;
		for (auto i = 1; i < count; i++)
		{
			p[glAnotherIter(iter)] = glm::vec3(st.x + i * delta_x, st.y, st.z + i * delta_z);
			vertices.push_back(p[iter]);
			vertices.push_back(p[glSwitchIter(iter)]);
		}
		break;
	default:
		break;
	}

	return vertices;
}

void mathtest_vec3f()
{
	float num[3];
	std::ifstream ifs("./test.txt", std::ios_base::in);
	std::ofstream ofs("./out.txt", std::ios_base::out);
	char order[512];
	char trush;
	if (ifs.is_open())
	{
		ofs.setf(std::ios::showpoint);
		ofs.setf(std::ios::fixed);
		ofs << std::setprecision(2);
		while (!ifs.eof())
		{
			ifs >> order;
			ifs >> num[0] >> trush >> num[1] >> trush >> num[2];
			ZaxicMathUtil::vec3f v1(sizeof(num), num);
			if (!strcmp(order, "向量标准化"))
			{
				ofs << order << '\n' << v1[0] << ',' << v1[1] << ',' << v1[2] << '\t';
				v1 = v1.norm();
				ofs << v1[0] << ',' << v1[1] << ',' << v1[2] << '\n';
				continue;
			}
			else if (!strcmp(order, "向量求模"))
			{
				ofs << order << '\n' << v1[0] << ',' << v1[1] << ',' << v1[2] << '\t';
				num[0] = v1.veclength();
				ofs << num[0] << '\n';
				continue;
			}
			else if (!strcmp(order, "向量数乘"))
			{
				ofs << order << '\n' << v1[0] << ',' << v1[1] << ',' << v1[2] << '\t';
				ifs >> num[0];
				v1 = v1.scalarMul(num[0]);
				ofs << num[0] << v1[0] << ',' << v1[1] << ',' << v1[2] << '\n';
				continue;
			}
			ifs >> num[0] >> trush >> num[1] >> trush >> num[2];
			ZaxicMathUtil::vec3f v2(sizeof(num), num);
			ofs << order << '\n' << v1[0] << ',' << v1[1] << ',' << v1[2] << '\t' << v2[0] << ',' << v2[1] << ',' << v2[2] << '\t';
			if (!strcmp(order, "向量相加"))
			{
				v2 = v1 + v2;
				ofs << v2[0] << ',' << v2[1] << ',' << v2[2] << '\n';
			}
			else if (!strcmp(order, "向量点乘"))
			{
				num[0] = v1 * v2;
				ofs << num[0] << '\n';
			}
			else if (!strcmp(order, "向量叉乘"))
			{
				v2 = v1.crossproduct(v2);
				ofs << v2[0] << ',' << v2[1] << ',' << v2[2] << '\n';
			}
			else if (!strcmp(order, "向量投影"))
			{
				v2 = v1.proj(v2);
				ofs << v2[0] << ',' << v2[1] << ',' << v2[2] << '\n';
			}
		}
		ifs.close();
	}
	ofs.close();
}

void matTest()
{
	glMat3<float> m;
	m = -m;
	float num[16];
	std::ifstream ifs("./test.txt", std::ios_base::in);
	std::ofstream ofs("./out.txt", std::ios_base::out);
	char order[512];
	char trush;
	if (ifs.is_open())
	{
		ofs.setf(std::ios::showpoint);
		ofs.setf(std::ios::fixed);
		ofs << std::setprecision(2);
		auto out = [](float* dat, int n, std::ostream& ofs)
		{
			for (auto i = 0; n; i++, n--)
				if (n > 1)
					ofs << dat[i] << ',';
				else
					ofs << dat[i];
		};
		auto in = [_num = num](std::istream& ifs, int n)
		{
			char trush;
			for (auto i = 0; n; n--, i++)
				if (n > 1)
					ifs >> _num[i] >> trush;
				else
					ifs >> _num[i];
		};
		while (!ifs.eof())
		{
			ifs >> order;
			ofs << order << '\n';
			if (!strcmp(order, "矩阵设置旋转"))
			{
				in(ifs, 1);
				out(num, 1, ofs);
				ofs << '\t';
				num[3] = num[0];
				in(ifs, 3);
				out(num, 3, ofs);
				ofs << '\t';
				ZaxicMathUtil::vec3f v(12, num);
				ZaxicMathUtil::mat4f mat1 = ZaxicMathUtil::GenRotationMatrix(v, ZaxicMathUtil::toRadians(num[3]));
				out(mat1.dat, 16, ofs);
				ofs << '\n';
				continue;
			}
			else if (!strcmp(order, "矩阵设置平移"))
			{
				in(ifs, 3);
				out(num, 3, ofs);
				ofs << '\t';
				ZaxicMathUtil::vec3f v(12, num);
				ZaxicMathUtil::mat4f mat1 = ZaxicMathUtil::GenTranslationMatrix(v);
				out(mat1.dat, 16, ofs);
				ofs << '\n';
				continue;
			}
			else if (!strcmp(order, "矩阵设置缩放"))
			{
				in(ifs, 3);
				out(num, 3, ofs);
				ofs << '\t';
				ZaxicMathUtil::vec3f v(12, num);
				ZaxicMathUtil::mat4f mat1 = ZaxicMathUtil::GenScalizeMatrix(v);
				out(mat1.dat, 16, ofs);
				ofs << '\n';
				continue;
			}
			in(ifs, 16);
			ZaxicMathUtil::mat4f mat1(sizeof(num), num);
			out(mat1.dat, 16, ofs);
			if (!strcmp(order, "矩阵数乘"))
			{
				ifs >> num[0];
				ofs << '\t' << num[0] << '\t';
				mat1 = mat1 * num[0];
				out(mat1.dat, 16, ofs);
				ofs << '\n';
				continue;
			}
			else if (!strcmp(order, "矩阵乘向量"))
			{
				in(ifs, 3);
				ofs << '\t';
				out(num, 3, ofs);
				ofs << '\t';
				num[3] = 0;
				ZaxicMathUtil::vec4f v(16, num);
				v = mat1 * v;
				out(v.get(), 3, ofs);
				ofs << '\n';
				continue;
			}
			else if (!strcmp(order, "矩阵乘位置"))
			{
				in(ifs, 3);
				ofs << '\t';
				out(num, 3, ofs);
				ofs << '\t';
				num[3] = 1;
				ZaxicMathUtil::vec4f v(16, num);
				v = mat1 * v;
				out(v.get(), 3, ofs);
				ofs << '\n';
				continue;
			}
			else if (!strcmp(order, "矩阵求逆"))
			{
				ofs << '\t';
				mat1 = mat1.inverse();
				out(mat1.dat, 16, ofs);
				ofs << '\n';
				continue;
			}
			else if (!strcmp(order, "矩阵相乘"))
			{
				in(ifs, 16);
				ZaxicMathUtil::mat4f mat2(64, num);
				mat1 = mat1 * mat2;
				ofs << '\t';
				out(mat2.dat, 16, ofs);
				ofs << '\t';
				out(mat1.dat, 16, ofs);
				ofs << '\n';
				continue;
			}
		}
		ifs.close();
	}
	ofs.close();
}

void eulquatTest()
{
	float num[16];
	char trush;
	std::ifstream ifs("./test.txt", std::ios_base::in);
	std::ofstream ofs("./out.txt", std::ios_base::out);
	//ofs.setf(std::ios::showpoint);
	ofs.setf(std::ios::fixed);
	ofs << std::setprecision(2);
	auto _in = [tg = num,&tr = trush,&_ifs = ifs](int c)
	{
		for (auto i = 0; i < c; i++)
			if (i == c - 1)
				_ifs >> tg[i];
			else
			{
				_ifs >> tg[i];
				_ifs >> tr;
			}
	};
	auto _out = [&of = ofs](int c, float* tg)
	{
		//of << std::setprecision(0);
		of.unsetf(std::ios::showpoint);
		for (auto i = 0; i < c; i++)
			if (i == c - 1)
				if (ZAXIC_FLOAT_SAME(tg[i], -0.0f))
					of << 0.0f;
				else
					of << tg[i];
			else
				if (ZAXIC_FLOAT_SAME(tg[i], -0.0f))
					of << 0.0f << ',';
				else
					of << tg[i] << ',';
	};

	auto outMat = [&of = ofs](float* val_ptr)
	{
		of.setf(std::ios::showpoint);
		//of << std::setprecision(2);
		for (auto i = 0; i < 4; i++)
			for (auto j = 0; j < 4; j++)
				if (i == 3 && j == 3)
					of << val_ptr[j * 4 + i];
				else
					of << val_ptr[j * 4 + i] << ',';
	};

	auto outQuat = [&of = ofs](float* p)
	{
		of.setf(std::ios::showpoint);
		of << p[1] << ',' << p[2] << ',' << p[3] << ',' << p[0];
	};
	
	auto out = [&of = ofs](int c, float* tg)
	{
		of.setf(std::ios::showpoint);
		for (auto i = 0; i < c; i++)
			if (i == c - 1)
				if (ZAXIC_FLOAT_SAME(tg[i], -0.0f))
					of << 0.0f;
				else
					of << tg[i];
			else
				if (ZAXIC_FLOAT_SAME(tg[i], -0.0f))
					of << 0.0f << ',';
				else
					of << tg[i] << ',';
	};

	char order[512];
	if (ifs.is_open())
	{
		while (!ifs.eof())
		{
			ifs >> order;
			ofs << order << std::endl;
			if (!strcmp(order, "欧拉角转换向量"))
			{
				_in(3);
				glEuler el(num[0], num[1], num[2]);
				ZaxicMathUtil::vec4f v(0.0f, 0.0f, -1.0f, 0.0f);
				v = el.EulerMatrix() * v;
				//glm::vec3 v(num[0], num[1], num[2]);
				
				//glvector3<float> v(cosf(ZaxicMathUtil::toRadians(num[1])) * sinf(ZaxicMathUtil::toRadians(num[0])), sinf(ZaxicMathUtil::toRadians(num[1])), cosf(ZaxicMathUtil::toRadians(num[1])) * cosf(ZaxicMathUtil::toRadians(num[0])));
				_out(3, num);
				ofs << '\t';
				out(3, v.num);
			}
			else if (!strcmp(order, "向量转换欧拉角"))
			{
				_in(3);
				glvector3<float> v(3 * sizeof(float), num);
				v = v.norm();
				num[4] = asinf(v.y);
				v.y = 0.0f;
				v = v.norm();
				ZaxicMathUtil::vec3f n(0.0f, 0.0f, -1.0f);
				num[3] = acosf(n * v);
				if (((v.x > 0.0f)&&(num[3]>=0.0f))|| ((v.x < 0.0f) && (num[3] <= 0.0f)))
					num[3] = -num[3];

				num[3] = ZaxicMathUtil::toTheta(num[3]);
				num[4] = ZaxicMathUtil::toTheta(num[4]);
				num[5] = 0.0f;
				_out(3, num);
				ofs << '\t';
				out(3, num + 3);
			}
			else if (!strcmp(order, "欧拉角转换四元数"))
			{
				_in(3);
				glEuler el(num[0], num[1], num[2]);
				glQuaternion q(el);
				_out(3, num);
				ofs << '\t';
				outQuat(q.v4.num);
			}
			else if (!strcmp(order, "四元数转换欧拉角"))
			{
				_in(4);
				glQuaternion q;
				q.w = num[3];
				q.x = num[0];
				q.y = num[1];
				q.z = num[2];
				glEuler el = toglEuler(q);
				_out(4, num);
				ofs << '\t';
				out(3, el.v.num);
			}
			else if (!strcmp(order, "欧拉角转换矩阵"))
			{
				_in(3);
				glEuler el(num[0], num[1], num[2]);
				_out(3, num);
				ofs << '\t';
				glMat4<float> mat = el.EulerMatrix();
				outMat(mat.dat);
			}
			else if (!strcmp(order, "矩阵转换欧拉角"))
			{
				_in(16);
				glMat4<float> mat(sizeof(num), num);
				glEuler el(mat);
				_out(16,mat.dat);
				ofs << '\t';
				out(3, el.v.num);
			}
			else if (!strcmp(order, "矩阵转换四元数"))
			{
				_in(16);
				glMat4<float> mat(sizeof(num), num);
				//glm::quat q(glm::mat4());
				glQuaternion q(mat);
				_out(16, mat.dat);
				ofs << '\t';
				outQuat(q.v4.num);
			}
			else if (!strcmp(order, "四元数转换矩阵"))
			{
				_in(4);
				glQuaternion q;
				q.w = num[3];
				q.x = num[0];
				q.y = num[1];
				q.z = num[2];
				_out(4, num);
				ofs << '\t';
				glMat4<float> mat = q.toMatrix();
				outMat(mat.dat);
			}
			else if (!strcmp(order, "四元数单位化"))
			{
				_in(4);
				glQuaternion q;
				q.w = num[3];
				q.x = num[0];
				q.y = num[1];
				q.z = num[2];
				_out(4, num);
				q = q.norm();
				ofs << '\t';
				outQuat(q.v4.num);
			}
			else if (!strcmp(order, "欧拉角标准化"))
			{
				_in(3);
				_out(3, num);
				ofs << '\t';
				for (auto i=0;i<3;i++)
					while (!((num[i] <= 180.0f) && (num[i] >= -180.0f)))
					{
						if (num[i] > 180.0f)
							num[i] -= 360.0f;
						else if (num[i] < -180.0f)
							num[i] += 360.0f;
					}
				out(3, num);
			}
			else if (!strcmp(order, "四元数相乘"))
			{
				_in(4);
				_out(4, num);
				ofs << '\t';
				glQuaternion q1, q2;
				q1.w = num[3];
				q1.x = num[0];
				q1.y = num[1];
				q1.z = num[2];
				_in(4);
				_out(4, num);
				ofs << '\t';
				q2.w = num[3];
				q2.x = num[0];
				q2.y = num[1];
				q2.z = num[2];
				q1 = q1 * q2;
				outQuat(q1.v4.num);
			}
			else if (!strcmp(order, "四元数求差"))
			{
				_in(4);
				_out(4, num);
				ofs << '\t';
				glQuaternion q1, q2;
				q1.w = num[3];
				q1.x = num[0];
				q1.y = num[1];
				q1.z = num[2];
				_in(4);
				_out(4, num);
				ofs << '\t';
				q2.w = num[3];
				q2.x = num[0];
				q2.y = num[1];
				q2.z = num[2];
				q1 = q1 - q2;
				outQuat(q1.v4.num);
			}
			else if (!strcmp(order, "四元数点乘"))
			{
				_in(4);
				_out(4, num);
				ofs << '\t';
				glQuaternion q1, q2;
				q1.w = num[3];
				q1.x = num[0];
				q1.y = num[1];
				q1.z = num[2];
				_in(4);
				_out(4, num);
				ofs << '\t';
				q2.w = num[3];
				q2.x = num[0];
				q2.y = num[1];
				q2.z = num[2];
				num[0] = q1.dot(q2);
				out(1, num);
			}
			else if (!strcmp(order, "四元数求逆"))
			{
				_in(4);
				_out(4, num);
				ofs << '\t';
				glQuaternion q1;
				q1.w = num[3];
				q1.x = num[0];
				q1.y = num[1];
				q1.z = num[2];
				q1 = q1.inverse();
				outQuat(q1.v4.num);
			}
			else if (!strcmp(order, "四元数求角度和旋转轴"))
			{
				_in(4);
				_out(4, num);
				ofs << '\t';
				glQuaternion q1;
				q1.w = num[3];
				q1.x = num[0];
				q1.y = num[1];
				q1.z = num[2];
				q1 = q1.norm();
				num[0] = ZaxicMathUtil::toTheta(2 * acosf(q1.w));
				num[4] = acosf(q1.w);
				num[1] = q1.x / sinf(num[4]);
				num[2] = q1.y / sinf(num[4]);
				num[3] = q1.z / sinf(num[4]);
				out(1, num);
				ofs << '\t';
				out(3, num + 1);
			}
			else if (!strcmp(order, "四元数插值"))
			{
				_in(4);
				_out(4, num);
				ofs << '\t';
				glQuaternion q1, q2;
				q1.w = num[3];
				q1.x = num[0];
				q1.y = num[1];
				q1.z = num[2];
				_in(4);
				_out(4, num);
				ofs << '\t';
				q2.w = num[3];
				q2.x = num[0];
				q2.y = num[1];
				q2.z = num[2];
				q1 = q1.norm();
				q2 = q2.norm();
				_in(1);
				_out(1, num);
				ofs << '\t';
				q1 = SLERP(q1, q2, num[0]);
				outQuat(q1.v4.num);
			}
			ofs << std::endl;
		}
		ifs.close();
	}
	ofs.close();
}

glm::mat4 transZaxicMatToGLM(ZaxicMathUtil::mat4f mat)
{
	float* dat = mat.dat;
	glm::mat4 res(dat[0], dat[1], dat[2], dat[3], dat[4], dat[5], dat[6], dat[7], dat[8], dat[9], dat[10], dat[11], dat[12], dat[13], dat[14], dat[15]);
	return res;
}

glm::mat3 transZaxicMatToGLM(ZaxicMathUtil::mat3f mat)
{
	glm::mat3 res(mat[0][0], mat[0][1], mat[0][2], mat[1][0], mat[1][1], mat[1][2], mat[2][0], mat[2][1], mat[2][2]);
	return res;
}

glm::vec3 transZaxicVecToGLM(ZaxicMathUtil::vec3f vec)
{
	glm::vec3 res(vec.x, vec.y, vec.z);
	return res;
}

glm::vec4 transZaxicVecToGLM(ZaxicMathUtil::vec4f vec)
{
	glm::vec4 res(vec.x, vec.y, vec.z, vec.w);
	return res;
}


