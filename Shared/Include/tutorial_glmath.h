#if !defined(__TUTORIAL_MATH__)
#define __TUTORIAL_MATH__

#if !defined(__glew_h__)
#error glew.h must be included before tutorial_glutils.h
#endif


namespace tutorial
{
	struct Vector3f
	{
		float x;
		float y;
		float z;

		Vector3f();
		Vector3f(float _x, float _y, float _z = 0);
	};
}


#endif // !__TUTORIAL_MATH__