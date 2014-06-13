#include "Mesh.h"


Mesh::Mesh(std::string type, int noOfPoints, float lengthOfExt, std::vector <glm::vec3> vertices)
{
	//this->noOfPoints = noOfPoints;
	//meshType = type;
	//lengthOfExtrusion = lengthOfExt;

	//glm::vec3 a(0, 0, 0);
	////vertices = new glm::vec3[2 * noOfPoints];

	//for(int i = 0; i < noOfPoints; i++)
	//{
	//	vertices.push_back(vert[i]);//[i] = vert[i];
	//	vertices.insert(i + noOfPoints, (vert[i + noOfPoints]);//vertices[i + noOfPoints] = vert[i];
	//	vertices[i + noOfPoints][1] = lengthOfExtrusion;
	//}
	//convex = true;

	////Convex Testing
	//float temp = 0;
	//glm::vec3 c, d;
	//c = vertices[1] - vertices[0];
	//d = vertices[2] - vertices[1];
	//d = glm::cross(c, d);
	//temp = d[1];

	//for(int i = 1; i < noOfPoints - 2; i++)
	//{
	//	glm::vec3 a, b;
	//	a = vertices[i + 1] - vertices[i];
	//	b = vertices[i + 2] - vertices[i + 1];
	//	b = glm::cross(a, b);
	//	if(((temp > 0) && (b[1] < 0)) || ((temp < 0) && (b[1] > 0)))
	//		convex = false;
	//}

	////Convex

	//if(convex)
	//{
	//	normal = new glm::vec3[(2 * (noOfPoints - 2) + 2 * noOfPoints)];
	//	faces = new int*[(2 * (noOfPoints - 2) + 2 * noOfPoints)];
	//	for(int i = 0; i < (2 * (noOfPoints - 2) + 2 * noOfPoints); i++)
	//	{
	//		faces[i] = new int[3];
	//	}

	//	for(int i = 0; i < noOfPoints - 2; i++)
	//	{
	//		faces[i][0] = 0;
	//		faces[i][1] = i + 1;
	//		faces[i][2] = i + 2;
	//		glm::vec3 a, b;
	//		a = vertices[i + 1] - vertices[0];
	//		b = vertices[i + 2] - vertices[i + 1];
	//		normal[i] = glm::cross(b, a);

	//		faces[noOfPoints - 2 + i][0] = noOfPoints;
	//		faces[noOfPoints - 2 + i][1] = noOfPoints + i + 1;
	//		faces[noOfPoints - 2 + i][2] = noOfPoints + i + 2;
	//		//normal[noOfPoints - 2 + i] = -normal[i];
	//		normal[noOfPoints - 2 + i] = glm::cross(a, b);
	//	}

	//	for(int i = 0; i < noOfPoints; i++)
	//	{
	//		faces[i + (2 * (noOfPoints - 2))][0] = i;
	//		faces[i + (2 * (noOfPoints - 2))][1] = i + 1;
	//		faces[i + (2 * (noOfPoints - 2))][2] = i + noOfPoints;
	//		glm::vec3 a, b;
	//		a = vertices[i + 1] - vertices[i];
	//		b = vertices[i + noOfPoints] - vertices[i + 1];
	//		normal[i + (2 * (noOfPoints - 2))] = glm::cross(a, b);

	//		faces[i + (2 * (noOfPoints - 2)) + 1][0] = i + 1;
	//		faces[i + (2 * (noOfPoints - 2)) + 1][1] = i + 1 + noOfPoints;
	//		faces[i + (2 * (noOfPoints - 2)) + 1][2] = i + noOfPoints;
	//		normal[i + (2 * (noOfPoints - 2)) + 1] = normal[i + (2 * (noOfPoints - 2))];
	//	}
	//}
	//else
	//{
	//	normal = new glm::vec3[(2 * noOfPoints)];
	//	faces = new int*[(2 * noOfPoints)];
	//	for(int i = 0; i < (2 * noOfPoints); i++)
	//	{
	//		faces[i] = new int[3];
	//	}
	//	for(int i = 0; i < noOfPoints; i++)
	//	{
	//		faces[i][0] = i;
	//		faces[i][1] = i + 1;
	//		faces[i][2] = i + noOfPoints;
	//		glm::vec3 a, b;
	//		a = vertices[i + 1] - vertices[i];
	//		b = vertices[i + noOfPoints] - vertices[i + 1];
	//		normal[i] = glm::cross(a, b);

	//		faces[i + noOfPoints][0] = i + 1;
	//		faces[i + noOfPoints][1] = i + 1 + noOfPoints;
	//		faces[i + noOfPoints][2] = i + noOfPoints;
	//		normal[i + noOfPoints] = normal[i];
	//	}
	//}

	this->noOfPoints = noOfPoints;
	meshType = type;
	lengthOfExtrusion = lengthOfExt;
	convex = true;
	for(int i = 0; i < noOfPoints; i++)
	{
		this->vertices.push_back(vertices[i]);
	}

	for(int i = 0; i < noOfPoints; i++)
	{
		glm::vec3 vtemp = vertices[i];
		vtemp[1] = lengthOfExtrusion;
		this->vertices.push_back(vtemp);
	}

	convex = true;

	//Convex Testing
	float temp = 0;
	glm::vec3 c, d;
	c = vertices[1] - vertices[0];
	d = vertices[2] - vertices[1];
	d = glm::cross(c, d);
	temp = d[1];

	for(int i = 1; i < noOfPoints; i++)
	{
		glm::vec3 a, b;
		if(i < noOfPoints -2)
		{
			a = vertices[i + 1] - vertices[i];
			b = vertices[i + 2] - vertices[i + 1];
		}
		else if(i == noOfPoints - 2)
		{
			a = vertices[i + 1] - vertices[i];
			b = vertices[0] - vertices[i + 1];
		}
		else if(i == noOfPoints - 1)
		{
			a = vertices[0] - vertices[i];
			b = vertices[1] - vertices[0];
		}
		b = glm::cross(a, b);
		if(((temp > 0) && (b[1] < 0)) || ((temp < 0) && (b[1] > 0)))
			convex = false;
	}
	//Convex

	if(convex)
	{
		for(int i = 0; i < (noOfPoints - 2); i++)
		{
			std::vector <glm::vec3> vtemp;
			vtemp.push_back(vertices[0]);
			vtemp.push_back(vertices[i + 1]);
			vtemp.push_back(vertices[i + 2]);
			glm::vec3 a = vertices[i + 1] - vertices[0];
			glm::vec3 b = vertices[i + 2] - vertices[i + 1];
			//glm::vec3 tempNormal = glm::cross(b, a);
			glm::vec3 tempNormal = glm::cross(a, b);
			Face *f;
			f = new Face(vtemp, tempNormal);
			faces.push_back(f);

			glm::vec3 g = vertices[0];
			g[1] = lengthOfExtrusion;
			glm::vec3 h= vertices[i + 1];
			h[1] = lengthOfExtrusion;
			glm::vec3 s= vertices[i + 2];
			s[1] = lengthOfExtrusion;
			vtemp.clear();
			vtemp.push_back(g);
			vtemp.push_back(h);
			vtemp.push_back(s);
			//tempNormal = -tempNormal;
			tempNormal = -1.0f * tempNormal;
			f = new Face(vtemp, tempNormal);
			faces.push_back(f);
		}

		for(int i = 0; i < noOfPoints; i++)
		{
			if(i != noOfPoints - 1)
			{
				std::vector <glm::vec3> vtemp;
				vtemp.push_back(vertices[i]);
				vtemp.push_back(vertices[i + 1]);
				glm::vec3 g = vertices[i];
				g[1] = lengthOfExtrusion;
				vtemp.push_back(g);
				glm::vec3 a = vertices[i + 1] - vertices[i];
				glm::vec3 b = g - vertices[i + 1];
				//glm::vec3 tempNormal = glm::cross(a, b);
				glm::vec3 tempNormal = glm::cross(b, a);
				Face *f = new Face(vtemp, tempNormal);
				faces.push_back(f);

		
				glm::vec3 h = vertices[i + 1];
				h[1] = lengthOfExtrusion;
				vtemp.clear();
				vtemp.push_back(h);
				vtemp.push_back(g);
				vtemp.push_back(vertices[i + 1]);
				f = new Face(vtemp, tempNormal);
				faces.push_back(f);
			}
			else
			{
				std::vector <glm::vec3> vtemp;
				vtemp.push_back(vertices[i]);
				vtemp.push_back(vertices[0]);
				glm::vec3 g = vertices[i];
				g[1] = lengthOfExtrusion;
				vtemp.push_back(g);
				glm::vec3 a = vertices[0] - vertices[i];
				glm::vec3 b = g - vertices[0];
				//glm::vec3 tempNormal = glm::cross(a, b);
				glm::vec3 tempNormal = glm::cross(b, a);
				Face *f = new Face(vtemp, tempNormal);
				faces.push_back(f);

		
				glm::vec3 h = vertices[0];
				h[1] = lengthOfExtrusion;
				vtemp.clear();
				vtemp.push_back(h);
				vtemp.push_back(g);
				vtemp.push_back(vertices[0]);
				f = new Face(vtemp, tempNormal);
				faces.push_back(f);
			}
		}
	}

	else				//Concave
	{
		for(int i = 0; i < noOfPoints; i++)
		{
			if(i != noOfPoints - 1)
			{
				std::vector <glm::vec3> vtemp;
				vtemp.push_back(vertices[i]);
				vtemp.push_back(vertices[i + 1]);
				glm::vec3 g = vertices[i];
				g[1] = lengthOfExtrusion;
				vtemp.push_back(g);
				glm::vec3 a = vertices[i + 1] - vertices[i];
				glm::vec3 b = g - vertices[i + 1];
				glm::vec3 tempNormal = glm::cross(a, b);
				Face *f = new Face(vtemp, tempNormal);
				faces.push_back(f);

		
				glm::vec3 h = vertices[i + 1];
				h[1] = lengthOfExtrusion;
				vtemp.clear();
				vtemp.push_back(h);
				vtemp.push_back(g);
				vtemp.push_back(vertices[i + 1]);
				f = new Face(vtemp, tempNormal);
				faces.push_back(f);
			}
			else
			{
				std::vector <glm::vec3> vtemp;
				vtemp.push_back(vertices[i]);
				vtemp.push_back(vertices[0]);
				glm::vec3 g = vertices[i];
				g[1] = lengthOfExtrusion;
				vtemp.push_back(g);
				glm::vec3 a = vertices[0] - vertices[i];
				glm::vec3 b = g - vertices[0];
				glm::vec3 tempNormal = glm::cross(a, b);
				Face *f = new Face(vtemp, tempNormal);
				faces.push_back(f);

		
				glm::vec3 h = vertices[0];
				h[1] = lengthOfExtrusion;
				vtemp.clear();
				vtemp.push_back(h);
				vtemp.push_back(g);
				vtemp.push_back(vertices[0]);
				f = new Face(vtemp, tempNormal);
				faces.push_back(f);
			}
		}
	}
}

