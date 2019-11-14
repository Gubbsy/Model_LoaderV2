#pragma once
#include <string>
#include <glm\ext\vector_float3.hpp>
#include <GL\glew.h>


using namespace std;
using namespace glm;

class Material
{

private:
	string materialName;
	vec3 ambientCol;
	vec3 diffuseCol;
	vec3 specularCol;
	GLfloat specularWeight;
	GLfloat diffuse;
	string mapKD;
	string mapD;
	
public:
	void SetName(string _matrialName);
	void SetAmbientCol(vec3 _ambientColor);
	void setDiffuseCol(vec3 _diffuseCol);
	void SetSpecularCol(vec3 _specularCol);
	void SetSpecularWeight(GLfloat _specularWeight);
	void SetDifuse(GLfloat _diffuse);
	void SetMapKD(string _mapKD);
	void SetMapD(string _mapD);
	
	string GetMapD();
	vec3 GetDiffuseCol();
};

