#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform float bias;
out vec3 ourColor;

void main()
{
    gl_Position = vec4(position.x+bias*0.8,-position.y,position.z, 1.0f);
    ourColor = gl_Position.xyz;
}
