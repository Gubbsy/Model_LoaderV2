#pragma once
#include <GL\glew.h>
#include <vector>
#include "Vertex.h"
#include <string>

#define BUFFER_OFFSET(offset) ((void*)(offset))


class Mesh
{
private:
	enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

	GLuint texture1;
	GLuint shader;

	std::vector<Vertex> vertexes;
	std::vector<GLuint> indices;

	GLuint VAO;
	GLuint VBO;
	GLuint indicesEBO;

	bool canReadFile;


	void BindIndices();
	void BindVertices();

	void ApplyTexture();

public: 
	Mesh(GLuint* shaderProgram, std::string& modelLoc);
	void Draw();

};

