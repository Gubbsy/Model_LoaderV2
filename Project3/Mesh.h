#pragma once
#include <GL\glew.h>
#include <vector>
#include "Material.h"
#include "Vertex.h"
#include <string>
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

	Material material;

	bool canReadFile;

	void BindIndices();
	void BindVertices();
	void ApplyTexture();

public: 
	Mesh();
	void Draw(GLuint& shaderProgram);
	void AddVertexes(std::vector<Vertex>& _vertexes);
	void AddIndices(std::vector<GLuint>& _indices);
	void AddMaterial(Material _material);
};

