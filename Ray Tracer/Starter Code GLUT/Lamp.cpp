#include "Lamp.h"


Lamp::Lamp(void)
{
}


Lamp::~Lamp(void)
{
}

float Lamp::giveHeight()
{
	//return 5.375f;
	//return 1.375f;
	return 3.0f;
}

void Lamp::intersect(glm::vec3 eye, glm::vec3 direction, glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, double &t, glm::vec4 &normal)
{
	glm::vec3 zz;
	zz[0] = xIndex;
	zz[1] = yIndex;
	zz[2] = zIndex;
	modelView = glm::translate(modelView, zz);
	modelView = glm::rotate(modelView, rotation, glm::vec3(0, 1, 0));
	zz[0] = xScale;
	zz[1] = yScale;
	zz[2] = zScale;
	modelView = glm::scale(modelView, zz);

	//BOUNDING SPHERE
	float bind[3] = {0, 0, 1};
	zz[0] = 0;
	zz[1] = 1.5;
	zz[2] = 0;
	glm::mat4 bSphere = glm::translate(modelView, zz);
	zz[0] = 2;
	zz[1] = 2;
	zz[2] = 2;
	bSphere = glm::scale(bSphere, zz);
	//drawSphere(bSphere, bind);
	glm::vec4 tempo;
	double ttempo = rayInt->Test_RaySphereIntersect(eye, direction, bSphere, tempo);
	if(ttempo == -1)
	{
		t = -1.0;
		return;
	}
	//BOUNDING SPHERE

	zz[0] = 1.5;
	zz[1] = 0.2;
	zz[2] = 1.5;
	glm::mat4 Base = glm::scale(modelView, zz);
	zz[0] = 0.0;
	zz[1] = 0.4;
	zz[2] = 0.0;
	Base = glm::translate(Base, zz);

	zz[0] = 0.0;
	zz[1] = 0.8;
	zz[2] = 0.0;
	glm::mat4 Rod = glm::translate(modelView, zz);
	zz[0] = 0.5;
	zz[1] = 1.0;
	zz[2] = 0.5;
	Rod = glm::scale(Rod, zz);

	zz[0] = 0.0;
	zz[1] = 2.0;
	zz[2] = 0.0;
	glm::mat4 Ball = glm::translate(modelView, zz);
	zz[0] = 1.0;
	zz[1] = 1.0;
	zz[2] = 1.0;
	Ball = glm::scale(Ball, zz);

	double tt[3];
	glm::vec4 norm[3];

	//drawCuboid(Base, color);
	tt[0] = rayInt->Test_RayCubeIntersect(eye, direction, Base, norm[0]);
	//drawCylinder(Rod, color);
	tt[1] = rayInt->Test_RayCylinderIntersect(eye, direction, Rod, norm[1]);
	//drawSphere(Ball, color);
	tt[2] = rayInt->Test_RaySphereIntersect(eye, direction, Ball, norm[2]);

	double min = 1e26;
	int minIndex;
	for(int i = 0; i < 3; i++)
	{
		if((min > tt[i]) && (tt[i] > 0))
		{
			min = tt[i];
			minIndex = i;
		}
	}

	if(min != 1e26)
	{
		t = min;
		normal = norm[minIndex];
	}
	else
		t = -1;
}