#pragma once
#include "furniture.h"
class Floor :
	public Furniture
{
public:
	int xSize, zSize;
	Floor(void);
	~Floor(void);
	void Floor::drawFurniture(glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, float color[]);
	void Floor::drawFloor(int, int);
	float giveHeight();
	void Floor::intersect(glm::vec3 eye, glm::vec3 direction, glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, double &t, glm::vec4 &normal);
};

