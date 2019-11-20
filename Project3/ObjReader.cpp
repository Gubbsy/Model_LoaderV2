#include "ObjReader.h"

Model* ObjReader::ReadFile(string _file) {
	
	//Intialise objects to form model
	Model* model = new Model();
	Object* tempObject = nullptr;
	Mesh* tempMesh = nullptr;
	
	try {
		//Intilas varables for file reading
		Material currentMaterial;
		file = _file;
		string line = "";
		ifstream myFile(file);
		int indicesOffSet = 0;

		// Reset reader to remove hangover from previouse reading
		ResetReader();

		//Construct folder tree from path to grab textue
		ConstructFolderTree();

		//Open file
		if (myFile.is_open()) {

			cout << file <<" Opened File" << endl;
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

				// Once new material identifier encountered, add all current vertex data to mesh and add mesh to object
				// clear temp stores
				if (token[0] == "usemtl") {
					string materialKey = token[1];
					currentMaterial = materialsMap[materialKey];
					if (tempMesh != nullptr) {
						tempMesh->Init(vertexes, indices, currentMaterial, relFolderTree);
						tempObject->AddMesh(*tempMesh);
						vertexes.clear();
						indices.clear();
						indicesOffSet = 0;
					}
					//Instantiate new temp Mesh to be new mesh
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
						// Set temp store for vertex data
						vec3 temp_pos = vec3(0.0f);
						vec2 temp_tex = vec2(0.0f);
						vec3 temp_norm = vec3(0.0f);
						vec4 temp_col = vec4(currentMaterial.GetDiffuseCol(), 0.1f);

						//for every face
						while (getline(faceIss, faceLine, '/')) {

							//if index 0 fetch vertices
							if (index_counter == 0) {
								temp_pos = vertices[stoi(faceLine) - 1];
							}
							// if index 1 fetch from texture
							else if (index_counter == 1) {
								temp_tex = textures[stoi(faceLine) - 1];
							}
							// if index 2 fetch from normal
							else if (index_counter == 2) {
								temp_norm = normals[stoi(faceLine) - 1];
							}

							//Incremet index counter
							index_counter++;
						}

						//Add to list of vertexes
						vertexes.push_back(Vertex(temp_pos, temp_tex, temp_norm, temp_col));
					}

					//Generate indices based on number of vertices that make up a face (in order to construct triangles from quad)
					if (token.size() - 1 == 4) {
						indices.push_back(0 + indicesOffSet);
						indices.push_back(1 + indicesOffSet);
						indices.push_back(2 + indicesOffSet);
						indices.push_back(0 + indicesOffSet);
						indices.push_back(2 + indicesOffSet);
						indices.push_back(3 + indicesOffSet);
						indicesOffSet = indicesOffSet + 4;
					}
					//Generate indices for triangle
					else if (token.size() - 1 == 3) {
						indices.push_back(0 + indicesOffSet);
						indices.push_back(1 + indicesOffSet);
						indices.push_back(2 + indicesOffSet);
						indicesOffSet = indicesOffSet + 3;
					}
				}
			}
			//Add all remaining mesh data to object
			tempMesh->Init(vertexes, indices, currentMaterial, relFolderTree);
			tempObject->AddMesh(*tempMesh);
			//construct model from objects
			model->AddObject(*tempObject);

			// clean up.
			delete(tempMesh);
			delete(tempObject);
		}
		else 
		{
			cout << "Unable to open obj file: " << file << endl;
			model = nullptr;
		}
	}
	catch (...) {
		cerr << "Unable to read obj file: " << file << ". The file you entered may be corrupted" << endl;
	}

	return model;
}

void ObjReader::ResetReader()
{
	//clean up data from previouse reading
	vertexes.clear();
	indices.clear();
	vertices.clear();
	textures.clear();
	normals.clear();
	materialsMap.clear();

	mtLib = "";
	relFolderTree = "";
}

void ObjReader::ConstructFolderTree()
{
	// Construct folder tree
	relFolderTree = ".";
	std::stringstream ss(file);
	std::string folder;
	std::vector<std::string> components;

	//Split on /
	while (std::getline(ss, folder, '/'))
	{
		components.push_back(folder);
	}
	
	//Construct folder tree, removing file
	for (int i = 0; i < components.size() -1; i++) {
		relFolderTree = relFolderTree + "/" + components[i];
	}

	relFolderTree += "/";
}

void ObjReader::LoadMaterials() {
	string curerntMtl = "";
	string line = "";
	ifstream myFile(relFolderTree + "/" + mtLib );
	
	//Open material File
	if (myFile.is_open()) {
		while (getline(myFile, line)) {
			vector<string> token;
			SplitOnSpace(token, line);

			// Material file has content
			if (token.size() > 0) {

				// Cretae material upon reaching new material identifier and add it to material map
				if (token[0] == "newmtl") {
					curerntMtl = token[1];

					Material newMaterial = Material();
					materialsMap[curerntMtl] = newMaterial;
					materialsMap[curerntMtl].SetName(token[1]);
				}
				// Pull relevant data using identifiers
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

// Split on space
void ObjReader::SplitOnSpace(vector<string>& token, string& stringToSplit)
{
	istringstream iss(stringToSplit);

	for (string s; iss >> s; ) {
		token.push_back(s);
	}
}



