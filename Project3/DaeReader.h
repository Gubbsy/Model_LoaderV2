#pragma once
#include <map>
#include <string>
#include <vector>
#include <GL\glew.h>
#include "Model.h"
#include "Input.h"

class DaeReader
{
private:
	std::map<std::string, std::vector<GLfloat>> sources;
	std::string fileString;
	std::vector<GLuint> vertexDefs;
	std::vector<Input> inputs;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;


public:
	Model* ReadFile(string _file);
	void PullSource(string _fileString);
	void PullInputs(string _fileString);
	void PullVertextDef(string _flleString);
	void GenerateVertices();
	void GenerateIndices();
};

