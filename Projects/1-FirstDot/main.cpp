/*
-- What is OpenGL?
Platform-Independent Renderer API (Application Program Interface) Specification Standart that lets you standardize communication with your Graphic Driver OpenGL Codes (which you just hate to download when you get a new PC).
Think "specification standart" as "YOU MUST IMPLEMENT THAT USING THIS WAY"
OpenGL codes are unique and inside the Graphic Driver you just installed.
So the graphic driver decides which feature to support.
This is where OpenGL differs from others:
The driver manages the data.
You as a developer, must use identifiers (HANDLES) to identify any data but YOU MUST NOT CARE ABOUT GPU DATA MANAGEMENT.
To show something on the screen, you need to use and compose the primitives (dot, line, triangle...) to create a bigger composed object.
The fastest primitive is the triangle and they optimized triangle for GPU especially.


-- How OpenGL Works?
Oftenly, you will hear "OpenGL works with states so its a state machine". OK I GOT IT BUT HOOOOOOOW THE HELL IT WORKS?
Imagine OpenGL as a workshop (context/data).
While working in this workshop, you have tools (objects/inner datas) that you can use.
Each tool has a number (name/identifier).
Before performing an operation, you must grab (bind/use) the tool in your hand (target).
When you're done, you can either put somewhere (unbind/put it down) or discard it (delete/trash it) or use another tool (unbind/put it down previous -> bind/use new).
(Also, when you request deletion of an identifier (HANDLE), that means the data is deleted too. (we will seeeeeeee))


-- OpenGL Context (Room)
Its a data for which OpenGL runs. (Whenever you hear the word "Context", it means it is a "Data". FANCY WORD, GREAT YEAHh)
Without this data, OpenGL functions wont work since there is no data at all; used by which GPU, using which OpenGL version, decide which window to draw, store object data and manage access, etc...
You will request Graphic Driver to create a context and the driver will return the HANDLE/IDENTIFIER to you and driver will manage after it. (CONTEXT's DADDY IS DRIVER, NOT YOU)
Only 1 OpenGL Context can be active per thread in a process. (RING RING, IMPORTANT!!) (SEARCH ABOUT PROCESS AND THREADS IF YOU DONT KNOW HOW AN APP RUNS)
You will just select which context (data) will be active at the current thread.


-- Initialize OpenGL
The first phase is the creation of an OpenGL Context; the second phase is to load all of the freaking OpenGL functions for that context.

1) OpenGL Context Creation
Every OS (Operating System) has its own system for window, input, event, file etc...
Because each OS API is not standardized, the initialization (connection between context<->OS<->GPU Driver Code) will differ.
If we want just abstraction to one initialization, we use some kind of window and input management library that works with OpenGL.
Ofc, you must use these libraries for education, because they are covering only fundamental things you need.

1) FreeGlut - Library (USING HEREE)
2) GLFW - Library

Assume we created a window.
Every FreeGlut window have its own context HANDLE/IDENTIFIER. (NOT REFERENCE!!!)
Now, the events of OS<->GPU connected to OpenGL Context. (so to your window) (context<->OS<->GPU)

2) Loading Extensions (Functions of OpenGL)
Remember, OpenGL just a specification standart and actual codes are inside the GPU Driver.
The OpenGL driver code are unique, but the access path (interface) to the driver code is standardized by the operating system.
This way, we can request the driver OpenGL code functions from inside of the OS.
For fun, the driver can have absurd methods inside it's OpenGL code which you can load!!
So standardized access path on OS, lets you connect with different OpenGL code on the Graphic Driver.
If it wasnt standardized and if we didnt use any runtime loading, every Driver Code could been unflexible because it would have been required to use the DLL & LIB. (Huh? DO YOU want to update LIB file that references to OpenGL DLL in your project every time you update your driver? Hell na)
So, to make use of OpenGL Context, we must load the function pointers from the driver code into OUR CODE at runtime.
Because each OS's access path is not the same, the load will differ for each OS.
We can load functions manually but its a headache.
If we want to abstract to one load, we use a library (often called as 'Extension Loader Library') and blame the driver developers if anything goes wrong.
You may ask: "Why just context wont let us access the Driver code?" No. Context is a data, not an interface.


-- OpenGL Object (Tool)
Data at GPU.
First, you create an identifier for the object. (and NOT THE OBJECT ITSELF THAT IS FILLED WITH DATA!)
Rememeber, we dont care about GPU data management.
We just assume, we got an identifier for somewhere in GPU data.
So you got an identifier and the real object data managed inside the Context (In GPU).
But the data inside object is empty, so you have the identifier only.
Even tho we have the identifier, its just imaginary.
We must somehow put data inside the object but we can only find the actual data place with a target (hand).
So we bind (use) the object (tool) inside a target (hand) to create the actual object.
Now we can finally put any data inside of the object (tool) using a target (hand) since we are using that object (tool) that we found by an identifier.
EEEEYYYY! DONT FORGET TO UNBIND OR DELETE THE OBJECT IN UR HANDDDDD.


- Target (Hand)
Every target is a ****SINGLE SLOT FOR SINGLE OBJECT****. (example: GL_ARRAY_BUFFER)
There may be thousands of objects but you are capable of using one at a time. (This is what the State Machine means here)


-- Vertex
A point/dot.
It's attributes are arbitrary.
Because of being arbitrary, we have Vertex Array and Vertex Buffer object to use those arbitrary stuff in other places (SUCH AS A SHAHAAAAADER).
Only in pipeline vertex gets meaning, like life does.

- Vertex Array Object
Series of attributes. (example: position, color, normal...)
Stores which Vertex Buffer Object and it's data will be used for each attribute.
.
- Vertex Buffer Object
Stores actual vertex data.


-- Primitive
The main shapes that you will use no matter what.
OR The meaning of: What a list of vertex data you just created actually means?
(line? dot? triangle?)

- Patch
A primitive with a user-defined number of vertices.


-- Shader
There is two meanings. (WHY DOESNT THE PEOPLE TELL THEM TO YOUUUU! OMG)
Shader: User-defined program in the Rendering Pipeline that runs on GPU. (example: Vertex Shader, Tesellation Shaders, Geometry Shader...)
Shader Program: Composed of Shaders that you created. (example: COMPOSE OF Vertex Shaders, Tesellation Shaders, Geometry Shaders...)
That way, we are able to use different SHADER PROGRAM for different PRIMITIVE/VERTEX data.


-- Fragment
A piece that is created from primitive that is a candidate to be a pixel.
Holds some informations:
	Color (interpolated)
	Texture coordinates
	Normals
	Depth (Z value)...


-- Rendering Pipeline
Sequence of steps for converting all vertex data to one image on your screen.
A copy of data taken from your data and and each stage updates the copied data.
So your actual data is untouched unless you use Transform Feedback, Compute Shader or Shader Storage Buffer Object (SSBO).

1) Vertex Specification
Creating vertex datas: Array Element Buffer (index buffer), Vertex Buffers, Transform Feedback Buffers.
You will send vertex datas to Vertex Shader using Vertex Buffer Object in format of Vertex Array Object by calling draw commands.

2) Vertex Shader
Input: 1 vertex data.
Process: Manipulate input.

3) Tessellation Shader (OPTIONALLL)
Input: Primitive Patch.
Process:
	Divide input into smaller primitives to obtain a more detailed geometry.
	Tessellation Control Shader: To how many and which main primitives can the current input can be subdivided to?
	Tessellation Evaluation Shader: Creates new vertex data or primitive(s).

4) Geometry Shader (OPPPPTIONAL)
Input: Primitive.
Process: Manipulate input or create new Primitive(s). (example: A point can be the CENTER of a particle system)

5) Transform Feedback
Input: The 'out' variables you specified in any Shader.
Process: Updates Vertex Buffer Object value that is on your side.

6) Primitive Assembly (IF NEW VERTEX HAD BEEN CREATED)
Input: Vertex Datas with specificied format in draw call. (example: GL_TRIANGLE)
Process: Grouping vertex data to primitives as you told in your program.

7) Clipping, Perspective Divide and Viewport Transform
Input: Primitive in Clip Space.
Process:
	Crop anything outside the camera's field of view.
	Calculate Normalized Device Coordinates (NDC) of input. (normalized position of the vertex between -1 and 1 where 0 assumed as middle)
	Convert Normalized Device Coordinates to Viewport Coordinates (Window).

8) Rasterizer
Input: Primitive that holds the positions for Viewport Coordinates.
Process:
	Create fragments from input.

9) Fragment Shader
Input: Fragment
Process: Manipulate or discard input.

10) Pre-Sample Processing
Input: Fragment
Process:
Do some tests.
If fails, DISCARD BABY
	Scissor Test
	Stencil Test
	Depth Test
	Blending
	Logical Operation
	Write Mask...

Now the fragments will be used in Frame Buffer to generate actual pixells.


-- Glossary
- glutSetWindow
Set the active context for current thread.

- GL_COLOR_BUFFER_BIT
Mask that tells "its a FREAKING COLOR BUUUUUUUUUUfFER".

- glClear
Clear the active context.

- glClearColor
Set the color of cleared context.

- glGenXXXX
Create identifier for specific target.

- glBindXXX
Use identifier data in specific target.

- glBufferData
Set whole data for the buffer identifier.

- glBufferSubData
Update part of the data for the buffer identifier.

- glGenVertexArrays
Create identifier for vertex array(s).

- glBindVertexArray
Use identifier data as vertex array.

- glBindVertexBuffer
Use THAT Vertex Buffer Object when THAT binding-index is used.
Damn it to whoever thought its a good idea but, you need to tell what is the format of the Vertex Buffer Object.

- glVertexAttribBinding
Use THAT Vertex Buffer Object when THAT attribute is used. (n -> vbo)
If not used, (n -> attribute)

- glVertexAttribFormat
For THAT attribute, tell where is the data and how data is stored at the Vertex Buffer Object.

- glEnableVertexAttribArray
Set state of attribute to ENABLEEEEEEEEEEEED.

- GL_ARRAY_BUFFER
Target for vertex data.
Must be used with Vertex Array Object.

- glDrawArrays
Reads from GL_ARRAY_BUFFER and passes to pipeline.

- Interpolated
The calculated value.
Example: in a triangle there is 3 vertex data and each of it has different colors. But only one color will be out. So the middle color will be INTERPOLATED/CALCULATED FROM IT.

*/

