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
	std::string relFolderTree;
	std::string file;
	std::string texFile;
	
	Material newMaterial;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	
	

public:
	Model* ReadFile(string _file);
	void ResetReader();
	void PullSource(string _fileString);
	void PullInputs(string _fileString);
	void PullVertextDef(string _fileString);
	void PullTexture(string _fileString);
	void ConstructFolderTree();
	void GenerateVertices();
	void GenerateIndices();
};

