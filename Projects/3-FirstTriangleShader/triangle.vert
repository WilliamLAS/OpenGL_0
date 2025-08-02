#version 460 core

layout(location = 0) in vec3 inPosition;

void main()
{
	gl_Position = vec4(0.5f * inPosition.x,
					0.5f * inPosition.y,
					inPosition.z,
					1.0f);
}