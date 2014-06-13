#pragma once

#include "Furniture.h"
#include <vector>

class Node
{
public:
	std::string fur;
	Furniture *furnitureType;
	int xIndex, zIndex;
	float r, g, b, rotation, xScale, yScale, zScale;
	std::vector <Node *> children;
	Node *parent;
	Node(void);
	Node(std::string fur, Furniture*, int, int, float, float, float, float, float, float, float);
	Node(std::string fur, Furniture*, int, int, float, float, float, float,
		glm::vec3, float, float, float, float, float);
	~Node(void);

	//RAY TRACER
	glm::vec3 dcol;
	float specular, reflectivity, transparency, refraction, eight;
	//RAY TRACER
};