#include "Model.h"
#include <glm\ext\matrix_transform.hpp>
#include <GLFW\glfw3.h>
#include <glm\ext\matrix_clip_space.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>

Model::Model()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1, 1, 1);
	orientation = glm::vec3(-0.35f, 0, 0);

	modelMat = glm::mat4(1.0f);
}

void Model::GenerateModelMatrix()
{
	//Create Model
	modelMat = glm::scale(modelMat, scale);

	modelMat = glm::rotate(modelMat, radians(orientation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::rotate(modelMat, radians(orientation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat = glm::rotate(modelMat, radians(orientation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	modelMat = glm::translate(modelMat, position);
}

void Model::Translate(vec3 _translateBy)
{
	std::cout << "translat called" << endl;
	position = position + _translateBy;
}

void Model::Scale(vec3 _scaleBy)
{
	scale = scale + _scaleBy;
}

void Model::AddObject(Object object)
{
	objects.push_back(object);
}

void Model::Draw(GLuint& shader)
{
	GenerateModelMatrix();

	// creating the view matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));

	// creating the projection matrix
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 1000.0f);

	// Adding all matrices up to create combined matrix
	glm::mat4 mvp = projection * view * modelMat;

	// Pass mvp into shader
	int mvpLoc = glGetUniformLocation(shader, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	// for every object in model call draw method
	for (int i = 0; i < objects.size(); i++) {
		objects[i].Draw(shader);
	}
}
