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

class ObjReader
{
private:

	string mtLib;
	string relFolderTree;

	string file;

	vector<vec3> vertices;
	vector<vec2> textures;
	vector<vec3> normals;

	vector<Vertex> vertexes;
	vector<GLuint> indices;

	map<string, Material> materialsMap;

public:
	Model* ReadFile(string _file);
	void ResetReader();
	void ConstructFolderTree();
	void LoadMaterials();
	void SplitOnSpace(vector<string>& token, string& stringToSplit);
};

