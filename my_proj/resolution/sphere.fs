#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform PointLight point_light;
uniform DirLight dir_light;
uniform DirLight dir_light2;
uniform DirLight dir_light3;
uniform DirLight dir_light4;

uniform vec3 viewPos;
uniform float shininess;

vec3 CalcPointLight(PointLight point_light);
vec3 CalcDirLight(DirLight dir_light);

void main()
{
    vec3 result = CalcPointLight(point_light);
    result += CalcDirLight(dir_light);
    result += CalcDirLight(dir_light2);
    
    FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight point_light) {
    // 环境光
    vec3 ambient = point_light.ambient;

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(point_light.position-FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 diffuse;

    if (dot(norm,viewDir)<0) {
        diffuse = vec3(0,0,0);
    } else {
        diffuse = point_light.diffuse * diff;
    }

    // 镜面光
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = point_light.specular * spec;  

    float distance    = length(point_light.position - FragPos);
    float attenuation = 1.0 / (point_light.constant + point_light.linear * distance + 
                    point_light.quadratic * (distance * distance));

	vec3 result = (ambient + diffuse + specular) * attenuation;
    return result;
}

vec3 CalcDirLight(DirLight dir_light) {
    // 环境光
    vec3 ambient = dir_light.ambient;

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-dir_light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 diffuse;

    if (dot(norm,viewDir)<0) {
        diffuse = vec3(0,0,0);
    } else {
        diffuse = dir_light.diffuse * diff;
    }

    // 镜面光
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = dir_light.specular * spec;  

	vec3 result = (ambient + diffuse + specular);
    return result;
}

