#include "FileReader.h"
#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;

vector<GLfloat> outVertices;
vector<GLfloat> outTextures;
vector<GLuint> outIndices;


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

				outVertices.push_back(vec1);
				outVertices.push_back(vec2);
				outVertices.push_back(vec3);

			}

			if (token[0] == "vt") {
				float vec1 = ::atof(token[1].c_str());
				float vec2 = ::atof(token[2].c_str());

				outTextures.push_back(vec1);
				outTextures.push_back(vec2);
			}

			if (token[0] == "f") {
				for (int i = 1; i < token.size(); i++) {
					int indices  = token[i][0] - '0';
					outIndices.push_back(indices);
					
				}
			}

			/*if (token[0] == "vn") {

				float vec1 = ::atof(token[1].c_str());
				float vec2 = ::atof(token[2].c_str());
				float vec3 = ::atof(token[3].c_str());

				glm::vec3 normal(vec1, vec2, vec3);
				outNormals.push_back(normal);
			}*/
		}

		GenVertices();

		/*for (int i = 0; i < outIndices.size(); i++) {
			cout << outIndices[i] << " outIndices: " << endl;
		}*/
		

	}
	else {
		cout << "Cannot open File" << endl;
	}

}

void FileReader::GenVertices() {

	int numStartVert = outVertices.size();

	cout << "Number of vertices before addition" << outVertices.size() << endl;

	//Generate new vertices

			/* For (i = numStartVert + 1; i < textures.size(); i++)
			  vertIndex = (numStartVert + indices[i]) & 8
			  outVertices.pushBack(vertices[verIndex])*/

	for (int i = numStartVert; i < outTextures.size(); i++) {
		int vertIndex =  outIndices[(numStartVert + i) % numStartVert ];
		outVertices.push_back(outVertices[vertIndex]);
	}

	cout << " Number of vertices after addition" << outVertices.size() << endl;
	cout << "Number of textures" << outTextures.size() << endl;

	for (int i = 0; i < outVertices.size(); i++) {
		cout << i << " "<< outVertices[i] << endl;
	}

}