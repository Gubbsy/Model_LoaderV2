#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>
#include <glm\ext\vector_float4.hpp>
#include <GL\glew.h>

struct Vertex
{
	glm::vec3 postion;
	glm::vec2 texture;
	glm::vec3 normal;
	glm::vec4 colour;

	Vertex(glm::vec3 _position, glm::vec2 _texture, glm::vec3 _noraml, glm::vec4 _colour) {
		this->postion = _position;
		this->texture = _texture;
		this->normal = _noraml;
		this->colour = _colour;
	}
};