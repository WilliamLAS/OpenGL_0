#include <iostream>
#include <string>
#include<glew.h>
#include <glut.h>
#include <tutorial_glmath.h>
#include <tutorial_glutils.h>



// Macros
#define coutWorkStart() std::cout << __FUNCTION__ << " started. \n";
#define coutWorkSuccess() std::cout << __FUNCTION__ << " succeded. \n\n";
#define cerrWorkFail() std::cerr << __FUNCTION__ << " failed. \n\n";



// Helpers
GLboolean InitializeShader(GLuint* shaderId, GLenum shaderType, const std::string shaderFilePath)
{
	coutWorkStart();
	std::cout << "Compiling type of (" << shaderType << ") for file (" << shaderFilePath << "). \n";

	const std::string shaderSource = tutorial::ReadFile(shaderFilePath);
	if (shaderSource.empty())
	{
		std::cout << "Source is empty. \n";
		cerrWorkFail();
		return GL_FALSE;
	}

	GLint isShaderCompiled = 0;
	*shaderId = tutorial::CreateCompiledShaderObject(shaderType, shaderSource, &isShaderCompiled);
	if (*shaderId == 0)
	{
		std::cout << "Creation failed. \n";
		cerrWorkFail();
		return GL_FALSE;
	}

	if (isShaderCompiled == 0)
	{
		GLchar temporaryInfoLog[1024];
		glGetShaderInfoLog(*shaderId, 1024, NULL, temporaryInfoLog);

		std::cout << "Compilation failed: \n" << temporaryInfoLog << "\n";
		cerrWorkFail();
		return GL_FALSE;
	}

	coutWorkSuccess();
	return GL_TRUE;
}

GLboolean InitializeShaderProgram(GLuint* shaderProgramId, const size_t shadersLength, const GLuint* shaders, const std::string arbitraryProgramName)
{
	coutWorkStart();
	std::cout << "Creating: " << arbitraryProgramName << "\n";

	GLint isShaderProgramCompiled = 0;
	*shaderProgramId = tutorial::CreateCompiledShaderProgramObject(shadersLength, shaders, &isShaderProgramCompiled);
	if (*shaderProgramId == 0)
	{
		std::cout << "Creation failed.";
		cerrWorkFail();
		return GL_FALSE;
	}

	if (isShaderProgramCompiled == 0)
	{
		GLchar temporaryInfoLog[1024];
		glGetProgramInfoLog(*shaderProgramId, 1024, NULL, temporaryInfoLog);

		std::cout << "Compilation failed because: \n" << temporaryInfoLog << "\n";
		cerrWorkFail();
		return GL_FALSE;
	}

	GLint isShaderProgramValid = 0;
	glValidateProgram(*shaderProgramId);
	glGetProgramiv(*shaderProgramId, GL_VALIDATE_STATUS, &isShaderProgramValid);
	if (isShaderProgramValid == 0)
	{
		GLchar temporaryInfoLog[1024];
		glGetProgramInfoLog(*shaderProgramId, 1024, NULL, temporaryInfoLog);

		std::cout << "Validation failed because: \n" << temporaryInfoLog << "\n";
		cerrWorkFail();
		return GL_FALSE;
	}

	coutWorkSuccess();
	return GL_TRUE;
}



// Variables
GLuint triangleVertexBufferObjectId = 0;
GLuint triangleVertexArrayObjectId = 0;

struct
{
	GLuint id = 0;
	GLint translationLocation = -1;

	GLboolean Initialize()
	{
		coutWorkStart();

		GLuint shaders[2] = { 0, 0 };
		GLboolean shaderCompilationResult = InitializeShader(&shaders[0], GL_VERTEX_SHADER, "triangle.vert");
		if (shaderCompilationResult == GL_FALSE)
		{
			cerrWorkFail();
			return GL_FALSE;
		}

		shaderCompilationResult = InitializeShader(&shaders[1], GL_FRAGMENT_SHADER, "triangle.frag");
		if (shaderCompilationResult == GL_FALSE)
		{
			cerrWorkFail();
			return GL_FALSE;
		}

		GLboolean shaderProgramLinkingResult = InitializeShaderProgram(&id, 2, shaders, "Triangle Shader Program");
		if (shaderProgramLinkingResult == GL_FALSE)
		{
			cerrWorkFail();
			return GL_FALSE;
		}

		translationLocation = glGetUniformLocation(id, "translation");
		if (translationLocation < 0)
		{
			std::cout << "Cant find translationLocation. \n";
			cerrWorkFail();
			return GL_FALSE;
		}

		for (size_t i = 0; i < 2; i++)
			glDeleteShader(shaders[i]);

		coutWorkSuccess();
		return GL_TRUE;
	}
} triangleTransformationShaderProgram;



