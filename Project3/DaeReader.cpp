#include "DaeReader.h"
#include "ObjReader.h"
#include <regex>
#include "Input.h"

using namespace std;

Model* DaeReader::ReadFile(string _file)
{
	//Reset reader from previouse use
	ResetReader();

	//Initialise variables
	Model* model = new Model();
	Object* tempObject = new Object;
	Mesh* tempMesh = new Mesh();
	newMaterial = Material();

	try {
		file = _file;
		texFile = "";
		string line = "";
		ifstream myFile(file);
		
		//Open file
		if (myFile.is_open()) {
	
			cout << file << " Opened File" << endl;
			//Extract each line and add it to fileString
			while (getline(myFile, line)) {
				fileString += "\n" + line;
			}
			//Extract data from fileString
			PullSource(fileString);
			PullInputs(fileString);
			PullVertextDef(fileString);
			PullTexture(fileString);

			ConstructFolderTree();

			//Generate Vertices and Indices
			GenerateVertices();
			GenerateIndices();

			//Add texture file to material
			newMaterial.SetMapD(texFile);

			//Construt model
			tempMesh->Init(vertices, indices, newMaterial, relFolderTree);
			tempObject->AddMesh(*tempMesh);
			model->AddObject(*tempObject);
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

void DaeReader::ResetReader()
{
	sources = map<std::string, std::vector<GLfloat>>();
	fileString = "";
	vertexDefs = vector<GLuint>();
	inputs = vector<Input>();
	vertices = vector<Vertex>();
	indices = vector<GLuint>();
}
// Matches sources and adds to map<sourceID, sourceValue>
void DaeReader::PullSource(string _fileString)
{
	
	regex sourceReg("<source.+id=\"([\\s\\S]+?)\"[\\s\\S]*?>[\\s\\S]*?<float_array[\\s\\S]*?>([\\s\\S]+?)<\/float_array>");
	smatch matchResult;

	//While the regex has matches
	while (regex_search(_fileString, matchResult, sourceReg)) {

		//temp Store pulled source values (vertices data)
		vector<GLfloat> values;

		// Pull source ID and value string from file
		string id = matchResult[1];
		string valuesString = matchResult[2];

		//Split source values on space, cast them to float and add them to store
		istringstream iss(valuesString);
		for (string s; iss >> s; ) {
			values.push_back(stof(s));
		}

		//Add source to source map
		sources[id] = values;

		//Pass suffix (post match becomes next string to match)
		_fileString = matchResult.suffix();
	}
}

void DaeReader::PullInputs(string _fileString)
{
	//Regex match inputs
	regex triInputReg("<input (?=.*semantic=\"([\\s\\S]+?)\")[\\s\\S]*?(?=.*source=\"#?([\\s\\S]*?)\")[\\s\\S]*?(?=.*offset=\"([\\s\\S]*?)\")");
	smatch matchResult;

	while (regex_search(_fileString, matchResult, triInputReg)) {

		//Create temp stores for input data
		string semantic = matchResult[1];
		string sourceID = matchResult[2];
		GLint offSet = stoi(matchResult[3]);

		//Inputs are added. Avoids navigating through file to pull postion. Positions are pulled in directly and VERTEX semantic is emmited. 
		if (semantic != "VERTEX")
			inputs.push_back(Input(semantic, sourceID, offSet, sources[sourceID]));

		_fileString = matchResult.suffix();
	}
}

//Pulls vertex definitions (triangles indices) that create vertex.
void DaeReader::PullVertextDef(string _fileString)
{
	regex triIndicesReg("<triangles [\\s\\S]*?<p>([\\s\\S]+?)<\/p>");
	smatch matchResult;

	while (regex_search(_fileString, matchResult, triIndicesReg)) {

		// Pull indices values and assign thm
		string triIndices = matchResult[1];

		//Split data on space and add to vector
		istringstream iss(triIndices);
		for (string s; iss >> s; ) {
			vertexDefs.push_back(stoi(s));
		}

		_fileString = matchResult.suffix();
	}
}

//Pull texture File
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

			// Multiply by three as the data is incremented in 3 for each piece of vetex data i.e. triNormal, triPositin, triColour

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
	//Generte indices, counts up as data is in triangle order
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

