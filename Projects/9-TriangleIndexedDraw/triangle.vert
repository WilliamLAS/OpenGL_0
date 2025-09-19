#version 460 core


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
uniform mat4x4 model;

out vec4 color;


void main()
{
	gl_Position = model * vec4(inPosition, 1.0f);
	color = vec4(inColor, 1.0f);
}