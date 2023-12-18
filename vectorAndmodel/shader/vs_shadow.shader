#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;

out vs_out
{
	vec3 pos;
	vec3 norm;
	vec2 tc;
	vec4 ltspcpos;
} _out;

uniform mat4 model_pv;
uniform mat4 model;
uniform mat4 lt_pv;


void main()
{
	gl_Position = model_pv * model * vec4(pos, 1.0f);
	_out.pos = vec3( model * vec4(pos, 1.0f));
	_out.norm = normalize(transpose(inverse(mat3(model))) * norm);
	_out.tc = tex;
	_out.ltspcpos = lt_pv * vec4(_out.pos, 1.0f);
}