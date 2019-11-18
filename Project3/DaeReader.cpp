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

	regex triInputReg("<input semantic=\"([\\s\\S]+?)\"[\\s\\S]*?source=\"#?([\\s\\S]*?)\"[\\s\\S]*?offset=\"([\\s\\S]*?)\"");

	while (regex_search(fileStringCpy, matchResult, triInputReg)) {
		
		//cout << "Match Results 1: " << matchResult[1] << endl;
		//cout << "Match Results 2: " << matchResult[2] << endl;
		//cout << "Match Results 3: " << matchResult[3] << endl;

		string semantic = matchResult[1];
		string sourceID = matchResult[2];
		GLint offSet = stoi(matchResult[3]);
		
		inputs.push_back(Input(semantic, sourceID, offSet, sources[sourceID]));

		fileStringCpy = matchResult.suffix();
	}

	fileStringCpy = fileString;

	regex triIndicesReg("<triangles material=[\\s\\S]*?<p>([\\s\\S]+?)<\/p>");

	while (regex_search(fileStringCpy, matchResult, triIndicesReg)) {

		cout << "Match Results 1: " << matchResult[1] << endl;
		string triIndices = matchResult[1];

		istringstream iss(triIndices);
		for (string s; iss >> s; ) {
			vertexDefs.push_back(stof(s));
		}

		fileStringCpy = matchResult.suffix();
	}

 	return model;
}
