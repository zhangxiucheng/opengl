#version 330 core
layout (location = 0) in vec3 aPos; // λ�ñ���������λ��ֵΪ0
//layout (location = 1) in vec3 aColor;//point color
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;
//out vec3 ourColor; // ΪƬ����ɫ��ָ��һ����ɫ���
out vec2 ourTex;
out vec3 Norm;
out vec3 Pos;

uniform mat4 model;
uniform mat4 proj_view;

void main()
{
    gl_Position = proj_view * model * vec4(aPos, 1.0); // ע��������ΰ�һ��vec3��Ϊvec4�Ĺ������Ĳ���
    //ourColor = aColor;
    ourTex = aTex;
    Norm = mat3(transpose(inverse(model)))*aNorm;
    Pos = vec3(gl_Position.xyz);
}