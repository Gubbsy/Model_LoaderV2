#include "Face.h"

Face::Face(vector<string> _vertexs) {
	vertexes = _vertexs;
}

int Face::GetNumVertices() {
	return vertexes.size();
}