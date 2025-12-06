#include <glew.h>
#include <glut.h>
#include <tutorial_glmath.h>
#include <string>
#include <tutorial_glutils.h>
#include <iostream>
#include <random>



// Macros
#define coutWorkStart() std::cout << __FUNCTION__ << " started. \n";
#define coutWorkSuccess() std::cout << __FUNCTION__ << " succeded. \n\n";
#define cerrWorkFail() std::cerr << __FUNCTION__ << " failed. \n\n";



// Helpers
GLboolean InitializeShaderObject(GLuint* shaderId, GLenum shaderType, const std::string shaderFilePath)
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

GLboolean InitializeShaderProgramObject(GLuint* shaderProgramObjectId, const size_t shadersLength, const GLuint* shaders, const std::string arbitraryProgramName)
{
	coutWorkStart();
	std::cout << "Creating: " << arbitraryProgramName << "\n";

	GLint isShaderProgramCompiled = 0;
	*shaderProgramObjectId = tutorial::CreateCompiledShaderProgramObject(shadersLength, shaders, &isShaderProgramCompiled);
	if (*shaderProgramObjectId == 0)
	{
		std::cout << "Creation failed.";
		cerrWorkFail();
		return GL_FALSE;
	}

	if (isShaderProgramCompiled == 0)
	{
		GLchar temporaryInfoLog[1024];
		glGetProgramInfoLog(*shaderProgramObjectId, 1024, NULL, temporaryInfoLog);

		std::cout << "Compilation failed because: \n" << temporaryInfoLog << "\n";
		cerrWorkFail();
		return GL_FALSE;
	}

	GLint isShaderProgramValid = 0;
	glValidateProgram(*shaderProgramObjectId);
	glGetProgramiv(*shaderProgramObjectId, GL_VALIDATE_STATUS, &isShaderProgramValid);
	if (isShaderProgramValid == 0)
	{
		GLchar temporaryInfoLog[1024];
		glGetProgramInfoLog(*shaderProgramObjectId, 1024, NULL, temporaryInfoLog);

		std::cout << "Validation failed because: \n" << temporaryInfoLog << "\n";
		cerrWorkFail();
		return GL_FALSE;
	}

	coutWorkSuccess();
	return GL_TRUE;
}



// Definitions



// Variables
int windowWidth = 400;
int windowHeight = 300;

std::mt19937 randomGeneratorEngine = std::mt19937(12345);

struct Camera
{
	tutorial::quat rotation = tutorial::quat();
	tutorial::vec3 translation = tutorial::vec3();

	float FOVyInDegree = 90.0f;
	float nearZ = 1.0f;
	float farZ = 10.0f;


	tutorial::mat4x4 GetProjection() const
	{
		return tutorial::GetPerspectiveProjectionMatrix(FOVyInDegree, (GLfloat)windowWidth / (GLfloat)windowHeight, nearZ, farZ);
	}

	tutorial::mat4x4 GetView() const
	{
		tutorial::mat4x4 inverseTranslation = tutorial::mat4x4(
			1.0f, 0.0f, 0.0f, -translation.x,
			0.0f, 1.0f, 0.0f, -translation.y,
			0.0f, 0.0f, 1.0f, -translation.z,
			0.0f, 0.0f, 0.0f, 1.0f);

		return inverseTranslation * rotation.GetInverse().GetMatrix();
	}
} camera;

class
{
private:
	struct Vertex
	{
		tutorial::vec3 worldPosition;
		tutorial::vec3 diffuseColor;

		Vertex()
			: Vertex(tutorial::vec3(), tutorial::vec3(1.0f, 1.0f, 1.0f))
		{
		}

		Vertex(const tutorial::vec3& worldPosition, const tutorial::vec3& diffuseColor)
			: worldPosition(worldPosition), diffuseColor(diffuseColor)
		{
		}
	};


	GLuint VBOId = 0; // Vertex Buffer Object
	GLuint VEOId = 0; // Vertex Array Object
	GLuint VAOId = 0; // Vertex Element Object
	GLuint shaderProgramObjectId = 0;

	GLint PVMLocation = -1; // Projection, View, Model matrix


