#include <stdio.h>
#include <glew.h>
#include <glut.h>

struct Vector3f
{
	float x;
	float y;
	float z;

	Vector3f()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3f(float _x, float _y, float _z = 0)
	{
		x = _x;
		y = _y;
		z = _z;
	}
};

GLuint vertexBufferObject;

// Initialize
int InitializeWindowWithGlut(int& argCount, char* args[], const char* title)
{
	glutInit(&argCount, args);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);

	return glutCreateWindow(title);
}

GLenum InitializeGlewContextForCurrentWindow()
{
	GLenum result = glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return result;
}

void printInitConfig()
{
	printf("Active windowId: %d \n", glutGetWindow());
	printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
	printf("\n");
}

void InitDotVertexBuffer()
{
	Vector3f vertices[1];
	vertices[0] = Vector3f();

	// YOU DIDNT UNDERSTOOD THE BUFFERING STUFF HERE. (Allocate, Bind, Use)
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f), vertices, GL_STATIC_DRAW);
}


// Update
void DrawDot()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);


}

void OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Sleep(25);
	DrawDot();

	glutSwapBuffers();
}

int main(int argCount, char* args[])
{
	int windowIdResult;
	GLenum glewInitResult;

	windowIdResult = InitializeWindowWithGlut(argCount, args, "Create Window");
	if (windowIdResult <= 0)
	{
		fprintf(stderr, "Error: %s \n", "Unknown");
		return -1;
	}

	glewInitResult = InitializeGlewContextForCurrentWindow();
	if (glewInitResult != GLEW_OK)
	{
		fprintf(stderr, "Error: %s \n", glewGetErrorString(glewInitResult));
		return -1;
	}

	InitDotVertexBuffer();
	printInitConfig();
	
	glutDisplayFunc(OnDisplay);
	glutMainLoop();
	return 0;
}