#pragma once
#include <vector>
#include "Mesh.h"

class Object
{
private:
	std::vector<Mesh> meshes;

public:
	void AddMesh(Mesh& _mesh);
	void Draw(GLuint& shader);
	void Delete();
};

