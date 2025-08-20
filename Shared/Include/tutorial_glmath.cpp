#include <glew.h>
#include "tutorial_glmath.h"


namespace tutorial
{
	vec3::vec3()
		: x(0), y(0), z(0)
	{ }

	vec3::vec3(GLfloat _x, GLfloat _y, GLfloat _z)
		: x(_x), y(_y), z(_z)
	{ }


	mat4x4::mat4x4()
		: mat4x4(
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f)
	{ }

	mat4x4::mat4x4(
		GLfloat row00, GLfloat row01, GLfloat row02, GLfloat row03,
		GLfloat row10, GLfloat row11, GLfloat row12, GLfloat row13,
		GLfloat row20, GLfloat row21, GLfloat row22, GLfloat row23,
		GLfloat row30, GLfloat row31, GLfloat row32, GLfloat row33)
	{
		data[0][0] = row00; data[0][1] = row01; data[0][2] = row02; data[0][3] = row03;
		data[1][0] = row10; data[1][1] = row11; data[1][2] = row12; data[1][3] = row13;
		data[2][0] = row20; data[2][1] = row21; data[2][2] = row22; data[2][3] = row23;
		data[3][0] = row30; data[3][1] = row31; data[3][2] = row32; data[3][3] = row33;
	}
}