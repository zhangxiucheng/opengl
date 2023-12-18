#version 330 core
layout(location = 0) in vec3 mpos;
//layout(location = 1) in vec3 mcolor;

out vec3 tc;
//uniform mat4 model;
uniform mat4 model_pv;

void main()
{
	gl_Position = model_pv * vec4(mpos,1.0);
	tc = mpos;
	//gl_Position = pos.xyww;
}