#version 460 core


in vec4 color;

layout(location = 0) out vec4 diffuseColor;


void main()
{
	diffuseColor = color;
}