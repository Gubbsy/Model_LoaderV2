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
#include "FileReader.h"
#include "Model.h"

// to use this example you will need to download the header files for GLM put them into a folder which you will reference in
// properties -> VC++ Directories -> libraries

// Each attribute matches to a location in the vertex shader (for either postion, colour or texture)
enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };
using namespace std;

GLuint shader;
Model CurrentModel;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void
init(void)
{
	// bind textures on corresponding texture units
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

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

void
display (vector<Model>& _mods)
{

	//glClearBufferfv(GL_COLOR, 0, black);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);


	for (int i = 0; i < _mods.size(); i++) {
		_mods[i].Draw(shader);
	}
}

int
main(int argc, char** argv)
{
	vector<Model> models;
	Model mod;
	Model mod2;
	string modelPath = "models/creeper/creeper3.obj";
	string modelPath2 = "models/creeper/creeper2.obj";

	cout << "enter the relative file path to your model -->  ./";
	//cin >> modelPath;

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Textured Cube", NULL, NULL);
	CurrentModel = mod2;

	glfwMakeContextCurrent(window);
	glewInit();
	
	FileReader* fileReader = new FileReader();
	models.push_back(fileReader->ReadFile(modelPath));
	models.push_back(fileReader->ReadFile(modelPath2));
	

	cout << "Mods sixe: " << models.size() << endl;

	init();

	glfwSetKeyCallback(window, key_callback);

	while (!glfwWindowShouldClose(window))
	{
		//uncomment to draw only wireframe 
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		display(models);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_REPEAT) {
		cout << "W pressed" << endl;
		CurrentModel.Translate(glm::vec3(-1.0f, 0.f, 0.f));
	}

	if (key == GLFW_KEY_A && action == GLFW_REPEAT)
		cout << "A pressed" << endl;

	if (key == GLFW_KEY_S && action == GLFW_REPEAT)
		cout << "S pressed" << endl;

	if (key == GLFW_KEY_D && action == GLFW_REPEAT)
		cout << "D pressed" << endl;
}