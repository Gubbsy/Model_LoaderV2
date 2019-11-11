//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"
#include "LoadShaders.h"
#include <glm/glm.hpp> //includes GLM
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <vector>
#include "Mesh.h"

// to use this example you will need to download the header files for GLM put them into a folder which you will reference in
// properties -> VC++ Directories -> libraries

// Each attribute matches to a location in the vertex shader (for either postion, colour or texture)
enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

GLuint shader;

using namespace std;

//----------------------------------------------------------------------------
//
// init
//

void
init(void)
{

	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "media/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "media/triangles.frag" },
		{ GL_NONE, NULL }
	};

	//Read in shaders and use them
	shader = LoadShaders(shaders);
	glUseProgram(shader);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}


//----------------------------------------------------------------------------
//
// display
//s

void
display (Mesh* mesh)
{

	//glClearBufferfv(GL_COLOR, 0, black);
	glClear(GL_COLOR_BUFFER_BIT);
	// bind textures on corresponding texture units
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//Created matrices
	glm::mat4 model = glm::mat4(1.0f);
	/*model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01));
	model = glm::rotate(model, glm::radians((float)glfwGetTime()* 30) , glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));*/

	model = glm::translate(model, glm::vec3(0.0f, 0.f, 0.0f));
	model = glm::rotate(model, glm::radians((float)glfwGetTime() * 30), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

	// creating the view matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));

	// creating the projection matrix
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 1000.0f);

	// Adding all matrices up to create combined matrix
	glm::mat4 mvp = projection * view * model;


	//adding the Uniform to the shader
	int mvpLoc = glGetUniformLocation(shader, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	mesh->Draw();
}


//----------------------------------------------------------------------------
//
// main
//


int
main(int argc, char** argv)
{
	string modelPath;
	cout << "enter the relative file path to your model: ";
	cin >> modelPath;

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Textured Cube", NULL, NULL);

	glfwMakeContextCurrent(window);
	glewInit();

	Mesh* cubeMesh = new Mesh(&shader, modelPath);

	init();

	while (!glfwWindowShouldClose(window))
	{
		//uncomment to draw only wireframe 
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		display(cubeMesh);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete cubeMesh;

	glfwDestroyWindow(window);

	glfwTerminate();
}
