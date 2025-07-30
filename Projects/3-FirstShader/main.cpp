#include <iostream>
#include <string>
#include<glew.h>
#include <glut.h>
#include <tutorial_glmath.h>
#include <tutorial_glutils.h>

#define coutWorkStart(information) std::cout << __FUNCTION__ << " started: " << information << "\n";
#define coutWorkSuccess(information) std::cout << __FUNCTION__ << " succeded: " << information << "\n";
#define cerrWorkFail(reason) std::cerr << __FUNCTION__ << " failed: " <<  reason << "\n";

GLuint triangleVertexBufferObjectId = 0;
GLuint triangleVertexArrayObjectId = 0;
GLuint triangleShaderProgramId = 0;


// Initialize
GLboolean InitializeGLWindow(int* argCount, char* args[], const char* title)
{
	coutWorkStart("Creating window using FreeGlut.");
	
	glutInit(argCount, args);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);

	int windowId = glutCreateWindow(title);
	if (windowId <= 0)
	{
		cerrWorkFail("Unknown.");
		return GL_FALSE;
	}
	
	coutWorkSuccess("Active window identifier: " + std::to_string(glutGetWindow()));
	return GL_TRUE;
}

GLboolean InitializeGLFunctions()
{
	coutWorkStart("Loading GL functions using Glew.");
	GLenum glewResult = glewInit();

	if (glewResult != GLEW_OK)
	{
		cerrWorkFail(glewGetErrorString(glewResult));
		return GL_FALSE;
	}
	
	coutWorkSuccess("");
	return GL_TRUE;
}

GLboolean InitializeGLSettings()
{
	coutWorkStart("Setting initial values.");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_CULL_FACE);

	coutWorkSuccess("");
	return GL_TRUE;
}

GLboolean InitializeTriangleVertexData()
{
	coutWorkStart("");
	tutorial::Vector3f positions[3] = { tutorial::Vector3f(1, -1),  tutorial::Vector3f(0, 1),  tutorial::Vector3f(-1, -1) };

	glGenBuffers(1, &triangleVertexBufferObjectId);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVertexBufferObjectId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &triangleVertexArrayObjectId);
	glBindVertexArray(triangleVertexArrayObjectId);
	glBindVertexBuffer(0, triangleVertexBufferObjectId, 0, sizeof(tutorial::Vector3f));
	glVertexAttribBinding(0, 0);
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	coutWorkSuccess("");
	return GL_TRUE;
}

GLboolean InitializeShader(GLuint* shaderId, GLenum shaderType, const std::string shaderFilePath)
{
	coutWorkStart("Compiling type of (" + std::to_string(shaderType) + ") for file (" + shaderFilePath + ").");
	GLint isShaderCompiled = 0;
	const std::string shaderSource = tutorial::ReadFile(shaderFilePath);

	if (shaderSource.empty())
	{
		cerrWorkFail("Source is empty.");
		return GL_FALSE;
	}

	*shaderId = tutorial::CreateCompiledShader(GL_VERTEX_SHADER, shaderSource, &isShaderCompiled);
	if (*shaderId == 0)
	{
		cerrWorkFail("Creation failed.");
		return GL_FALSE;
	}

	if (isShaderCompiled == 0)
	{
		GLchar temporaryInfoLog[1024];
		glGetShaderInfoLog(*shaderId, 1024, NULL, temporaryInfoLog);

		cerrWorkFail("Compilation failed.\n" + std::string(temporaryInfoLog));
		return GL_FALSE;
	}
	
	coutWorkSuccess("");
	return GL_TRUE;
}

GLboolean InitializeShaderProgram(GLuint* shaderProgramId, const size_t shadersLength, const GLuint* shaders, const std::string arbitraryProgramName)
{
	coutWorkStart("Linking (" + arbitraryProgramName + ").");
	GLboolean isShaderCompiled = GL_FALSE;
	GLint isShaderProgramCompiled = 0;
	GLint isShaderProgramValid = 0;

	*shaderProgramId = tutorial::CreateCompiledShaderProgram(shadersLength, shaders, &isShaderProgramCompiled, &isShaderProgramValid);
	if (*shaderProgramId == 0)
	{
		cerrWorkFail("Creation failed.");
		return GL_FALSE;
	}

	if (isShaderProgramValid == 0)
	{
		GLchar temporaryInfoLog[1024];
		std::string failReason;
		glGetProgramInfoLog(*shaderProgramId, 1024, NULL, temporaryInfoLog);

		if (isShaderProgramCompiled == 0)
			failReason = "Compilation failed.";
		else
			failReason = "Validation failed.";

		cerrWorkFail(failReason + "\n" + std::string(temporaryInfoLog));
		return GL_FALSE;
	}

	coutWorkSuccess("");
	return GL_TRUE;
}


// Update
void DrawTriangle()
{
	glUseProgram(triangleShaderProgramId);
	glBindVertexArray(triangleVertexArrayObjectId);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	glUseProgram(0);
}

void OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Sleep(25);
	DrawTriangle();

	glutSwapBuffers();
}

int main(int argCount, char* args[])
{
	GLboolean windowInitResult = InitializeGLWindow(&argCount, args, "3 - First Shader");
	if (windowInitResult == GL_FALSE)
		return -1;

	GLboolean glewInitResult = InitializeGLFunctions();
	if (glewInitResult == GL_FALSE)
		return -1;
	
	GLuint shaders[2] = { 0, 0 };
	GLboolean shaderCompilationResult = InitializeShader(&shaders[0], GL_VERTEX_SHADER, "triangle.vert");
	if (shaderCompilationResult == GL_FALSE)
		return -1;

	shaderCompilationResult = InitializeShader(&shaders[1], GL_FRAGMENT_SHADER, "triangle.frag");
	if (shaderCompilationResult == GL_FALSE)
		return -1;

	GLboolean shaderProgramLinkingResult = InitializeShaderProgram(&triangleShaderProgramId, 2, shaders, "Triangle Shader Program");
	if (shaderProgramLinkingResult == GL_FALSE)
		return -1;

	InitializeGLSettings();
	InitializeTriangleVertexData();
	glutDisplayFunc(OnDisplay);

	printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
	printf("Using OpenGL %s \n", glGetString(GL_VERSION));

	glutMainLoop();
	return 0;
}