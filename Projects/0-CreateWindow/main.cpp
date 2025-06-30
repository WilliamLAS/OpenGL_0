#include <iostream>
#include <stdio.h>
#include <stdexcept>
#include <glew.h>
#include <glut.h>

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


// Update
void IncrementClearColorAlpha()
{
	static GLclampf currentAlpha = 0.0f;

	currentAlpha += 1.0f / 256.0f;
	if (currentAlpha >= 0.5f)
		currentAlpha = 0;
	
	glClearColor(currentAlpha, currentAlpha, currentAlpha, 1.0f);
	glutPostRedisplay();
}

void OnDisplay()
{
	Sleep(25);
	IncrementClearColorAlpha();

	glClear(GL_COLOR_BUFFER_BIT);
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

	printInitConfig();
	glutDisplayFunc(OnDisplay);
	glutMainLoop();
	return 0;
}