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
	enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2, nPosition = 3 };

	GLuint texture1;
	GLuint shader;

	std::vector<Vertex> vertexes;
	std::vector<GLuint> indices;

	GLuint VAO;
	GLuint VBO;
	GLuint indicesEBO;

	Material material;
	std::string folderTree;
	std::string texturePath;

	void BindIndices();
	void BindVertices();
	void ApplyTexture();

public: 
	void Init(std::vector<Vertex>& _vertexes, std::vector<GLuint>& _indices, Material& _material, string& _fileTree);
	void PassToShader();
	void Draw(GLuint& shaderProgram);
	void Delete();
	bool exists(const std::string& name);
};

