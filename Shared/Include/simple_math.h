#ifndef __SIMPLE_MATH__
#define __SIMPLE_MATH__

struct GLVector3f
{
	GLfloat x;
	GLfloat y;
	GLfloat z;

	GLVector3f()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	GLVector3f(GLfloat _x, GLfloat _y, GLfloat _z = 0)
	{
		x = _x;
		y = _y;
		z = _z;
	}
};

#endif // !__SIMPLE_MATH__