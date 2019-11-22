#pragma once
#include <string>
#include <vector>
#include "GLFW/glfw3.h"



struct Input {
	std::string semantic;
	std::string sourceID;
	GLuint offSet;
	std::vector<GLfloat> data;
	
	Input(std::string _semantic, std::string _sourceID, GLuint _offSet, std::vector<GLfloat> _data) {
		this->semantic = _semantic;
		this->sourceID = _sourceID;
		this->offSet = _offSet;
		this->data = _data;
	}
};