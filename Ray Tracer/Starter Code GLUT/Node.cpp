#include "Node.h"


Node::Node(void)
{
}

Node::Node(std::string fur, Furniture *furnitureType, int xIndex, int zIndex, float r, float g, float b, float rotation, float xScale, float yScale, float zScale)
{
	this->fur = fur;
	parent = 0;
	this->furnitureType = furnitureType;
	this->xIndex = xIndex;
	this->zIndex = zIndex;
	this->r = r;
	this->g = g;
	this->b = b;
	this->rotation = rotation;
	this->xScale = xScale;
	this->yScale = yScale;
	this->zScale = zScale;
}


//RAY TRACER
Node::Node(std::string fur, Furniture *furnitureType, int xIndex, int zIndex,
	float rotation, float xScale, float yScale, float zScale, 
	glm::vec3 dcol, float specular, float reflectivity, float transparency, float refraction, float eight)
{
	//FOR BACKWARD COMPATABILITY
	this->r = dcol[0];
	this->g = dcol[1];
	this->b = dcol[2];
	//FOR BACKWARD COMPATABILITY
	this->fur = fur;
	parent = 0;
	this->furnitureType = furnitureType;
	this->xIndex = xIndex;
	this->zIndex = zIndex;
	this->rotation = rotation;
	this->xScale = xScale;
	this->yScale = yScale;
	this->zScale = zScale;
	this->dcol = dcol;
	this->specular = specular;
	this->reflectivity = reflectivity;
	this->transparency = transparency;
	this->refraction = refraction;
	this->eight = eight;
}
//RAY TRACER

Node::~Node(void)
{
}
