#version 330 core

in vs_out
{
	vec3 pos;
	vec3 norm;
	vec2 tc;
	vec4 ltspcpos;
} fs_in;

out vec4 FragColor;

uniform sampler2D shadowMap;
uniform sampler2D tx;
//uniform int code;
uniform int chosen;
uniform float ambient;
uniform float rt;
uniform vec4 fc;

uniform vec3 lightPos;
uniform vec3 CamPos;

//Blinn-Phong lighting model
float Blinn_Phong(vec3 nlightDir, vec3 viewDir, vec3 norm, float specularStrength, float Shininess, float shadow)
{
    vec3 halfwayDir = normalize(viewDir - nlightDir);
    //float ph = 0.0f;
    //ambient
    //float ambient = 0.6f;

    //diffuse
    float diff = max(dot(-nlightDir, norm), 0.0f) * max((1.0f - ambient - specularStrength), 0.0f);

    //specular
    //vec3 viewDir = normalize(CamPos - fs_in.pos);
    vec3 refDir = normalize(reflect(nlightDir, norm));
    float spec = min(specularStrength, 1.0 - ambient) * pow (max(dot(halfwayDir, norm), 0.0f), Shininess);

    return (1.0f - shadow) * (spec +  diff) + ambient;
}

float CalcShadow(vec3 ld)
{
    //perspective div
    vec3 projCoord = fs_in.ltspcpos.xyz / fs_in.ltspcpos.w;
    projCoord = projCoord * 0.5f + 0.5f;
    //closest depth
    float dp = texture(shadowMap, projCoord.xy).r;
    float _dp = projCoord.z - max(0.05f * (1.0 - dot(fs_in.norm, ld)), 0.005f);
    //result
    return (_dp > dp) ? 1.0f : 0.0f;
}

void main()
{
    vec3 nlightDir = normalize(fs_in.pos - lightPos);
    vec3 viewDir = normalize(CamPos - fs_in.pos);
    float shadow = CalcShadow(-nlightDir);
    if (chosen == 0)
        FragColor = vec4(texture(tx, fs_in.tc).rgb * Blinn_Phong(nlightDir, viewDir, fs_in.norm, .2f, 64.f, shadow), 1.f);
    else
        FragColor = mix(vec4(texture(tx, fs_in.tc).rgb * Blinn_Phong(nlightDir, viewDir, fs_in.norm, .2f, 64.f, shadow), 1.f), fc, rt);
}