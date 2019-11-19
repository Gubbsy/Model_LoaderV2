#include "DaeReader.h"
#include "ObjReader.h"
#include <regex>
#include "Input.h"

using namespace std;

Model* DaeReader::ReadFile(string _file)
{	
	ResetReader();

	Model* model = new Model();
	Object* tempObject = new Object;
	Mesh* tempMesh = new Mesh();
	newMaterial = Material();

	file = _file;
	texFile = "";
	string line = "";
	ifstream myFile(file);

	if (myFile.is_open()) {
		while (getline(myFile, line)) {
			fileString += "\n" + line;
		}
	}

	PullSource(fileString);
	PullInputs(fileString);
	PullVertextDef(fileString);
	PullTexture(fileString);
	ConstructFolderTree();
	GenerateVertices();
	GenerateIndices();
	
	newMaterial.SetMapD(texFile);
	tempMesh->Init(vertices, indices, newMaterial, relFolderTree);
	tempObject->AddMesh(*tempMesh);
	model->AddObject(*tempObject);
	
 	return model;
}

void DaeReader::ResetReader()
{
	sources = map<std::string, std::vector<GLfloat>>();
	fileString = "";
	vertexDefs = vector<GLuint>();
	inputs = vector<Input>();
	vertices = vector<Vertex>();
	indices = vector<GLuint>();
}

void DaeReader::PullSource(string _fileString)
{
	// Matches sources and adds to map<sourceID, sourceValue>
	regex sourceReg("<source.+id=\"([\\s\\S]+?)\"[\\s\\S]*?>[\\s\\S]*?<float_array[\\s\\S]*?>([\\s\\S]+?)<\/float_array>");
	smatch matchResult;

	while (regex_search(_fileString, matchResult, sourceReg)) {

		vector<GLfloat> values;

		string id = matchResult[1];
		string valuesString = matchResult[2];

		istringstream iss(valuesString);
		for (string s; iss >> s; ) {
			values.push_back(stof(s));
		}

		sources[id] = values;

		_fileString = matchResult.suffix();
	}
}

void DaeReader::PullInputs(string _fileString)
{
	regex triInputReg("<input (?=.*semantic=\"([\\s\\S]+?)\")[\\s\\S]*?(?=.*source=\"#?([\\s\\S]*?)\")[\\s\\S]*?(?=.*offset=\"([\\s\\S]*?)\")");
	smatch matchResult;

	while (regex_search(_fileString, matchResult, triInputReg)) {

		string semantic = matchResult[1];
		string sourceID = matchResult[2];
		GLint offSet = stoi(matchResult[3]);

		if (semantic != "VERTEX")
			inputs.push_back(Input(semantic, sourceID, offSet, sources[sourceID]));

		_fileString = matchResult.suffix();
	}
}

void DaeReader::PullVertextDef(string _fileString)
{
	regex triIndicesReg("<triangles [\\s\\S]*?<p>([\\s\\S]+?)<\/p>");
	smatch matchResult;

	while (regex_search(_fileString, matchResult, triIndicesReg)) {

		string triIndices = matchResult[1];

		istringstream iss(triIndices);
		for (string s; iss >> s; ) {
			vertexDefs.push_back(stoi(s));
		}

		_fileString = matchResult.suffix();
	}
}

void DaeReader::PullTexture(string _fileString)
{
	regex triIndicesReg("<image id=[\\s\\S]*?<init_from>([\\s\\S]+?)<\/init_from>");
	smatch matchResult;

	while (regex_search(_fileString, matchResult, triIndicesReg)) {

		texFile = matchResult[1];

		_fileString = matchResult.suffix();
	}
}


void DaeReader::GenerateVertices()
{
	//Step through indices with stide of input lenght
	for (int i = 0; i < vertexDefs.size(); i = i + inputs.size()) {
		//Set-up temp data stores to create vertex
		glm::vec3 vert = glm::vec3();
		glm::vec4 col = glm::vec4(1.0f);
		glm::vec2 tex = glm::vec2();
		glm::vec3 norm = glm::vec3();

		// for the number of inputs that build a vertex
		for (size_t j = 0; j < inputs.size(); j++)
		{
			//set current index
			int curIndex = vertexDefs[i + j];

			//if POSITION input, add to temp vertex store
			if (inputs[j].semantic == "POSITION") {
				vert.x = inputs[j].data[curIndex * 3];
				vert.y = inputs[j].data[curIndex * 3 + 1];
				vert.z = inputs[j].data[curIndex * 3 + 2];
			}

			//if NORMAL input, add normals
			if (inputs[j].semantic == "NORMAL") {
				norm.x = inputs[j].data[curIndex * 3];
				norm.y = inputs[j].data[curIndex * 3 + 1]; //index offset
				norm.z = inputs[j].data[curIndex * 3 + 2];
			}

			//if TEXTCOORD input, add normals
			if (inputs[j].semantic == "TEXCOORD") {
				tex.s = inputs[j].data[curIndex * 2];
				tex.t = inputs[j].data[curIndex * 2 + 1];
			}

			//if COLOR input, add normals
			if (inputs[j].semantic == "COLOR") {
				col.r = inputs[j].data[curIndex * 4];
				col.g = inputs[j].data[curIndex * 4 + 1];
				col.b = inputs[j].data[curIndex * 4 + 2];
				col.a = inputs[j].data[curIndex * 4 + 3];
			}
		}

		vertices.push_back(Vertex(vert, tex, norm, col));
	}
}

void DaeReader::GenerateIndices()
{
	for (int i = 0; i < vertexDefs.size(); i++) {
		indices.push_back(i);
	}
}

void DaeReader::ConstructFolderTree()
{
	relFolderTree = ".";
	std::stringstream ss(file);
	std::string folder;
	std::vector<std::string> components;

	while (std::getline(ss, folder, '/'))
	{
		components.push_back(folder);
	}

	for (int i = 0; i < components.size() - 1; i++) {
		relFolderTree = relFolderTree + "/" + components[i];
	}

	relFolderTree += "/";
}

