#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>
#include <glm\ext\vector_float4.hpp>
#include <GL\glew.h>
#include <vector>
#include <string>
#include <fstream>
#include <map> 
#include <iostream>
#include <cstring>
#include <sstream>
#include "Object.h"
#include "Material.h"
#include "Vertex.h"
#include "Model.h"

using namespace std;
using namespace glm;

class FileReader
{
private:

	string file;
	string mtLib;
	string relFolderTree;
	vector<vec3> vertices;
	vector<vec2> textures;
	vector<vec3> normals;

	map<string, Material> materialsMap;
	
	vector<Vertex> vertexes;
	vector<GLuint> indices;

public:
	Model ReadFile(string _file);
	void ConstructFolderTree();
	void LoadMaterials();
	void SplitOnSpace(vector<string>& token, string& stringToSplit);
};

