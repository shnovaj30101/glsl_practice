#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float innerCutOff;
	float outerCutOff;
};

uniform Light light;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

uniform Material material;

uniform vec3 viewPos;

void main()
{
	// 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position-FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));  


	float theta = dot(lightDir, normalize(-light.direction));

	if(theta > light.innerCutOff) 
	{       
	
		vec3 result = (ambient + diffuse + specular);
		FragColor = vec4(result, 1.0);

	  // 执行光照计算
	}
	else if (theta > light.outerCutOff) {
		vec3 result = ambient + (diffuse + specular)*(theta-light.outerCutOff)/(light.innerCutOff-light.outerCutOff);
		FragColor = vec4(result, 1.0);

	}
	else { // 否则，使用环境光，让场景在聚光之外时不至于完全黑暗
		FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
	}

}

