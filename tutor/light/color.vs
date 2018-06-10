#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aNormal;

uniform mat4 TransModel;
uniform mat4 RotateModel;
uniform mat4 View;
uniform mat4 Proj;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    gl_Position = Proj*View*TransModel*RotateModel*vec4(aPos, 1.0);
    FragPos = vec3(View*TransModel *RotateModel* vec4(aPos, 1.0));
    Normal = mat3(View*RotateModel) * aNormal;
}
