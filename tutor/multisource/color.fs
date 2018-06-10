#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct SpotLight {
    vec3 direction;
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float outerCutOff;
    float innerCutOff;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};


struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

uniform Material material;
uniform DirLight dir_light;
uniform PointLight point_light;
uniform SpotLight spot_light;
uniform vec3 viewPos;
vec3 CalcDirLight(DirLight dir_light, vec3 normal , vec3 viewDir);
vec3 CalcPointLight(PointLight point_light, vec3 normal , vec3 viewDir);
vec3 CalcSpotLight(SpotLight spot_light, vec3 normal , vec3 viewDir);

void main()
{
    vec3 ViewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dir_light,Normal,ViewDir)*0.3;
    result += CalcPointLight(point_light,Normal,ViewDir)*0.3;
    result += CalcSpotLight(spot_light,Normal,ViewDir)*1;

    FragColor = vec4(result,1.0);
}

vec3 CalcDirLight(DirLight dir_light, vec3 normal , vec3 viewDir) {
	// 环境光
    vec3 ambient = dir_light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 漫反射 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-dir_light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dir_light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 镜面光
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dir_light.specular * spec * vec3(texture(material.specular, TexCoords));  
	vec3 result = ambient + diffuse + specular;

    return result;
}

vec3 CalcPointLight(PointLight point_light, vec3 normal , vec3 viewDir) {
    // 环境光 
    vec3 ambient = point_light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 漫反射 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(point_light.position-FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = point_light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 镜面光
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = point_light.specular * spec * vec3(texture(material.specular, TexCoords));  

    float distance    = length(point_light.position - FragPos);
    float attenuation = 1.0 / (point_light.constant + point_light.linear * distance + point_light.quadratic * (distance * distance));
	vec3 result = (ambient + diffuse + specular) * attenuation;

    return result;
}

vec3 CalcSpotLight(SpotLight spot_light, vec3 normal , vec3 viewDir) {
    // 环境光
    vec3 ambient = spot_light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 漫反射 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(spot_light.position-FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = spot_light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 镜面光
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spot_light.specular * spec * vec3(texture(material.specular, TexCoords));  


	float theta = dot(lightDir, normalize(-spot_light.direction));

	if(theta > spot_light.innerCutOff) 
	{       
	
		vec3 result = (ambient + diffuse + specular);
        return result;

	  // 执行光照计算
	}
	else if (theta > spot_light.outerCutOff) {
		vec3 result = ambient + (diffuse + specular)*(theta-spot_light.outerCutOff)/(spot_light.innerCutOff-spot_light.outerCutOff);
        return result;

	}
	else { // 否则，使用环境光，让场景在聚光之外时不至于完全黑暗
		vec3 result = spot_light.ambient * vec3(texture(material.diffuse, TexCoords));
        return result;
	}


}
