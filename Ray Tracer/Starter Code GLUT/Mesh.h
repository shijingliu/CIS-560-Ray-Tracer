#pragma once
#include "furniture.h"
#include <string>
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include <vector>
#include "Face.h"

class Mesh :
	public Furniture
{
public:
	std::vector <glm::vec3> vertices;
	std::vector <Face*> faces;
	int noOfPoints;
	float lengthOfExtrusion;
	int radialSlices;
	bool convex;
	std::string meshType;
	Mesh(std::string, int, int, std::vector <glm::vec3>);
	Mesh(std::string, int, float, std::vector <glm::vec3>);
	~Mesh(void);
	void Mesh::drawFurniture(glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, float color[]);
	float giveHeight();
	void Mesh::intersect(glm::vec3 eye, glm::vec3 direction, glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, double &t, glm::vec4 &normal);

	//BOUNDING SPHERE
	float Mesh::giveXMax();
	float Mesh::giveXMin();
	float Mesh::giveZMax();
	float Mesh::giveZMin();
	//BOUNDING SPHERE
};