Mesh::Mesh(std::string type, int noOfPoints, int slices, std::vector <glm::vec3> vertices)
{
	/*this->noOfPoints = noOfPoints;
	meshType = type;
	vertices = vert;
	convex = true;
	radialSlices = slices;

	glm::vec3 a(0, 0, 0);
	vertices = new glm::vec3[((noOfPoints - 1) * radialSlices)];
	for(int i = 0; i < ((noOfPoints - 1) * radialSlices); i++)
	{
		vertices[0] = a;
	}

	faces = new int*[((noOfPoints - 1) * radialSlices)];
	for(int i = 0; i < ((noOfPoints - 1) * radialSlices); i++)
	{
		faces[i] = new int[3];
	}*/

	this->noOfPoints = noOfPoints;
	meshType = type;
	radialSlices = slices;
	for(int i = 0; i < noOfPoints; i++)
	{
		this->vertices.push_back(vertices[i]);
	}

	convex = true;		//Unnecessary

	float theta = 2 * 3.14159 / radialSlices;
	float tempTheta = theta;
	//glm::vec3 Vertices[noOfPoints];
	std::vector <glm::vec3> Vertices;
	for(int i = 0; i < vertices.size(); i++)
		Vertices.push_back(vertices[i]);
	for(int j = 0; j < radialSlices; j++)
	{	
		std::vector <glm::vec3> VVertices;
		for(int i = 0; i < noOfPoints - 1; i++)
		{
			glm::vec3 x, y;
			x[0] = Vertices[i][0] * cos(tempTheta) + Vertices[i][2] * sin(tempTheta);
			x[2] = -1 * Vertices[i][0] * sin(tempTheta) + Vertices[i][2] * cos(tempTheta);
			x[1] = Vertices[i][1];
			std::vector <glm::vec3> vtemp;
			vtemp.push_back(Vertices[i]);
			vtemp.push_back(x);
			vtemp.push_back(Vertices[i + 1]);
			glm::vec3 aa, bb;
			aa = x - Vertices[i];
			bb = Vertices[i + 1] - x;
			glm::vec3 tempNormal = glm::cross(aa, bb);
			Face *f = new Face(vtemp, tempNormal);
			faces.push_back(f);

			y[0] = Vertices[i + 1][0] * cos(tempTheta) + Vertices[i + 1][2] * sin(tempTheta);
			y[2] = -1 * Vertices[i + 1][0] * sin(tempTheta) + Vertices[i + 1][2] * cos(tempTheta);
			y[1] = Vertices[i + 1][1];
			vtemp.clear();
			vtemp.push_back(x);
			vtemp.push_back(y);
			vtemp.push_back(Vertices[i + 1]);
			f = new Face(vtemp, tempNormal);
			faces.push_back(f);

			VVertices.push_back(x);
			if(i == noOfPoints - 2)
				VVertices.push_back(y);
		}
		for(int z = 0; z < VVertices.size(); z++)
		{
			Vertices[z] = VVertices[z];
		}
		//tempTheta += theta;
	}

}


