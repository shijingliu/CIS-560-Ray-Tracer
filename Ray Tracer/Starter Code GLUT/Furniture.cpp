#include "Furniture.h"


Furniture::Furniture(void)
{
	rayInt = new RayIntersection();
}


Furniture::~Furniture(void)
{
	delete rayInt;
}
