#if !defined(__TUTORIAL_UTILS__)
#define __TUTORIAL_UTILS__

#if !defined(__glew_h__)
#error glew.h must be included before tutorial_glutils.h
#endif

#if !defined(_STRING_)
#error string must be included before tutorial_glutils.h
#endif


namespace tutorial
{
	const std::string ReadFile(const std::string pathToFile);


	GLuint CreateCompiledShader(const GLenum shaderType, const std::string shaderSource, GLint* isShaderCompiled);

	GLuint CreateCompiledShaderProgram(const size_t shadersLength, const GLuint* shaders, GLint* isShaderProgramCompiled);
}


#endif // !__TUTORIAL_UTILS__