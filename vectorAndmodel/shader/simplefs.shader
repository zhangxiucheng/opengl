#version 330 core
in vec3 tc;
out vec4 FragColor;

uniform samplerCube skytex;

void main()
{
	FragColor = texture(skytex, tc);
}