#version 330 core
layout(location = 0) in vec3 mpos;

uniform mat4 model;
uniform mat4 proj_view;

void main()
{
	gl_Position = proj_view * model * vec4(mpos,1.0);
}