#pragma once
#include <iostream>
#include "glew.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../freeglut/include/GL/glut.h"
using namespace glm;
class RayIntersection
{
public:
	RayIntersection(void);
	~RayIntersection(void);
	double Test_RaySphereIntersect(vec3 const& P0, vec3 const& V0, mat4 const& T, vec4 &normal);
	//double Test_RayPolyIntersect(vec3 const& P0, vec3 const& V0, vec3 const& p1, vec3 const& p2, vec3 const& p3, mat4 const& T);
	double Test_RayPolyIntersect(vec3 const& P0, vec3 const& V0, vec3 const& p1, vec3 const& p2, vec3 const& p3, mat4 const& T, vec4 &normal);
	double Test_RayCubeIntersect(vec3 const& P0, vec3 const& V0, mat4 const& T, vec4 &normal);
	double Test_RayCylinderIntersect(vec3 const& P0, vec3 const& V0, mat4 const& T, vec4 &normal);
};

