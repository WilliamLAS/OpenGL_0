#version 460 core


layout(location = 0) in vec3 inPosition;
uniform mat4x4 rotation;


void main()
{
	gl_Position = rotation * vec4(inPosition, 1.0f);
}