#include "FileReader.h"

Model FileReader::ReadFile(string _file) {
	Material currentMaterial;
	file = _file;
	string line = "";
	ifstream myFile(file);
	int indicesOffSet = 0;

	ResetReader();

	Model* model = new Model();
	Object* tempObject = nullptr;
	Mesh* tempMesh = nullptr;
	
	ConstructFolderTree();

	if (myFile.is_open()) {

		cout << "Opened File" << endl;
		while (getline(myFile, line)) {

			vector<string> token;
			SplitOnSpace(token, line);

			//Load in material file and asign it to model
			if (token[0] == "mtllib") {
				mtLib = token[1];
				LoadMaterials();
			}

			// Find new object in file, if current object in memeory create new object, create new object.
			if (token[0] == "o") {
				if (tempObject != nullptr) {
					model->AddObject(*tempObject);
				}
				tempObject = new Object();
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

			if (token[0] == "usemtl") {
				string materialKey = token[1];
				currentMaterial = materialsMap[materialKey];
				if (tempMesh != nullptr) {
					tempMesh->Init(vertexes, indices, currentMaterial, relFolderTree);
					tempObject->AddMesh(*tempMesh);
					vertexes.clear();
					vertexes.clear();
				}
				tempMesh = new Mesh();
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
					vec4 temp_col = vec4(currentMaterial.GetDiffuseCol(), 0.1f);

					//for every face
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
					vertexes.push_back(Vertex(temp_pos, temp_tex, temp_norm, temp_col));
				}

				//Generate indices based on number of vertices that make up a face
				if (token.size() - 1 == 4) {
					indices.push_back(0 + indicesOffSet);
					indices.push_back(1 + indicesOffSet);
					indices.push_back(2 + indicesOffSet);
					indices.push_back(0 + indicesOffSet);
					indices.push_back(2 + indicesOffSet);
					indices.push_back(3 + indicesOffSet);
					indicesOffSet = indicesOffSet + 4;
					//cout << "IndicesOffset: " << indicesOffSet << endl;
				}
				else if (token.size() - 1 == 3) {
					indices.push_back(0 + indicesOffSet);
					indices.push_back(1 + indicesOffSet);
					indices.push_back(2 + indicesOffSet);
					indicesOffSet = indicesOffSet + 3;
				}
			}
		}
		tempMesh->Init(vertexes, indices, currentMaterial, relFolderTree);
		tempObject->AddMesh(*tempMesh);
		model->AddObject(*tempObject);
		
		delete(tempMesh);
		delete(tempObject);
		return *model;
	}
	else {
		cout << "Cannot open File" << endl;
	}
}

void FileReader::ResetReader()
{
	vertexes.clear();
	indices.clear();
	vertices.clear();
	textures.clear();
	normals.clear();
	materialsMap.clear();

	mtLib = "";
	relFolderTree = "";
}

void FileReader::ConstructFolderTree()
{
	relFolderTree = ".";
	std::stringstream ss(file);
	std::string folder;
	std::vector<std::string> components;

	while (std::getline(ss, folder, '/'))
	{
		components.push_back(folder);
	}
	
	for (int i = 0; i < components.size() -1; i++) {
		relFolderTree = relFolderTree + "/" + components[i];
	}

	relFolderTree += "/";
}

void FileReader::LoadMaterials() {
	string curerntMtl = "";

	string line = "";
	ifstream myFile(relFolderTree + "/" + mtLib );
	
	if (myFile.is_open()) {
		while (getline(myFile, line)) {
			vector<string> token;
			SplitOnSpace(token, line);

			if (token.size() > 0) {

				if (token[0] == "newmtl") {
					curerntMtl = token[1];

					Material newMaterial = Material();
					materialsMap[curerntMtl] = newMaterial;
					materialsMap[curerntMtl].SetName(token[1]);
				}

				if (token[0] == "Ns") {
					materialsMap[curerntMtl].SetSpecularWeight(stof(token[1]));
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
}

void FileReader::SplitOnSpace(vector<string>& token, string& stringToSplit)
{
	istringstream iss(stringToSplit);

	for (string s; iss >> s; ) {
		token.push_back(s);
	}
}