	GLboolean InitializeVBO()
	{
		coutWorkStart();
		std::uniform_real_distribution<GLfloat> randomRangeGenerator = std::uniform_real_distribution<GLfloat>(0.0f, 1.0f);
		Vertex vertexes[8];

		for (size_t i = 0; i < 8; i++)
		{
			vertexes[i].diffuseColor = tutorial::vec3(
				randomRangeGenerator(randomGeneratorEngine),
				randomRangeGenerator(randomGeneratorEngine),
				randomRangeGenerator(randomGeneratorEngine)
			);
		}

		vertexes[0].worldPosition = tutorial::vec3(0.5f, 0.5f, 0.5f);
		vertexes[1].worldPosition = tutorial::vec3(-0.5f, 0.5f, -0.5f);
		vertexes[2].worldPosition = tutorial::vec3(-0.5f, 0.5f, 0.5f);
		vertexes[3].worldPosition = tutorial::vec3(0.5f, -0.5f, -0.5f);
		vertexes[4].worldPosition = tutorial::vec3(-0.5f, -0.5f, -0.5f);
		vertexes[5].worldPosition = tutorial::vec3(0.5f, 0.5f, -0.5f);
		vertexes[6].worldPosition = tutorial::vec3(0.5f, -0.5f, 0.5f);
		vertexes[7].worldPosition = tutorial::vec3(-0.5f, -0.5f, 0.5f);

		glGenBuffers(1, &VBOId);
		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		coutWorkSuccess();
		return GL_TRUE;
	}

	GLboolean InitializeVEO()
	{
		coutWorkStart();

		GLuint elements[][3] = {
			{0, 1, 2},
			{1, 3, 4},
			{5, 6, 3},
			{7, 3, 6},
			{2, 4, 7},
			{0, 7, 6},
			{0, 5, 1},
			{1, 5, 3},
			{5, 0, 6},
			{7, 4, 3},
			{2, 1, 4},
			{0, 2, 7},
		};

		glGenBuffers(1, &VEOId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEOId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		coutWorkSuccess();
		return GL_TRUE;
	}

	GLboolean InitializeVAO()
	{
		coutWorkStart();

		if (VBOId <= 0)
		{
			std::cout << "VBOId not initialized. \n";
			cerrWorkFail();
			return GL_FALSE;
		}

		if (VEOId <= 0)
		{
			std::cout << "VEOId not initialized. \n";
			cerrWorkFail();
			return GL_FALSE;
		}

		glGenVertexArrays(1, &VAOId);
		glBindVertexArray(VAOId);
		glBindVertexBuffer(0, VBOId, 0, sizeof(Vertex));
		glVertexAttribBinding(0, 0);
		glVertexAttribBinding(1, 0);
		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, sizeof(tutorial::vec3));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexArrayElementBuffer(VAOId, VEOId);
		glBindVertexArray(0);

		coutWorkSuccess();
		return GL_TRUE;
	}


public:
	tutorial::vec3 scale = tutorial::vec3(1.0f, 1.0f, 1.0f);
	tutorial::quat rotation = tutorial::quat();
	tutorial::vec3 translation = tutorial::vec3();


	GLboolean Initialize()
	{
		coutWorkStart();

		GLboolean initializationResult = InitializeVBO();
		if (initializationResult == GL_FALSE)
		{
			cerrWorkFail();
			return GL_FALSE;
		}

		initializationResult = InitializeVEO();
		if (initializationResult == GL_FALSE)
		{
			cerrWorkFail();
			return GL_FALSE;
		}

		initializationResult = InitializeVAO();
		if (initializationResult == GL_FALSE)
		{
			cerrWorkFail();
			return GL_FALSE;
		}

		GLuint shaders[2] = { 0, 0 };
		initializationResult = InitializeShaderObject(&shaders[0], GL_VERTEX_SHADER, "cube.vert");
		if (initializationResult == GL_FALSE)
		{
			cerrWorkFail();
			return GL_FALSE;
		}

		initializationResult = InitializeShaderObject(&shaders[1], GL_FRAGMENT_SHADER, "cube.frag");
		if (initializationResult == GL_FALSE)
		{
			cerrWorkFail();
			return GL_FALSE;
		}

		initializationResult = InitializeShaderProgramObject(&shaderProgramObjectId, 2, shaders, "Cube Shader Program");
		if (initializationResult == GL_FALSE)
		{
			cerrWorkFail();
			return GL_FALSE;
		}

		PVMLocation = glGetUniformLocation(shaderProgramObjectId, "MVP");
		if (PVMLocation < 0)
		{
			std::cout << "Cant find PVMLocation. \n";
			cerrWorkFail();
			return GL_FALSE;
		}

		for (size_t i = 0; i < 2; i++)
			glDeleteShader(shaders[i]);

		coutWorkSuccess();
		return GL_TRUE;
	}

