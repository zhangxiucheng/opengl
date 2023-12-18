#ifndef __GLSL_SHADER__
#define __GLSL_SHADER__
#include"glFrameWork.h"
class shaderProgram {
private:
	glID program;
	//extension
	glID vs, fs;
	
public:
	shaderProgram() = delete;
	shaderProgram(const shaderProgram&) = delete;
	shaderProgram(std::string& vspath,std::string& fspath);

	shaderProgram& operator=(const shaderProgram&) = delete;

	void use();
	void setUniform(glUniformType ut, char* data, size_t sz, const std::string& name);

	//uniform api


	~shaderProgram();
};

#endif // !__GLSL_SHADER__
