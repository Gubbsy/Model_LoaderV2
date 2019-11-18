#include "DaeReader.h"
#include "ObjReader.h"
#include <regex>
#include "Input.h"

using namespace std;

Model* DaeReader::ReadFile(string _file)
{	
	Model* model = new Model();
	Object* tempObject = nullptr;
	Mesh* tempMesh = nullptr;

	std::string file = _file;
	string line = "";
	ifstream myFile(file);

	if (myFile.is_open()) {
		while (getline(myFile, line)) {
			fileString += "\n" + line;
		}
	}

	string fileStringCpy = fileString;

	// Matches sources and adds to map<sourceID, sourceValue>
	regex sourceReg("<source.+id=\"([\\s\\S]+?)\"[\\s\\S]*?>[\\s\\S]*?<float_array[\\s\\S]*?>([\\s\\S]+?)<\/float_array>");
	smatch matchResult;

	while (regex_search(fileStringCpy, matchResult, sourceReg)) {

		vector<GLfloat> values;

		//cout << "Match Results 1: " << matchResult[1] << endl;
		//cout << "Match Results 2: " << matchResult[2] << endl;

		string id = matchResult[1];
		string valuesString = matchResult[2];

		istringstream iss(valuesString);
		for (string s; iss >> s; ) {
			values.push_back(stof(s));
		}

		sources[id] = values;

		fileStringCpy = matchResult.suffix();
	}

	fileStringCpy = fileString;

	regex triInputReg("<input (?=.*semantic=\"([\\s\\S]+?)\")[\\s\\S]*?(?=.*source=\"#?([\\s\\S]*?)\")[\\s\\S]*?(?=.*offset=\"([\\s\\S]*?)\")");

	while (regex_search(fileStringCpy, matchResult, triInputReg)) {
		
		//cout << "Match Results 1: " << matchResult[1] << endl;
		//cout << "Match Results 2: " << matchResult[2] << endl;
		//cout << "Match Results 3: " << matchResult[3] << endl;

		string semantic = matchResult[1];
		string sourceID = matchResult[2];
		GLint offSet = stoi(matchResult[3]);
		
		if(semantic != "VERTEX")
			inputs.push_back(Input(semantic, sourceID, offSet, sources[sourceID]));

		fileStringCpy = matchResult.suffix();
	}

	fileStringCpy = fileString;

	regex triIndicesReg("<triangles material=[\\s\\S]*?<p>([\\s\\S]+?)<\/p>");

	while (regex_search(fileStringCpy, matchResult, triIndicesReg)) {

		string triIndices = matchResult[1];

		istringstream iss(triIndices);
		for (string s; iss >> s; ) {
			vertexDefs.push_back(stoi(s));
		}

		fileStringCpy = matchResult.suffix();
	}

	//Step through indices with stide of input lenght
	for (int i = 0; i < vertexDefs.size(); i = i + inputs.size()) {
		//Set-up temp data stores to create vertex
		glm::vec3 vert = glm::vec3();
		glm::vec4 col = glm::vec4();
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

	for (int i = 0; i < vertexDefs.size(); i++) {
		indices.push_back(i);
	}
	
 	return model;
}
