#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float reflectStrength = 0.5;
    vec3 ToViewDir = normalize(viewPos - FragPos);
    vec3 ReflectDir = normalize(norm*2*dot(norm,lightDir)-lightDir);
    float refl = pow(max(dot(ToViewDir,ReflectDir),0.0),16);
    vec3 reflection = reflectStrength * refl * lightColor;

	vec3 result = (ambient+diffuse+reflection) * objectColor;
    FragColor = vec4(result, 1.0);
}

