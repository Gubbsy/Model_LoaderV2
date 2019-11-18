#pragma once
#include <map>
#include <string>
#include <vector>
#include <GL\glew.h>
#include "Model.h"
#include "Input.h"

class DaeReader
{
private:
	std::map<std::string, std::vector<GLfloat>> sources;
	std::string fileString;
	std::vector<Input> inputs;

public:
	Model* ReadFile(string _file);
};

