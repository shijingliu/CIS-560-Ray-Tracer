#pragma once
#include "furniture.h"
class Chair :
	public Furniture
{
public:
	Chair(void);
	~Chair(void);
	void Chair::drawFurniture(glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, float color[]);
	float giveHeight();
	void Chair::intersect(glm::vec3 eye, glm::vec3 direction, glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, double &t, glm::vec4 &normal);
};

