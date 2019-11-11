#include "Model.h"

Model::Model()
{
}

void Model::AddObject(Object object)
{
	objects.push_back(object);
}

void Model::Draw(GLuint shader)
{
	// for every object in model call draw method
	for (int i = 0; i < objects.size(); i++) {
		objects[i].Draw(shader);
	}
}
