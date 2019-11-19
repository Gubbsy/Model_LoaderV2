#include "Mesh.h"
#include "stb_image.h"
#include "ObjReader.h"
#include <filesystem>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

using namespace std;

void Mesh::Init(std::vector<Vertex>& _vertexes, std::vector<GLuint>& _indices, Material& _material, string& _folderTree)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	vertexes = _vertexes;
	indices = _indices;
	material = _material;
	folderTree = _folderTree;

	BindVertices();
	BindIndices();
	PassToShader();
	ApplyTexture();
}

void Mesh::PassToShader()
{
	// matches to the location on the vertex shader 
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);

	glVertexAttribPointer(tPosition, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offsetof(Vertex, texture)));
	glEnableVertexAttribArray(tPosition);

	glVertexAttribPointer(cPosition, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offsetof(Vertex, colour)));
	glEnableVertexAttribArray(cPosition);

	glVertexAttribPointer(nPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(nPosition);
}

void Mesh::BindVertices() {
	// Bining All the vertixes that triangles can be made from (VBO)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(Vertex), &vertexes[0], GL_STATIC_DRAW);
}

void Mesh::BindIndices() {
	glGenBuffers(1, &indicesEBO);
	// Binding Contains the combination that from triangles (using the vertexes) [EBO] (for re-using points bassicaly)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}


void Mesh::ApplyTexture() {
	//Generate Texture
	glGenTextures(1, &texture1);
	//Bind generated textures as type
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	//Create 'Space'
	GLint width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis (it's loaded upside down).
	
	texturePath = folderTree + material.GetMapD();
	
	//Creates texture data from resource
	if (!exists(texturePath)) {
		texturePath = "./media/textures/DefaultWhite.png";
	}

	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		//Creates texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//Generates minimap for texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Query - In the shader program, get me the Uniform location and set it to 0, then feed it into shader
	glUniform1i(glGetUniformLocation(shader, "texture1"), 0);

}

void Mesh::Draw(GLuint& shaderProgram) {
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::Delete()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &indicesEBO);
	glDeleteVertexArrays(1, &VAO);
}

bool Mesh::exists(const std::string& name)
{
	ifstream f(name.c_str());
	return f.good();
}



