#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 TransModel;
uniform mat4 RotateModel;
uniform mat4 View;
uniform mat4 Proj;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
    gl_Position = Proj*View*TransModel*RotateModel*vec4(aPos, 1.0);
    FragPos = vec3(TransModel *RotateModel* vec4(aPos, 1.0));
    Normal = mat3(RotateModel) * aNormal;
    TexCoords = aTexCoords;
}
