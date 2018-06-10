#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;
out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = Proj*View*Model*vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}
