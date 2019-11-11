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

public:
	Model();
	void AddObject(Object object);
	void Draw(GLuint& shader);
};

