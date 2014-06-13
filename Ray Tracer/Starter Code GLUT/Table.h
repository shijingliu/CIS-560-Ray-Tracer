#pragma once
#include "furniture.h"
class Table :
	public Furniture
{
public:
	Table(void);
	~Table(void);
	void Table::drawFurniture(glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, float color[]);
	float giveHeight();
	void Table::intersect(glm::vec3 eye, glm::vec3 direction, glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, double &t, glm::vec4 &normal);
};