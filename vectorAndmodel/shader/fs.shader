#version 330 core
//in vec3 ourColor;
in vec2 ourTex;
in vec3 Norm;
in vec3 Pos;
out vec4 FragColor;

//uniform float alpha; // 在OpenGL程序代码中设定这个变量

uniform sampler2D tex0;
//uniform sampler2D tex2;

uniform vec3 lightPos;
uniform vec3 CamPos;

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
    float spec = min(specularStrength,1.0 - ambient) * max(pow(dot(refDir, viewDir), Shininess), 0.0f);

    return spec + diff + ambient;
}

void main()
{
    vec3 nlight = normalize(Pos - lightPos);
    vec3 norm = normalize(Norm);
    FragColor = texture(tex0, ourTex) *Phong(nlight, norm, 64, 0.3);
    //FragColor = mix(texture(tex1, ourTex), texture(tex2, ourTex), alpha) * vec4(ourColor, 1.0);
    //FragColor = texture(tex1, ourTex)*(0.2 + max( dot( normalize(lightPos - Pos), Norm), 0.0) + max(dot(normalize(CamPos - Pos), reflect( normalize(Pos - lightPos), Norm)), 0) * 0.5);
}

