#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>
#include <glm\ext\vector_float4.hpp>
#include <GL\glew.h>
#include <vector>
#include <string>
#include <fstream>
#include "Vertex.h"

using namespace std;



class FileReader
{
private:

	vector<glm::vec3> vertices;
	vector<glm::vec2> textures;
	vector<glm::vec3> normals;

	

public:
	void ReadFile(const char* file, vector<Vertex>& vertices, vector<GLuint>& indices);
	void LoadMaterials();
};

