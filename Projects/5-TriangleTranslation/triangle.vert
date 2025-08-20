#version 460 core


layout(location = 0) in vec3 inPosition;
uniform mat4x4 translation;


void main()
{
	gl_Position = translation * vec4(inPosition, 1.0f);
}