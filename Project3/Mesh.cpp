#include "Mesh.h"
#include "stb_image.h"
#include "FileReader.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

Mesh::Mesh(GLuint* shaderProgram) {

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	FileReader fileReader = FileReader();
	fileReader.ReadFile("./models/low_poly_boat/low_poly_boat.obj", vertexes, indices);

	shader = *shaderProgram;
	
	BindVertices(); 
	BindIndices();

	ApplyTexture();
}

void Mesh::BindVertices() {

	// Generate for Buffer arrays 
	glGenBuffers(1, &VBO);

	// Bining All the vertixes that triangles can be made from (VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(Vertex), &vertexes[0], GL_STATIC_DRAW);
}

void Mesh::BindIndices() {
	glGenBuffers(1, &indicesEBO);

	// Binding Contains the combination that from triangles (using the vertexes) [EBO] (for re-using points bassicaly)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	// matches to the location on the vertex shader to render postion ASK SWEN!!!!!
	// This method will point the currently bound buffer to the specified shader locatiion
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);

	//glVertexAttribPointer(nPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offsetof(Vertex, normal)));
	//glEnableVertexAttribArray(nPosition);
	
	glVertexAttribPointer(tPosition, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offsetof(Vertex, texture)));
	glEnableVertexAttribArray(tPosition);
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
	//Creates texture data from resource
	unsigned char* data = stbi_load("models/Creeper-obj/Texture.png", &width, &height, &nrChannels, 0);
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

void Mesh::Draw() {
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}