#include <stdio.h>
#include <glew.h>
#include <glut.h>
#include <tutorial_glmath.h>

GLuint vertexBufferObjectID;
GLuint vertexArrayObjectID;


// Initialize
int InitializeWindowWithGlut(int& argCount, char* args[], const char* title)
{
	glutInit(&argCount, args);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);

	return glutCreateWindow(title);
}

GLenum InitializeGLContextForCurrentThread()
{
	return glewInit();
}

void printInitConfig()
{
	printf("Active windowId: %d \n", glutGetWindow());
	printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
	printf("Using OpenGL %s \n", glGetString(GL_VERSION));
	printf("\n");
}

void InitializeDotVertexBuffer()
{
	tutorial::vec3 positions[1] = { tutorial::vec3() };

	glGenBuffers(1, &vertexBufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &vertexArrayObjectID);
	glBindVertexArray(vertexArrayObjectID);
	glBindVertexBuffer(0, vertexBufferObjectID, 0, sizeof(tutorial::vec3));
	glVertexAttribBinding(0, 0);
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}


// Update
void DrawDot()
{
	glBindVertexArray(vertexArrayObjectID);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
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

	windowIdResult = InitializeWindowWithGlut(argCount, args, "1 - First Dot");
	if (windowIdResult <= 0)
	{
		fprintf(stderr, "Error: %s \n", "Unknown");
		return -1;
	}

	glewInitResult = InitializeGLContextForCurrentThread();
	if (glewInitResult != GLEW_OK)
	{
		fprintf(stderr, "Error: %s \n", glewGetErrorString(glewInitResult));
		return -1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	InitializeDotVertexBuffer();
	printInitConfig();

	glutDisplayFunc(OnDisplay);
	glutMainLoop();
	return 0;
}