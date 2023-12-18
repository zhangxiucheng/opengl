#include"shader.h"
shaderProgram::shaderProgram(std::string& vspath,std::string& fspath)
{
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	program = glCreateProgram();
	std::ifstream ifs;
	std::stringstream ss;
	std::string content;
	ifs.open(vspath, std::ios_base::in);
	if (ifs.is_open())
	{
		ss << ifs.rdbuf();
		content = ss.str();
		const char* buf = content.c_str();
		glShaderSource(vs, 1, &buf, NULL);
		glCompileShader(vs);
		if (g_log!=nullptr)
			g_log->check(glComponentType::SHADER, GL_COMPILE_STATUS, vs);
		else
		{
			int success;
			char errinfo[512];
			glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vs, 512, NULL, errinfo);
				std::cout << "Vertex Shader Compile Error!\n" << errinfo << std::endl;
				exit(-1);
			}
		}
	}
	else
	{
		std::string c = "Error When opening shader:" + vspath;
		if (g_log != nullptr)
			g_log->newlog(c);
		else
			std::cout << c << std::endl;
	}
	
	ss.str("");
	content = "";

	ifs.close();
	ifs.open(fspath, std::ios_base::in);
	if (ifs.is_open())
	{
		ss << ifs.rdbuf();
		content = ss.str();
		const char* buf = content.c_str();
		glShaderSource(fs, 1, &buf, NULL);
		glCompileShader(fs);
		if (g_log != nullptr)
			g_log->check(glComponentType::SHADER, GL_COMPILE_STATUS, fs);
		else
		{
			int success;
			char errinfo[512];
			glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fs, 512, NULL, errinfo);
				std::cout << "Fragment Shader Compile Error!\n" << errinfo << std::endl;
				exit(-1);
			}
		}
	}
	else
	{
		std::string c = "Error When opening shader:" + fspath;
		if (g_log != nullptr)
			g_log->newlog(c);
		else
			std::cout << c << std::endl;
	}

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	if (g_log != nullptr)
		g_log->check(glComponentType::PROGRAM, GL_LINK_STATUS, program);
	else
	{
		int success;
		char errinfo[512];
		glGetProgramiv(program, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(vs, 512, NULL, errinfo);
			std::cout << "Program Link Error!\n" << errinfo << std::endl;
			exit(-1);
		}
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
}

void shaderProgram::use()
{
	glUseProgram(program);
}

void shaderProgram::setUniform(glUniformType ut, char* data, size_t sz,const std::string& name)
{
	glUseProgram(program);
	const char* buf = name.c_str();
	int loc = glGetUniformLocation(program, buf);
	switch (ut)
	{
	case glUniformType::FLOAT:
		if (sz==4)
			glUniform1f(loc, *(GLfloat*)data);
		break;
	case glUniformType::INT:
		if (sz==4)
			glUniform1i(loc, *(GLint*)data);
		break;
	case glUniformType::VEC3F:
		if (sz == 12)
			glUniform3f(loc, ((GLfloat*)data)[0], ((GLfloat*)data)[1], ((GLfloat*)data)[2]);
		break;
	case glUniformType::VEC4F:
		if (sz == 16)
			glUniform4f(loc, ((GLfloat*)data)[0], ((GLfloat*)data)[1], ((GLfloat*)data)[2], ((GLfloat*)data)[3]);
		break;
	case glUniformType::VEC4I:
		if (sz == 16)
			glUniform4i(loc, ((GLint*)data)[0], ((GLint*)data)[1], ((GLint*)data)[2], ((GLint*)data)[3]);
		break;
	case glUniformType::VEC3I:
		if (sz == 12)
			glUniform3i(loc, ((GLint*)data)[0], ((GLint*)data)[1], ((GLint*)data)[2]);
		break;
	case glUniformType::MAT4:
		if (sz == 64)
			glUniformMatrix4fv(loc, 1, GL_FALSE, (GLfloat*)data);
		break;
	default:
		break;
	}
}

shaderProgram::~shaderProgram()
{
	glDeleteProgram(program);
}
