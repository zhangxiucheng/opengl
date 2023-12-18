#ifndef __GL_FRAMEWORK__
#define __GL_FRAMEWORK__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<ft2build.h>
#include FT_FREETYPE_H

#include<iostream>
#include<fstream>
#include<filesystem>
#include<string>
#include<vector>
#include<queue>
#include<sstream>
#include<cctype>
#include<ctime>
#include<map>
#include<list>

#include"glmath.h"



typedef unsigned glID;


enum class glComponentType
{
	SHADER,
	PROGRAM
};

enum class glUniformType
{
	FLOAT,
	INT,
	VEC3F,
	VEC4F,
	VEC4I,
	VEC3I,
	MAT4
};

#define _1M 1048576u



#define MAXLOGCOUNT 10
class mlog 
{
private:
	unsigned count;
	unsigned seq;
	std::string logpath;
	std::ofstream ofs;
	void loadprofile();
	void _newlog();
public:
	mlog();
	void check(glComponentType tp, GLenum pname, glID pid);
	void newlog(std::string& content);
	~mlog();
};

extern mlog* g_log;

enum class glTrajectoryType
{
	RING,
	RECTANGLE,
	LINE,

};
#define glIterFirst  (0)
#define glIterSecond (1)
#define glSwitchIter(i) ((i)=(((i)==glIterFirst) ? glIterSecond : glIterFirst))
#define glAnotherIter(i) (((i)==glIterFirst) ? glIterSecond : glIterFirst)

std::string getTime();
std::vector<glm::vec3> glGenTrajectory(glTrajectoryType tt, uint32_t count, glm::vec3 st,glm::vec3 ed);

void mathtest_vec3f();
void matTest();
void eulquatTest();

glm::mat4 transZaxicMatToGLM(ZaxicMathUtil::mat4f mat);
glm::mat3 transZaxicMatToGLM(ZaxicMathUtil::mat3f mat);
glm::vec3 transZaxicVecToGLM(ZaxicMathUtil::vec3f vec);
glm::vec4 transZaxicVecToGLM(ZaxicMathUtil::vec4f vec);

typedef struct _character
{
	glID texID;
	glm::ivec2 Size;       // 字形大小
	glm::ivec2 Bearing;    // 从基准线到字形左部/顶部的偏移值
	GLuint     Advance;    // 原点距下一个字形原点的距离
}Character, * pCharacter;

#define WNDOFFSET 200.f
#endif // !__GL_FRAMEWORK__