#include "Material.h"

void Material::SetName(string _materialName) {
	materialName = _materialName;
}

void Material::SetAmbientCol(vec3 _ambientColor) {
	ambientCol = _ambientColor;
}

void Material::setDiffuseCol(vec3 _diffuseCol) {
	diffuseCol = _diffuseCol;
}

void Material::SetSpecularCol(vec3 _specularCol) {
	specularCol = _specularCol;
}

void Material::SetSpecularWeight(GLfloat _specularWeight) {
	specularWeight = _specularWeight;
}

void Material::SetDifuse(GLfloat _diffuse) {
	diffuse = _diffuse;
}

void Material::SetMapKD(string _mapKD) {
	mapKD = _mapKD;
}

void Material::SetMapD(string _mapD) {
	mapD = _mapD;
}

string& Material::GetMaterial()
{
	return mapD;
}
