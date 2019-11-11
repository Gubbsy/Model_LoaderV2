#include "FileReader.h"
#include <iostream>
#include <cstring>
#include <sstream>


bool FileReader::ReadFile(string file, vector<Vertex>& vertexes, vector<GLuint>& indices) {

	int indicesOffSet = 0;

	string line = "";
	ifstream myFile(file); 

	if (myFile.is_open()) {

		cout << "Opened File" << endl;
		while (getline(myFile, line)) {

			vector<string> token;
			istringstream iss(line);


			for (string s; iss >> s; ) {
				token.push_back(s);
			}

			if (token[0] == "mtllib") {
				LoadMaterials();
			}

			//Pull ot vertices
			if (token[0] == "v") {
				vertices.push_back(vec3(stof(token[1]), stof(token[2]), stof(token[3])));

			}

			//Pull out textures
			if (token[0] == "vt") {
				textures.push_back(vec2(stof(token[1]), stof(token[2])));
			}

			//Pull out normals
			if (token[0] == "vn") {
				normals.push_back(vec3(stof(token[1]), stof(token[2]), stof(token[3])));
			}

			//Pull out faces
			if (token[0] == "f") {
				vector<string> verts;
				
				//for every set of vertex indices
				for (int i = 1; i < token.size(); i++) {
					string faceLine = "";
					vector<string> faceToken;
					istringstream faceIss(token[i]);

					int index_counter = 0;

					vec3 temp_pos = vec3(0.0f);
					vec2 temp_tex = vec2(0.0f);
					vec3 temp_norm = vec3(0.0f);

					while (getline(faceIss, faceLine, '/')) {

						//if 0 fetch vertices
						if (index_counter == 0) {
							temp_pos = vertices[stoi(faceLine) -1];
						}
						// if 1 fetch from texture
						else if (index_counter == 1) {
							temp_tex = textures[stoi(faceLine) -1];
						}
						// if 2 fetch from normal
						else if (index_counter == 2) {
							temp_norm = normals[stoi(faceLine) -1];
						}
						
						index_counter++;
					}

					//Add to list of vertexes
					vertexes.push_back(Vertex(temp_pos, temp_tex, temp_norm));
				}

				//Generate indices based on number of vertices that make up a face
				if (token.size() - 1 == 4) {
					indices.push_back(2 + indicesOffSet);
					indices.push_back(1 + indicesOffSet);
					indices.push_back(0 + indicesOffSet);
					indices.push_back(3 + indicesOffSet);
					indices.push_back(2 + indicesOffSet);
					indices.push_back(0 + indicesOffSet);
					indicesOffSet = indicesOffSet + 4;
					//cout << "IndicesOffset: " << indicesOffSet << endl;
				}
				else if (token.size() - 1 == 3) {
					indices.push_back(2 + indicesOffSet);
					indices.push_back(1 + indicesOffSet);
					indices.push_back(0 + indicesOffSet);
					indicesOffSet = indicesOffSet + 3;
				}
			}
		}
		return true;
	}
	else {
		cout << "Cannot open File" << endl;
		return false;
	}
}

void FileReader::LoadMaterials() {
	string curerntMtl = "";

	string line = "";
	ifstream myFile("./models/low_poly_boat/low_poly_boat.mtl" );
	
	if (myFile.is_open()) {

		while (getline(myFile, line)) {
			vector<string> token;
			istringstream iss(line);

			for (string s; iss >> s; ) {
				token.push_back(s);
			}

			if (token.size() > 0) {

				if (token[0] == "newmtl") {
					curerntMtl = token[1];

					Material newMaterial = Material();
					materialsMap[curerntMtl] = newMaterial;
					materialsMap[curerntMtl].SetName(token[1]);
				}

				if (token[0] == "Ns") {
					materialsMap[curerntMtl].SetSpecularWeight(stof(token[1]));
					cout << "NS token " << token[1] << endl;
				}

				if (token[0] == "Ka") {
					materialsMap[curerntMtl].SetAmbientCol(vec3(stof(token[1]), stof(token[2]), stof(token[3])));
				}

				if (token[0] == "Kd") {
					materialsMap[curerntMtl].setDiffuseCol(vec3(stof(token[1]), stof(token[2]), stof(token[3])));
				}

				if (token[0] == "Ka") {
					materialsMap[curerntMtl].SetSpecularCol(vec3(stof(token[1]), stof(token[2]), stof(token[3])));
				}

				if (token[0] == "d") {
					materialsMap[curerntMtl].SetDifuse(stof(token[1]));
				}

				if (token[0] == "map_Kd") {
					materialsMap[curerntMtl].SetMapKD(token[1]);
				}

				if (token[0] == "map_d") {
					materialsMap[curerntMtl].SetMapD(token[1]);
				}
			}
		}
	}
	else
	{
		cout << "unable to read material file" << endl;
	}

	cout << "Matmap size: " << materialsMap.size() << endl;

}