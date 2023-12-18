#version 330 core
layout (location = 0) in vec3 aPos; // 位置变量的属性位置值为0
//layout (location = 1) in vec3 aColor;//point color
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;
//out vec3 ourColor; // 为片段着色器指定一个颜色输出
out vec2 ourTex;
out vec3 Norm;
out vec3 Pos;

uniform mat4 model;
uniform mat4 proj_view;

void main()
{
    gl_Position = proj_view * model * vec4(aPos, 1.0); // 注意我们如何把一个vec3作为vec4的构造器的参数
    //ourColor = aColor;
    ourTex = aTex;
    Norm = mat3(transpose(inverse(model)))*aNorm;
    Pos = vec3(gl_Position.xyz);
}