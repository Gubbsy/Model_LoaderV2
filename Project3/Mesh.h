#pragma once
#include <GL\glew.h>
#include <vector>
#include "Vertex.h"


#define BUFFER_OFFSET(offset) ((void*)(offset))

using namespace std;

class Mesh
{
private:
	enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

	GLuint texture1;
	GLuint shader;

	vector<Vertex> vertexes;
	vector<GLuint> indices;

	GLuint VAO;
	GLuint VBO;
	GLuint indicesEBO;


	void BindIndices();
	void BindVertices();

	void ApplyTexture();

public: 
	Mesh(GLuint* shaderProgram);
	void Draw();
	
	
};

