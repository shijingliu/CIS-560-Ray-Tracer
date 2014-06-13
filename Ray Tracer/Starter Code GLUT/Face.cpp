#include "Face.h"


Face::Face(std::vector <glm::vec3> vertices, glm::vec3 normal)
{
	for(int i = 0; i < 3; i++)
	{
		this->vertices.push_back(vertices[i]);
	}
	this->normal = normal;
}


Face::~Face(void)
{
	vertices.clear();
}