	tutorial::mat4x4 GetModel() const
	{
		tutorial::mat4x4 scaleMatrix = tutorial::mat4x4(
			scale.x, 0.0f, 0.0f, 0.0f,
			0.0f, scale.y, 0.0f, 0.0f,
			0.0f, 0.0f, scale.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		tutorial::mat4x4 rotationMatrix = rotation.GetMatrix();

		tutorial::mat4x4 translationMatrix = tutorial::mat4x4(
			1.0f, 0.0f, 0.0f, translation.x,
			0.0f, 1.0f, 0.0f, translation.y,
			0.0f, 0.0f, 1.0f, translation.z,
			0.0f, 0.0f, 0.0f, 1.0f);

		return translationMatrix * rotationMatrix * scaleMatrix;
	}

	void Draw(tutorial::mat4x4 PVM) const // Model, View, Projection matrix
	{
		glUseProgram(shaderProgramObjectId);
		glUniformMatrix4fv(PVMLocation, 1, GL_TRUE, &PVM.data[0][0]);
		glBindVertexArray(VAOId);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

} cube;



// Initialize
GLboolean InitializeGLWindow(int* argCount, char* args[], const char* title)
{
	coutWorkStart();
	std::cout << "Using FreeGlut API: " << GLUT_API_VERSION << "\n";
	
	glutInit(argCount, args);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
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
	
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << "\n";
	
	if (!GL_VERSION_4_6)
	{
		std::cout << "OpenGL Version 4.6 and above is supported only. \n";
		cerrWorkFail();
		return GL_FALSE;
	}

	coutWorkSuccess();
	return GL_TRUE;
}

GLboolean InitializeGLSettings()
{
	coutWorkStart();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	coutWorkSuccess();
	return GL_TRUE;
}



// Update
void OnDisplay()
{
	static GLfloat rotationDegree = 0.0f;
	static GLfloat rotationDegreeSpeed = 1.0f;

	glClear(GL_COLOR_BUFFER_BIT);

	Sleep(25);

	rotationDegree += rotationDegreeSpeed;
	if (rotationDegree >= 360.0f)
		rotationDegree = 0.0f;

	tutorial::mat4x4 PVM = camera.GetProjection() * camera.GetView() * cube.GetModel(); // Projection, View, Model matrix
	camera.rotation = tutorial::quat().FromAxisAngle(tutorial::vec3(0.0f, 0.0f, -1.0f), rotationDegree);
	cube.translation = tutorial::vec3(0.0f, 0.0f, -2.0f);
	cube.rotation = tutorial::quat().FromAxisAngle(tutorial::vec3(0.0f, 1.0f, 0.0f), rotationDegree);
	cube.Draw(PVM);

	glutPostRedisplay();
	glutSwapBuffers();
}

void OnWindowSizeChanged(int width, int height)
{
	windowWidth = width;
	windowHeight = height;

	glViewport(0, 0, windowWidth, windowHeight);
}

int main(int argCount, char* args[])
{
	std::random_device randomSeedGenerator = std::random_device();
	randomGeneratorEngine = std::mt19937(randomSeedGenerator());

	GLboolean initializationResult = InitializeGLWindow(&argCount, args, "12 - Camera");
	if (initializationResult == GL_FALSE)
		return -1;

	initializationResult = InitializeGLFunctions();
	if (initializationResult == GL_FALSE)
		return -1;

	initializationResult = InitializeGLSettings();
	if (initializationResult == GL_FALSE)
		return -1;
	
	initializationResult = cube.Initialize();
	if (initializationResult == GL_FALSE)
		return -1;
	
	glutDisplayFunc(OnDisplay);
	glutReshapeFunc(OnWindowSizeChanged);

	std::cout << "FreeGlut is controlling the system now. \n";
	glutMainLoop();
	return 0;
}