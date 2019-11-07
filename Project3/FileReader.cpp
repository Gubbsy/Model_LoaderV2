#include "FileReader.h"
#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;


void FileReader::ReadFile(const char* file, vector<GLfloat>* vertices, vector<GLuint>* indices, vector<GLfloat>* textures) {

	string line = "";
	int lineLength;
	ifstream myFile(file);

	if (myFile.is_open()) {

		cout << "Opened File" << endl;
		while (getline(myFile, line)) {

			vector<string> token;
			istringstream iss(line);

			for (string s; iss >> s; ) {
				token.push_back(s);
			}

			if (token[0] == "v") {
				float vec1 = ::atof(token[1].c_str());
				float vec2 = ::atof(token[2].c_str());
				float vec3 = ::atof(token[3].c_str());

				vertices->push_back(vec1);
				vertices->push_back(vec2);
				vertices->push_back(vec3);
			}

			if (token[0] == "vt") {
				float vec1 = ::atof(token[1].c_str());
				float vec2 = ::atof(token[2].c_str());

				textures->push_back(vec1);
				textures->push_back(vec2);
			}

			/*if (token[0] == "vn") {

				float vec1 = ::atof(token[1].c_str());
				float vec2 = ::atof(token[2].c_str());
				float vec3 = ::atof(token[3].c_str());

				glm::vec3 normal(vec1, vec2, vec3);
				outNormals.push_back(normal);
			}*/

		}
	}
	else {
		cout << "Cannot open File" << endl;
	}

}