// Initialize
GLboolean InitializeGLWindow(int* argCount, char* args[], const char* title)
{
	coutWorkStart();
	std::cout << "Using FreeGlut API: " << GLUT_API_VERSION << "\n";
	
	glutInit(argCount, args);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);

	int windowId = glutCreateWindow(title);
	if (windowId <= 0)
	{
		std::cout << "Unknown error occured. \n";
		cerrWorkFail();
		return GL_FALSE;
	}
	
	std::cout << "Active window identifier: " << glutGetWindow() << "\n";
	coutWorkSuccess();
	return GL_TRUE;
}

GLboolean InitializeGLFunctions()
{
	coutWorkStart();
	std::cout << "Using GLEW: " << glewGetString(GLEW_VERSION) << "\n";

	GLenum glewResult = glewInit();
	if (glewResult != GLEW_OK)
	{
		std::cout << glewGetErrorString(glewResult) << "\n";
		cerrWorkFail();
		return GL_FALSE;
	}
	
	std::cout << "Using OpenGL:" << glGetString(GL_VERSION) << "\n";
	coutWorkSuccess();
	return GL_TRUE;
}

GLboolean InitializeGLSettings()
{
	coutWorkStart();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_CULL_FACE);

	coutWorkSuccess();
	return GL_TRUE;
}

GLboolean InitializeTriangleVertexData()
{
	coutWorkStart();
	tutorial::vec3 positions[3] = { tutorial::vec3(1.0f, -1.0f, 0.0f),  tutorial::vec3(0.0f, 1.0f, 0.0f),  tutorial::vec3(-1, -1, 0) };

	glGenBuffers(1, &triangleVertexBufferObjectId);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVertexBufferObjectId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &triangleVertexArrayObjectId);
	glBindVertexArray(triangleVertexArrayObjectId);
	glBindVertexBuffer(0, triangleVertexBufferObjectId, 0, sizeof(tutorial::vec3));
	glVertexAttribBinding(0, 0);
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	coutWorkSuccess();
	return GL_TRUE;
}



// Update
void DrawTriangle()
{
	static GLfloat amount = 0.0f;
	static GLfloat speed = 0.05f;

	amount += speed;
	if (amount > 1.0f)
	{
		amount = 1.0f;
		speed *= -1.0f;
	}
	else if (amount < -1.0f)
	{
		amount = -1.0f;
		speed *= -1.0f;
	}

	tutorial::mat4x4 translation = tutorial::mat4x4(
		1.0f, 0.0f, 0.0f, amount,
		0.0f, 1.0f, 0.0f, amount,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	glUseProgram(triangleTransformationShaderProgram.id);
	glUniformMatrix4fv(triangleTransformationShaderProgram.translationLocation, 1, GL_TRUE, &translation.data[0][0]);
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

	glutPostRedisplay();
	glutSwapBuffers();
}

int main(int argCount, char* args[])
{
	GLboolean windowInitResult = InitializeGLWindow(&argCount, args, "5 - Triangle Translation");
	if (windowInitResult == GL_FALSE)
		return -1;

	GLboolean glewInitResult = InitializeGLFunctions();
	if (glewInitResult == GL_FALSE)
		return -1;
	
	GLboolean shaderProgramInitResult = triangleTransformationShaderProgram.Initialize();
	if (shaderProgramInitResult == GL_FALSE)
		return -1;
	
	InitializeGLSettings();
	InitializeTriangleVertexData();
	glutDisplayFunc(OnDisplay);

	glutMainLoop();
	return 0;
}