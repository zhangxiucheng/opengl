#version 330 core
out vec4 FragColor;

in vec3 Pos;
in vec3 norm;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec4 fc;
uniform int code;

uniform vec3 lightPos;
uniform vec3 CamPos;
uniform int chosen;

float Phong(vec3 nlightDir, vec3 norm, float Shininess, float specularStrength)
{
    //float ph = 0.0f;
    //ambient
    float ambient = 0.6f;

    //diffuse
    float diff = max(dot(-nlightDir, norm), 0.0f) * max((1.0f - ambient - specularStrength), 0.0f);

    //specular
    vec3 viewDir = normalize(CamPos - Pos);
    vec3 refDir = normalize(reflect(nlightDir, norm));
    float spec = min(specularStrength, 1.0 - ambient) * max(pow(dot(refDir, viewDir), Shininess), 0.0f);

    return spec + diff + ambient;
}

void main()
{   
    vec3 nlightDir = lightPos - CamPos;
    vec3 _norm = normalize(norm);
    if (chosen==0)
        FragColor = vec4(texture(texture_diffuse1, TexCoords).rgb * Phong(nlightDir, _norm, 64f, 0.2f), float(code) / 255.f);
    else
        FragColor = vec4(fc.xyz * Phong(nlightDir, _norm, 32f, 0.2), float(code) / 255.f);
}