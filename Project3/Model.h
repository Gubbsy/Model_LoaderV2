#pragma once
#include "Object.h"
#include "Material.h"
#include <map>
#include <glm/glm.hpp> //includes GLM

class Model
{
private:
	std::vector<Object> objects;
	std::map<string, Material> materialsMap;

	mat4 modelMat;
	vec3 position;
	vec3 orientation;
	vec3 scale;

public:
	Model();
	void GenerateModelMatrix();
	void Translate(vec3 _translateBy);
	void Scale(vec3 _scaleBy);
	void AddObject(Object object);
	void Draw(GLuint& shader);
};

