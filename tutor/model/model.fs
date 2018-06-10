#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    float ambient;
    float specular; 
    float diffuse;
};

uniform PointLight point_light;
uniform PointLight point_light2;
uniform float shininess;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 ViewPos;

void main()
{    

    vec3 ambient = point_light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 ambient2 = point_light2.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(point_light.position - FragPos);
    vec3 lightDir2 = normalize(point_light2.position - FragPos);
    vec3 diffuse = point_light.diffuse * max(dot(norm,lightDir),0.0) * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse2 = point_light2.diffuse * max(dot(norm,lightDir2),0.0) * vec3(texture(texture_diffuse1, TexCoords));

    vec3 reflectDir = reflect(-lightDir,norm);
    vec3 reflectDir2 = reflect(-lightDir2,norm);
    vec3 viewDir = normalize(ViewPos-FragPos);
    vec3 specular = point_light.specular * pow(max(dot(reflectDir,viewDir),0.0),shininess) * vec3(texture(texture_specular1, TexCoords));
    vec3 specular2 = point_light2.specular * pow(max(dot(reflectDir2,viewDir),0.0),shininess) * vec3(texture(texture_specular1, TexCoords));

    float distance    = length(point_light.position - FragPos);
    float attenuation = 1.0 / (point_light.constant + point_light.linear * distance + 
                    point_light.quadratic * (distance * distance));
    float distance2    = length(point_light2.position - FragPos);
    float attenuation2 = 1.0 / (point_light2.constant + point_light2.linear * distance + 
                    point_light2.quadratic * (distance * distance));

    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;

    ambient2 *= attenuation2; 
    diffuse2 *= attenuation2;
    specular2*= attenuation2;

    vec3 result = ((ambient + diffuse + specular)+(ambient2 + diffuse2 + specular2))*10;
    FragColor = vec4(result,1.0);
}

