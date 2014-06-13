#pragma once
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "RayIntersection.h"


//#include <iostream>
//#include <vector>
//
////RAY TRACER
//
//class intersectionPoint
//{
//public:
//	double t;
//	glm::vec3 normal;
//};
////RAY TRACER

class Furniture
{
	//RAY TRACER
protected:
	RayIntersection *rayInt;
	//std::vector <intersectionPoint*> intersectionPoints;   
	//RAY TRACER
public:
	virtual void drawFurniture(glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, float color[])
	{}
	virtual void intersect(glm::vec3 eye, glm::vec3 direction, glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, double &t, glm::vec4 &normal)
	{}
	virtual float giveHeight()
	{
		return 0;
	}
	Furniture(void);
	~Furniture(void);
};

