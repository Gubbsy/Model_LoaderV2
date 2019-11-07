#include "Mesh.h"
#include "stb_image.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

Mesh::Mesh(GLuint* shaderProgram) {

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	shader = *shaderProgram;
	// Collection of points to build cube
	vertices = {
		0.5f,  0.5f, -0.5f,  //0 top right
		0.5f, -0.5f, -0.5f,  //1 bottom right
		-0.5f, -0.5f, -0.5f, //2 bottom left
		-0.5f,  0.5f, -0.5f,  //3 top left

		0.5f,  0.5f, 0.5f,  //4 top right
		0.5f, -0.5f, 0.5f,  //5 bottom right
		-0.5f, -0.5f, 0.5f, //6 bottom left
		-0.5f,  0.5f, 0.5f  //7 top left 
	};

	// array of indexes refering to vertec points (defined above) 
	indices = {  // note that we start from 0!
		0, 3, 1,  // first Triangle front
		3, 2, 1,   // second Triangle

		4, 7, 0 ,
		7, 3, 0 ,

		1, 2, 5 ,
		2, 6, 5 ,

		5, 4, 0 ,
		0, 1, 5 ,

		2, 3, 7 ,
		7, 6, 2 ,

		4, 5, 7 ,  // first Triangle back
		7, 5, 6    // second Triangle
	};

	// Array of colour values for each corner point (length 4 for RGBA)
	colours = {
		 1.0f, 0.0f, 0.0f, 1.0f , //Corner 1
		 0.0f, 1.0f, 0.0f, 1.0f ,
		0.0f, 0.0f, 1.0f, 1.0f ,
		 1.0f, 1.0f, 0.0f, 1.0f,
		 1.0f, 0.0f, 0.0f, 1.0f ,
		 0.0f, 1.0f, 0.0f, 1.0f ,
		 0.0f, 0.0f, 1.0f, 1.0f ,
		 1.0f, 1.0f, 0.0f, 1.0f
	};

	// Texture scale to the cube
	textures = {
		 1.0f, 1.0f, //Corener 1
		 1.0f, 0.0f,
		 0.0f, 0.0f,
		 0.0f, 1.0f,

		 0.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f,

	};

	BindVertices(); 
	BindIndices();
	BindColours();
	BindTextures();
	ApplyTexture();
}

void Mesh::BindVertices() {

	// Generate for Buffer arrays 
	glGenBuffers(1, &verticesVB0);

	// Bining All the vertixes that triangles can be made from (VBO)
	glBindBuffer(GL_ARRAY_BUFFER, verticesVB0);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
}

void Mesh::BindIndices() {
	glGenBuffers(1, &indicesEBO);

	// Binding Contains the combination that from triangles (using the vertexes) [EBO] (for re-using points bassicaly)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), &indices[0], GL_STATIC_DRAW);

	// matches to the location on the vertex shader to render postion ASK SWEN!!!!!
	// This method will point the currently bound buffer to the specified shader locatiion
	glVertexAttribPointer(vPosition, 3, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));
}

void Mesh::BindColours() {
	glGenBuffers(1, &coloursVBO);

	//Colour Binding 
	glBindBuffer(GL_ARRAY_BUFFER, coloursVBO);
	glBufferStorage(GL_ARRAY_BUFFER, colours.size() * sizeof(GLfloat), &colours[0], 0);

	// Passes colour intp shader
	glVertexAttribPointer(cPosition, 4, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));
}

void Mesh::BindTextures() {

	glGenBuffers(1, &texturesVB0);

	//Texture Binding
	glBindBuffer(GL_ARRAY_BUFFER, texturesVB0);
	glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(GLfloat), &textures[0], GL_STATIC_DRAW);

	//Pass texture to shader
	glVertexAttribPointer(tPosition, 2, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));
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
	unsigned char* data = stbi_load("media/textures/awesomeface.png", &width, &height, &nrChannels, 0);
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