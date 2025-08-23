#version 460 core


layout(location = 0) in vec3 inPosition;
// uniform float uniPositionScaleX; // Not recommended
uniform mat4x4 scale;



void main()
{
	/*
	// Not recommended. Matrixes can be combined and optimized.
	gl_Position = vec4(uniPositionScaleX * inPosition.x,
					uniPositionScaleX * inPosition.y,
					inPosition.z,
					1.0f);
	*/

	gl_Position = scale * vec4(inPosition, 1.0f);
}