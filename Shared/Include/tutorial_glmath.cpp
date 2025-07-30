#include <glew.h>
#include "tutorial_glmath.h"


namespace tutorial
{
	Vector3f::Vector3f()
		: x(0), y(0), z(0)
	{
	}

	Vector3f::Vector3f(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z)
	{
	}
}