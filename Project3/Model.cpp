#include "Model.h"
#include <glm\ext\matrix_transform.hpp>
#include <GLFW\glfw3.h>
#include <glm\ext\matrix_clip_space.hpp>
#include <glm\gtc\type_ptr.hpp>

Model::Model()
{
}

void Model::AddObject(Object object)
{
	objects.push_back(object);
}

void Model::Draw(GLuint& shader)
{
	glm::mat4 model = glm::mat4(1.0f);

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

	// for every object in model call draw method
	for (int i = 0; i < objects.size(); i++) {
		objects[i].Draw(shader);
	}
}
