#include "Floor.h"


Floor::Floor(void)
{
}


Floor::~Floor(void)
{
}

float Floor::giveHeight()
{
	return 1.0f;
}

void Floor::intersect(glm::vec3 eye, glm::vec3 direction, glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, double &t, glm::vec4 &normal)
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
	float maxa = max(this->xSize , this->zSize);
	float bind[3] = {0, 0, 1};
	zz[0] = maxa / 2.0;
	zz[1] = 0;
	zz[2] = maxa / 2.0;
	glm::mat4 bSphere = glm::translate(modelView, zz);
	zz[0] = (maxa / sqrt(2.0)) + 0.25;
	zz[1] = (maxa / sqrt(2.0)) + 0.25;
	zz[2] = (maxa / sqrt(2.0)) + 0.25;
	bSphere = glm::scale(bSphere, zz);
	//drawSphere(bSphere, bind);
	double tempo;
	glm::vec4 tempoN;
	tempo = rayInt->Test_RaySphereIntersect(eye, direction, bSphere, tempoN);
	if(tempo == -1)
	{
		t = -1;
		return;
	}
	//BOUNDING SPHERE

	zz[0] = (float)xSize / 2;
	zz[1] = 0.5;
	zz[2] = (float)zSize / 2;
	modelView = glm::translate(modelView, zz);
	zz[0] = xSize;
	zz[1] = 0.8;
	zz[2] = zSize;
	modelView = glm::scale(modelView, zz);
	

	//drawCuboid(modelView, color);
	t = rayInt->Test_RayCubeIntersect(eye, direction, modelView, normal);
}