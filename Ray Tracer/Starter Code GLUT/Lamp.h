#pragma once
#include "furniture.h"
class Lamp :
	public Furniture
{
public:
	Lamp(void);
	~Lamp(void);
	void Lamp::drawFurniture(glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, float color[]);
	float giveHeight();
	void Lamp::intersect(glm::vec3 eye, glm::vec3 direction, glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, double &t, glm::vec4 &normal);
};

