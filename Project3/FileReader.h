#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>
#include <glm\ext\vector_float4.hpp>
#include <GL\glew.h>
#include <vector>
#include <string>
#include <fstream>
#include <map> 
#include "Material.h"
#include "Vertex.h"

using namespace std;
using namespace glm;

class FileReader
{
private:

	vector<vec3> vertices;
	vector<vec2> textures;
	vector<vec3> normals;

	map<string, Material> materialsMap;
	

public:
	void ReadFile(const char* file, vector<Vertex>& vertices, vector<GLuint>& indices);
	void LoadMaterials();
};

