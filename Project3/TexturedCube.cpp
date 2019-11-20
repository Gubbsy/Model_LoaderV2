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
#include "ObjReader.h"
#include "DaeReader.h"
#include "Model.h"

// to use this example you will need to download the header files for GLM put them into a folder which you will reference in
// properties -> VC++ Directories -> libraries

// Each attribute matches to a location in the vertex shader (for either postion, colour or texture)
using namespace std;

GLuint shader;
uint currentModelIndex;
vector<Model> models;
vector<string> modelPaths;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

//Take User Input
void takeUserInput() {

	//Get user Input 
	string userInput;
	cout << "Enter the relative file path to your model. \n For multiple paths seperate each file with a whitespace  \n -->";
	getline(std::cin, userInput);

	//Split user input on whiet space to get file paths and add them to vector of files paths.
	istringstream iss(userInput);
	for (string s; iss >> s; ) {
		modelPaths.push_back(s);
	}
}

void
init(void)
{
	currentModelIndex = 0;
	// bind textures on corresponding texture units
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	//Assign shaders
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

	//For every model in the scene call thier draw method
	for (int i = 0; i < _mods.size(); i++) {
		_mods[i].Draw(shader);
	}
}

int
main(int argc, char** argv)
{
	//GLEW and GLFW Initialisation
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "Textured Cube", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();
	
	//Call take user input
	takeUserInput();

	//Create Readers
	ObjReader* objReader = new ObjReader();
	DaeReader* daeReader = new DaeReader();

	// For every model file path
	for (int i = 0; i < modelPaths.size(); i++)
	{
		//Intialise empty model
		Model* mod = nullptr;
		//Get File extension
		string extension = modelPaths[i].substr((modelPaths[i].length()) - 3);

		//Pass in file path to apprpriate reader
		if (extension == "obj") {
			mod = objReader->ReadFile(modelPaths[i]);
		}
		else if (extension == "dae") {
			mod = daeReader->ReadFile(modelPaths[i]);
		}
		else {
			cout << "Unsupported file type: " << extension << endl;
		}
		
		//If file could be read then add to list of models
		if (mod != nullptr) {
			models.push_back(*mod);
		}
	}

	//Intitlise shaders and window
	init();
	glfwSetKeyCallback(window, key_callback);

	//Main loop
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

//Togle currently selected model
void toggleCurrentModel() {
	if (models.size() != 0) {
		currentModelIndex = (currentModelIndex + 1) % models.size();
	}
}

// Handle user input
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (models.size() != 0) {
		//WASD - Translate
		if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			models[currentModelIndex].Translate(glm::vec3(0.00f, 0.10f, 0.0f));
		}

		else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			models[currentModelIndex].Translate(glm::vec3(0.10f, 0.0f, 0.0f));
		}

		else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			models[currentModelIndex].Translate(glm::vec3(0.0f, -0.10f, 0.0f));
		}

		else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			models[currentModelIndex].Translate(glm::vec3(-0.10f, 0.0f, 0.0f));
		}

		//+/- - Scale
		else if (key == GLFW_KEY_KP_ADD && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			models[currentModelIndex].Scale(glm::vec3(0.3f, 0.3f, 0.3f));
		}

		else if (key == GLFW_KEY_KP_SUBTRACT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			models[currentModelIndex].Scale(glm::vec3(-0.3f, -0.3f, -0.3f));
		}

		//Arrow keys - Rotate
		else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			models[currentModelIndex].Rotate(glm::vec3(0.0f, 10.0f, 0.0f));
		}

		else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			models[currentModelIndex].Rotate(glm::vec3(0.0f, -10.0f, 0.0f));
		}

		else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			models[currentModelIndex].Rotate(glm::vec3(-10.0f, 0.0f, 0.0f));
		}

		else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			models[currentModelIndex].Rotate(glm::vec3(10.0f, 0.0f, 0.0f));
		}

		// Tab through models
		else if (key == GLFW_KEY_TAB && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			toggleCurrentModel();
		}

		else if (key == GLFW_KEY_BACKSPACE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			int deleteIndex = models.size() - 1;
			models[deleteIndex].Delete();
			models.pop_back();
			toggleCurrentModel();
		}
	}
	//close program
	else if (key == GLFW_KEY_ESCAPE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		exit(0);
	}
}