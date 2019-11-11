#include "Object.h"

void Object::AddMesh(Mesh& _mesh)
{
	meshes.push_back(_mesh);
}

void Object::Draw(GLuint shader)
{
	// for every mesh in object
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
}
