#pragma once
#include "Object.h"
#include "Material.h"
#include <map>
#include <glm/glm.hpp> //includes GLM

class Model
{
private:
	std::vector<Object> objects;

	mat4 modelMat;
	vec3 position;
	vec3 orientation;
	vec3 scale;

public:
	Model();
	void Translate(vec3 _translateBy);
	void Rotate(vec3 _rotateBY);
	void Scale(vec3 _scaleBy);
	void AddObject(Object object);
	void Draw(GLuint& shader);
};

