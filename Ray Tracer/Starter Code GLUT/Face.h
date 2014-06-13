#pragma once
#include <vector>
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../freeglut/include/GL/glut.h"

class Face
{
public:
	std::vector <glm::vec3> vertices;
	glm::vec3 normal;

	Face(std::vector <glm::vec3>, glm::vec3);
	~Face(void);
};

