#pragma once
#include "furniture.h"
class Cabinet :
	public Furniture
{
public:
	float height;
	Cabinet(void);
	~Cabinet(void);
	void Cabinet::drawFurniture(glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, float color[]);
	float giveHeight();
	void Cabinet::intersect(glm::vec3 eye, glm::vec3 direction, glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, double &t, glm::vec4 &normal);
};