Mesh::~Mesh(void)
{
	vertices.clear();
	faces.clear();
}

float Mesh::giveHeight()
{
	if(meshType == "extrusion")
		return lengthOfExtrusion;
	else
	{
		float max = 0;
		for(int i = 0; i < vertices.size(); i++)
		{
			if(vertices[i][1] > max)
				max = vertices[i][1];
		}
		return max;
	}
}

//BOUNDING SPHERE
float Mesh::giveXMax()
{
	float max = 0;
	for(int i = 0; i < vertices.size(); i++)
	{
		if(vertices[i][0] > max)
			max = vertices[i][0];
	}
	return max;
}
float Mesh::giveZMax()
{
	float max = 0;
	for(int i = 0; i < vertices.size(); i++)
	{
		if(vertices[i][2] > max)
			max = vertices[i][2];
	}
	return max;
}
float Mesh::giveXMin()
{
	float min = 10000000;
	for(int i = 0; i < vertices.size(); i++)
	{
		if(vertices[i][0] < min)
			min = vertices[i][0];
	}
	return min;
}
float Mesh::giveZMin()
{
	float min = 10000000;
	for(int i = 0; i < vertices.size(); i++)
	{
		if(vertices[i][2] < min)
			min = vertices[i][2];
	}
	return min;
}
//BOUNDING SPHERE

