#if !defined(__TUTORIAL_MATH__)
#define __TUTORIAL_MATH__

#if !defined(__glew_h__)
#error glew.h must be included before tutorial_glutils.h
#endif // !__glew_h__


namespace tutorial
{
	struct vec3
	{
		GLfloat x;
		GLfloat y;
		GLfloat z;

		vec3();
		vec3(GLfloat _x, GLfloat _y, GLfloat _z);
	};

	struct mat4x4
	{
		GLfloat data[4][4];
		
		mat4x4();
		mat4x4(
			GLfloat row00, GLfloat row01, GLfloat row02, GLfloat row03,
			GLfloat row11, GLfloat row12, GLfloat row13, GLfloat row14,
			GLfloat row21, GLfloat row22, GLfloat row23, GLfloat row24,
			GLfloat row31, GLfloat row32, GLfloat row33, GLfloat row34
		);
	};
}


#endif // !__TUTORIAL_MATH__