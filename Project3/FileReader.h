#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>
#include <glm\ext\vector_float4.hpp>
#include <GL\glew.h>
#include <vector>
#include <string>
#include <fstream>

using namespace std;
class FileReader
{
private:
	void GenVertices();

public:
	void ReadFile(const char* file, vector<GLfloat>* vertices, vector<GLuint>* indices, vector<GLfloat>* textures);
	
};

