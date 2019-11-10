#include "FileReader.h"
#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;

vector<GLfloat> outVertices;
vector<GLfloat> outTextures;
vector<GLuint> outIndices;




void FileReader::ReadFile(const char* file, vector<Vertex>& vertexes, vector<GLuint>& indices) {

	int indicesOffSet = 0;

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
				float vecX = ::atof(token[1].c_str());
				float vecY = ::atof(token[2].c_str());
				float vecZ = ::atof(token[3].c_str());

				vertices.push_back(glm::vec3(vecX, vecY, vecZ));

			}

			if (token[0] == "vt") {
				float vecS = ::atof(token[1].c_str());
				float vecT = ::atof(token[2].c_str());

				textures.push_back(glm::vec2(vecS, vecT));

			}

			if (token[0] == "vn") {

				float vecX = ::atof(token[1].c_str());
				float vecY = ::atof(token[2].c_str());
				float vecZ = ::atof(token[3].c_str());

				normals.push_back(glm::vec3(vecX, vecY, vecZ));
			}


			if (token[0] == "f") {
				vector<string> verts;

				for (int i = 1; i < token.size(); i++) {
					string faceLine = "";
					vector<string> faceToken;
					istringstream faceIss(token[i]);

					int index_counter = 0;

					glm::vec3 temp_pos = glm::vec3(0.0f);
					glm::vec2 temp_tex = glm::vec2(0.0f);
					glm::vec3 temp_norm = glm::vec3(0.0f);

					while (getline(faceIss, faceLine, '/')) {

						//if 0 fetch from vertex
						if (index_counter == 0) {
							temp_pos = vertices[stoi(faceLine) -1];
						}
						// if 1 fecthc from texture
						else if (index_counter == 1) {
							temp_tex = textures[stoi(faceLine) -1];
						}
						// if 2 fetch from normal
						else if (index_counter == 2) {
							temp_norm = normals[stoi(faceLine) -1];
						}
						
						index_counter++;
					}

					vertexes.push_back(Vertex(temp_pos, temp_tex, temp_norm));
				}

				if (token.size() - 1 == 4) {
					indices.push_back(2 + indicesOffSet);
					indices.push_back(1 + indicesOffSet);
					indices.push_back(0 + indicesOffSet);
					indices.push_back(3 + indicesOffSet);
					indices.push_back(2 + indicesOffSet);
					indices.push_back(0 + indicesOffSet);
					indicesOffSet = indicesOffSet + 4;
					cout << "IndicesOffset: " << indicesOffSet << endl;
				}
				else if (token.size() - 1 == 3) {
					indices.push_back(2 + indicesOffSet);
					indices.push_back(1 + indicesOffSet);
					indices.push_back(0 + indicesOffSet);
					indicesOffSet = indicesOffSet + 3;
				}
			}
		}

	/*	indices.push_back(2);
		indices.push_back(1);
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);
		indices.push_back(0);
		
		indices.push_back(2 + 4);
		indices.push_back(1 + 4);
		indices.push_back(0 + 4);
		indices.push_back(3 + 4);
		indices.push_back(2 + 4);
		indices.push_back(0 + 4);
		
		indices.push_back(2 + 8);
		indices.push_back(1 + 8);
		indices.push_back(0 + 8);
		indices.push_back(3 + 8);
		indices.push_back(2 + 8);
		indices.push_back(0 + 8);
		
		indices.push_back(2 + 12);
		indices.push_back(1 + 12);
		indices.push_back(0 + 12);
		indices.push_back(3 + 12);
		indices.push_back(2 + 12);
		indices.push_back(0 + 12);
		
		indices.push_back(2 + 16);
		indices.push_back(1 + 16);
		indices.push_back(0 + 16);
		indices.push_back(3 + 16);
		indices.push_back(2 + 16);
		indices.push_back(0 + 16);
		
		indices.push_back(2 + 20);
		indices.push_back(1 + 20);
		indices.push_back(0 + 20);
		indices.push_back(3 + 20);
		indices.push_back(2 + 20);
		indices.push_back(0 + 20);*/
	}
	else {
		cout << "Cannot open File" << endl;
	}

}