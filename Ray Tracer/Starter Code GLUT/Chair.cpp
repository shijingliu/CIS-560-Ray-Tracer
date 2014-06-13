#include "Chair.h"


Chair::Chair(void)
{
}


Chair::~Chair(void)
{
}

float Chair::giveHeight()
{
	//return 2.25f;
	//rayInt = 0;
	return 1.5f;
}

void Chair::intersect(glm::vec3 eye, glm::vec3 direction, glm::mat4 modelView, 
	int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, double &t, glm::vec4 &normal)
{
	//RAY TRACER
	glm::vec4 norm[6];
	double tt[6];
	//RAY TRACER

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
	zz[1] = 0.75;
	zz[2] = 0;
	glm::mat4 bSphere = glm::translate(modelView, zz);
	zz[0] = 1.2;
	zz[1] = 1.2;
	zz[2] = 1.2;
	bSphere = glm::scale(bSphere, zz);
	//drawSphere(bSphere, bind);
	glm::vec4 tempoN;
	double ttempo = rayInt->Test_RaySphereIntersect(eye, direction, bSphere, tempoN);
	if(ttempo == -1)
	{
		t = -1;
		return;
	}
	//BOUNDING SPHERE

	glm::mat4 LeftBackLeg, RightBackLeg, LeftFrontLeg, RightFrontLeg, Seat, BackSupport;
	zz[0] = 0.1;
	zz[1] = 1.0;
	zz[2] = 0.1;
	LeftBackLeg = glm::scale(modelView, zz);
	RightBackLeg = glm::scale(modelView, zz);
	LeftFrontLeg = glm::scale(modelView, zz);
	RightFrontLeg = glm::scale(modelView, zz);

	zz[0] = 0.5;
	zz[1] = 0.5;
	zz[2] = 0.5;
	LeftBackLeg = glm::translate(LeftBackLeg, zz);

	zz[0] = 5.5;
	zz[1] = 0.5;
	zz[2] = 0.5;
	RightBackLeg = glm::translate(RightBackLeg, zz);

	zz[0] = 0.5;
	zz[1] = 0.5;
	zz[2] = 5.5;
	LeftFrontLeg = glm::translate(LeftFrontLeg, zz);

	zz[0] = 5.5;
	zz[1] = 0.5;
	zz[2] = 5.5;
	RightFrontLeg = glm::translate(RightFrontLeg, zz);

	//drawCuboid(LeftBackLeg, color);
	tt[0] = (rayInt->Test_RayCubeIntersect(eye, direction, LeftBackLeg, norm[0]));
	//drawCuboid(RightBackLeg, color);
	tt[1] = rayInt->Test_RayCubeIntersect(eye, direction, RightBackLeg, norm[1]);
	//drawCuboid(LeftFrontLeg, color);
	tt[2] = rayInt->Test_RayCubeIntersect(eye, direction, LeftFrontLeg, norm[2]);
	//drawCuboid(RightFrontLeg, color);
	tt[3] = rayInt->Test_RayCubeIntersect(eye, direction, RightFrontLeg, norm[3]);

	zz[0] = 0.6;
	zz[1] = 0.1;
	zz[2] = 0.6;
	Seat = glm::scale(modelView, zz);
	zz[0] = 0.5;
	zz[1] = 10;
	zz[2] = 0.5;
	Seat = glm::translate(Seat, zz);
	//drawCuboid(Seat, color);
	tt[4] = rayInt->Test_RayCubeIntersect(eye, direction, Seat, norm[4]);

	zz[0] = 0.6;
	zz[1] = 0.6;
	zz[2] = 0.1;
	BackSupport = glm::scale(modelView, zz);
	zz[0] = 0.5;
	zz[1] = 2;
	zz[2] = 0.5;
	BackSupport = glm::translate(BackSupport, zz);
	//drawCuboid(BackSupport, color);
	tt[5] = rayInt->Test_RayCubeIntersect(eye, direction, BackSupport, norm[5]);

	double min = 1e26;
	int minIndex = -1;
	for(int i = 0; i < 6; i++)
	{
		if((tt[i] < min) && (tt[i] > 0))
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