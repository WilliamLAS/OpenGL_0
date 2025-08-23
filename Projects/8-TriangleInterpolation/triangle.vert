#version 460 core


layout(location = 0) in vec3 inPosition;
uniform mat4x4 model;

const vec4[3] vertexColors = vec4[3](
	vec4(1.0f, 0.0f, 0.0f, 1.0f),
	vec4(0.0f, 1.0f, 0.0f, 1.0f),
	vec4(0.0f, 0.0f, 1.0f, 1.0f)
);

out vec4 color;


void main()
{
	gl_Position = model * vec4(inPosition, 1.0f);
	color = vertexColors[gl_VertexID];
}