void Mesh::intersect(glm::vec3 eye, glm::vec3 direction, glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, double &t, glm::vec4 &normal)
{
	glm::vec3 zz;
	zz[0] = xIndex;
	//Modified
	zz[1] = yIndex;
	//Modified
	zz[2] = zIndex;
	modelView = glm::translate(modelView, zz);
	modelView = glm::rotate(modelView, rotation, glm::vec3(0, 1, 0));
	zz[0] = xScale;
	zz[1] = yScale;
	zz[2] = zScale;
	modelView = glm::scale(modelView, zz);

	//BOUNDING SPHERE
	glm::mat4 bSphere;
	if(meshType == "extrusion")
	{
		zz[0] = (giveXMax() + giveXMin()) / 2.0;
		zz[1] = giveHeight() / 2.0;
		zz[2] = (giveZMax() + giveZMin()) / 2.0;
		bSphere = glm::translate(modelView, zz);

		float temporary = giveHeight() / 2;
		float radiusSq = temporary * temporary;
		temporary = (giveXMax() - giveXMin()) / 2.0;
		temporary = temporary * temporary;
		radiusSq += temporary;
		temporary = (giveZMax() - giveZMin()) / 2.0;
		temporary = temporary * temporary;
		radiusSq += temporary;

		float radius = sqrt(radiusSq);
		zz[0] = radius;
		zz[1] = radius;
		zz[2] = radius;
		bSphere = glm::scale(bSphere, zz);

		//drawSphere(bSphere, bind);
	}
	else
	{
		zz[0] = 0;
		zz[1] = giveHeight() / 2.0;
		zz[2] = 0;
		bSphere = glm::translate(modelView, zz);

		float temporary = giveHeight() / 2;
		float radiusSq = temporary * temporary;
		temporary = giveXMax();
		radiusSq += (temporary * temporary);

		float radius = sqrt(radiusSq);
		zz[0] = radius;
		zz[1] = radius;
		zz[2] = radius;
		bSphere = glm::scale(bSphere, zz);

		//drawSphere(bSphere, bind);
	}

	glm::vec4 tempoN;
	double ttempo = rayInt->Test_RaySphereIntersect(eye, direction, bSphere, tempoN);
	if(ttempo == -1)
	{
		t = -1;
		return;
	}
	//BOUNDING SPHERE

	glm::vec4 norm, tempNorm;
	double tt, min = 1e26;
	for(int i = 0; i < faces.size(); i++)
	{
		//drawTriangleOfMesh(modelView, faces[i]->vertices[0], faces[i]->vertices[1], faces[i]->vertices[2], faces[i]->normal, color);
		tt = rayInt->Test_RayPolyIntersect(eye, direction, faces[i]->vertices[0], faces[i]->vertices[1], faces[i]->vertices[2], modelView, tempNorm);
		if((min > tt) && (tt > 0))
		{
			min = tt;
			norm = tempNorm;
		}
	}
	if(min != 1e26)
	{
		t = min;
		normal = norm;
	}
	else
		t = -1;
}