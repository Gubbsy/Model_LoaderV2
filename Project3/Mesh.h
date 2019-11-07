#pragma once
#include <GL\glew.h>
#include <vector>


#define BUFFER_OFFSET(offset) ((void*)(offset))

using namespace std;

class Mesh
{
private:
	enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

	GLuint texture1;
	GLuint shader;

	GLuint VAO;
	GLuint indicesEBO;
	GLuint coloursVBO;
	GLuint texturesVB0;
	GLuint verticesVB0;

	vector<GLfloat> vertices;
	vector<GLuint> indices;
	vector<GLfloat> colours;
	vector<GLfloat> textures;


	void BindIndices();
	void BindVertices();
	void BindColours();
	void BindTextures();

	void ApplyTexture();

public: 
	Mesh(GLuint* shaderProgram);
	void Draw();
	
	
};

