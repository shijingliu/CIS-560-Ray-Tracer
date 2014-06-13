#include "Cabinet.h"


Cabinet::Cabinet(void)
{
}


Cabinet::~Cabinet(void)
{
}

float Cabinet::giveHeight()
{
	//return 2.0f;
	return 3.3f;
}

void Cabinet::intersect(glm::vec3 eye, glm::vec3 direction, glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, double &t, glm::vec4 &normal)
{
	double tt[4];
	glm::vec4 norm[4];

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
	zz[0] = 0.95;
	zz[1] = 1.65;
	zz[2] = 0.95;
	glm::mat4 bSphere = glm::translate(modelView, zz);
	zz[0] = 2.2;
	zz[1] = 2.2;
	zz[2] = 2.2;
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

	glm::mat4 BigCube, BottomSmallCube, MidSmallCube, TopSmallCube;
	zz[0] = 2;
	zz[1] = 3.3;
	zz[2] = 1;
	BigCube = glm::scale(modelView, zz);
	zz[0] = 0.5;
	zz[1] = 0.5;
	zz[2] = 0.5;
	BigCube = glm::translate(BigCube, zz);
	//drawCuboid(BigCube, color);
	tt[0] = rayInt->Test_RayCubeIntersect(eye, direction, BigCube, norm[0]);

	zz[0] = 1.8;
	zz[1] = 0.8;
	zz[2] = 0.2;
	BottomSmallCube = glm::scale(modelView, zz);
	zz[0] = 0.535;
	zz[1] = 0.6;
	zz[2] = 5.7;
	BottomSmallCube = glm::translate(BottomSmallCube, zz);
	//drawCuboid(BottomSmallCube, color);
	tt[1] = rayInt->Test_RayCubeIntersect(eye, direction, BottomSmallCube, norm[1]);

	zz[0] = 1.8;
	zz[1] = 0.8;
	zz[2] = 0.2;
	MidSmallCube = glm::scale(modelView, zz);
	zz[0] = 0.535;
	zz[1] = 2.0;
	zz[2] = 5.7;
	MidSmallCube = glm::translate(MidSmallCube, zz);
	//drawCuboid(MidSmallCube, color);
	tt[2] = rayInt->Test_RayCubeIntersect(eye, direction, MidSmallCube, norm[2]);

	zz[0] = 1.8;
	zz[1] = 0.8;
	zz[2] = 0.2;
	TopSmallCube = glm::scale(modelView, zz);
	zz[0] = 0.535;
	zz[1] = 3.4;
	zz[2] = 5.7;
	TopSmallCube = glm::translate(TopSmallCube, zz);
	//drawCuboid(TopSmallCube, color);
	tt[3] = rayInt->Test_RayCubeIntersect(eye, direction, TopSmallCube, norm[3]);

	double min = 1e26;
	int minIndex = -1;

	for(int i = 0; i < 4; i++)
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