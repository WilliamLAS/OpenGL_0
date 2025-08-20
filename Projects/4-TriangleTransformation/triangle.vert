#version 460 core

layout(location = 0) in vec3 inPosition;

uniform float uniPositionScaleX;

void main()
{
	gl_Position = vec4(uniPositionScaleX * inPosition.x,
					uniPositionScaleX * inPosition.y,
					inPosition.z,
					1.0f);
}