#include "RayIntersection.h"

#define EPSILON 0.0001

RayIntersection::RayIntersection(void)
{
}


RayIntersection::~RayIntersection(void)
{
}


double computeArea(vec3, vec3, vec3);

double RayIntersection::Test_RaySphereIntersect(vec3 const& P0, vec3 const& V0, mat4 const& T, vec4 &normal) {

	if(V0.length() == 0)
		return -1;
	//ADDED
	double result = -1.0;
	//ADDED
	vec4 P, V;
	double mag = V0[0] * V0[0] + V0[1] * V0[1] + V0[2] * V0[2];
	mag = sqrt(mag);					//Normalizing
	for(int i = 0; i < 3; i++)
	{
		P[i] = P0[i];
		V[i] = V0[i];
	}
	for(int i = 0; i < 3; i++)
	{
		V[i] = V[i] / mag;
	}
	P[3] = 1;
	V[3] = 0;
	mat4 Tinv;
	Tinv = inverse(T);
	P = Tinv * P;

	mat4 TT = T;
	TT[0][3] = 0;
	TT[1][3] = 0;
	TT[2][3] = 0;
	Tinv = inverse(TT);

	V = Tinv * V;
	
	//CORRECTION
	double A = V[0] * V[0] + V[1] * V[1] + V[2] * V[2];
	//CORRECTION
	double B = 2 * (V[0] * P[0] + V[1] * P[1] + V[2] * P[2]);
	double C = P[0] * P[0] + P[1] * P[1] + P[2] * P[2] - 1.0;
	double discr = B * B - 4 * A * C;
	if(discr < 0)
	{
		if(discr < 0 - EPSILON)
			result = -1;	//return -1;
		else
			discr = 0;
	}
	if(discr == 0)
	{
		//return (-1 * B / 2);
		result = (-1 * B / 2 * A);
	}
	else
	{
		discr = sqrt(discr);
		double t1 = ((-1 * B) + discr) / (2 * A);
		double t2 = ((-1 * B) - discr) / (2 * A);
		if((t1 >= 0 - EPSILON) && (t2 >= 0 - EPSILON))
		{
			if(t1 > t2)
				result = t2;	//return t2;
			else
				result = t1;	//return t1;
		}
		else if((t1 <= 0 +  EPSILON) && (t2 >= 0 - EPSILON))
		{
			result = t2;		//return t2;
		}
		else if((t2 <= 0 + EPSILON) && (t1 >= 0 - EPSILON))
			result = t1;		//return t1;
		else
			result = -1;		//return -1;
	}

	//ADDED
	if(abs(result) <= (0 + EPSILON))
		return -1;
	
	//RAY TRACER
	if(result != -1)
	{
		glm::vec4 norm;
		norm = P + V * result;
		norm[3] = 0;
		norm = T * norm;
		float normLength;
		normLength = sqrt(norm[0] * norm[0] + norm[1] * norm[1] + norm[2] * norm[2]);
		for(int i = 0; i < 3; i++)
			norm[i] = norm[i] / normLength;

		for(int i = 0; i < 4; i++)
		{
			//std::cout << norm[i] << "\t";
			normal[i] = norm[i];
		}
		normal[3] = 0;
	}
	//RAY TRACER

	return result;
	//ADDED
}

//double Test_RayPolyIntersect(vec3 const& P0, vec3 const& V0, vec3 const& p1, vec3 const& p2, vec3 const& p3, mat4 const& T) {
//	// TODO fill this in so it calls your own ray-casting function.
//	// See the documentation of this function in stubs.h.
//	if(V0.length() == 0)
//		return -1;
//	vec4 P, V;
//	double mag = V0[0] * V0[0] + V0[1] * V0[1] + V0[2] * V0[2];
//	mag = sqrt(mag);					//Normalizing
//	for(int i = 0; i < 3; i++)
//	{
//		P[i] = P0[i];
//		V[i] = V0[i];
//	}
//	for(int i = 0; i < 3; i++)
//	{
//		V[i] = V[i] / mag;
//	}
//	P[3] = 1;
//	V[3] = 0;
//	mat4 Tinv;
//	Tinv = inverse(T);
//	P = Tinv * P;
//
//	mat4 TT = T;
//	TT[0][3] = 0;
//	TT[1][3] = 0;
//	TT[2][3] = 0;
//	Tinv = inverse(TT);
//
//	V = Tinv * V;
//
//	vec3 a = p2 - p1;
//	vec3 b = p3 - p2;
//	vec3 N = cross(a, b);
//	mag = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
//	if(mag <= (0 + EPSILON))
//		return -1;
//	mag = sqrt(mag);
//	for(int i = 0; i < 3; i++)
//		N[i] = N[i] / mag;
//	double t;
//	if(abs(V[0] * N[0] + V[1] * N[1] + V[2] * N[2]) <= 0.0 + EPSILON)
//		return -1;
//	t = ((p1[0] - P[0]) * N[0] + (p1[1] - P[1]) * N[1] + (p1[2] - P[2]) * N[2]) / (V[0] * N[0] + V[1] * N[1] + V[2] * N[2]);
//	if(t < 0 + EPSILON)
//		return -1;
//	vec3 point(P[0] + t * V[0], P[1] + t * V[1], P[2] + t * V[2]);
//	double s = computeArea(p1, p2, p3);
//	double s1 = computeArea(point, p2, p3) / s;
//	double s2 = computeArea(point, p3, p1) / s;
//	double s3 = computeArea(point, p1, p2) / s;
//	
//	if(((s1 <= 1 + EPSILON) && (s1 >= 0 - EPSILON)) && ((s2 <= 1 + EPSILON) && (s2 >= 0 - EPSILON)) && ((s3 <= 1 + EPSILON) && (s3 >= 0 - EPSILON)))
//		if(((s1 + s2 + s3) <= (1 + EPSILON)) && ((s1 + s2 + s3) >= (1 - EPSILON)))
//		{
//			if(t <= 0 + EPSILON)
//				return -1;
//			//RAY TRACER
//			glm::vec4 point = P + V * t;
//			glm::vec4 ray = P - point;
//			glm::vec4 norm;
//			for(int i = 0; i < 3; i++)
//				norm[i] = N[i];
//			norm[3] = 0;
//			ray[3] = 0;
//			if(dot(norm, ray) < 0)
//				for(int i = 0; i < 3; i++)
//					norm[i] = -1 * norm[i];
//
//			norm = T * norm;
//			float normLength;
//			normLength = sqrt(norm[0] * norm[0] + norm[1] * norm[1] + norm[2] * norm[2]);
//			for(int i = 0; i < 3; i++)
//				norm[i] = norm[i] / normLength;
//
//			for(int i = 0; i < 4; i++)
//				std::cout << norm[i] << "\t";
//			std::cout << "\n";
//			//RAY TRACER
//			return t;
//		}
//	return -1;
//}

double RayIntersection::Test_RayPolyIntersect(vec3 const& P0, vec3 const& V0, vec3 const& p1, vec3 const& p2, vec3 const& p3, mat4 const& T, vec4 &normal) {
	// TODO fill this in so it calls your own ray-casting function.
	// See the documentation of this function in stubs.h.
	if(V0.length() == 0)
		return -1;
	vec4 P, V;
	double mag = V0[0] * V0[0] + V0[1] * V0[1] + V0[2] * V0[2];
	mag = sqrt(mag);					//Normalizing
	for(int i = 0; i < 3; i++)
	{
		P[i] = P0[i];
		V[i] = V0[i];
	}
	for(int i = 0; i < 3; i++)
	{
		V[i] = V[i] / mag;
	}
	P[3] = 1;
	V[3] = 0;
	mat4 Tinv;
	Tinv = inverse(T);
	P = Tinv * P;

	mat4 TT = T;
	TT[0][3] = 0;
	TT[1][3] = 0;
	TT[2][3] = 0;
	Tinv = inverse(TT);

	V = Tinv * V;

	vec3 a = p2 - p1;
	vec3 b = p3 - p2;
	vec3 N = cross(a, b);
	mag = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
	if(mag <= (0 + EPSILON))
		return -1;
	mag = sqrt(mag);
	for(int i = 0; i < 3; i++)
		N[i] = N[i] / mag;
	double t;
	if(abs(V[0] * N[0] + V[1] * N[1] + V[2] * N[2]) <= 0.0 + EPSILON)
		return -1;
	t = ((p1[0] - P[0]) * N[0] + (p1[1] - P[1]) * N[1] + (p1[2] - P[2]) * N[2]) / (V[0] * N[0] + V[1] * N[1] + V[2] * N[2]);
	if(t < 0 + EPSILON)
		return -1;
	vec3 point(P[0] + t * V[0], P[1] + t * V[1], P[2] + t * V[2]);
	double s = computeArea(p1, p2, p3);
	double s1 = computeArea(point, p2, p3) / s;
	double s2 = computeArea(point, p3, p1) / s;
	double s3 = computeArea(point, p1, p2) / s;
	
	if(((s1 <= 1 + EPSILON) && (s1 >= 0 - EPSILON)) && ((s2 <= 1 + EPSILON) && (s2 >= 0 - EPSILON)) && ((s3 <= 1 + EPSILON) && (s3 >= 0 - EPSILON)))
		if(((s1 + s2 + s3) <= (1 + EPSILON)) && ((s1 + s2 + s3) >= (1 - EPSILON)))
		{
			if(t <= 0 + EPSILON)
				return -1;
			//RAY TRACER
			glm::vec4 point = P + V * t;
			glm::vec4 ray = P - point;
			glm::vec4 norm;
			for(int i = 0; i < 3; i++)
				norm[i] = N[i];
			norm[3] = 0;
			ray[3] = 0;
			if(dot(norm, ray) < 0)
				for(int i = 0; i < 3; i++)
					norm[i] = -1 * norm[i];

			norm = T * norm;
			float normLength;
			normLength = sqrt(norm[0] * norm[0] + norm[1] * norm[1] + norm[2] * norm[2]);
			for(int i = 0; i < 3; i++)
			{
				norm[i] = norm[i] / normLength;
				normal[i] = norm[i];
			}
			normal[3] = 0;
			//RAY TRACER
			return t;
		}
	return -1;
}

double RayIntersection::Test_RayCubeIntersect(vec3 const& P0, vec3 const& V0, mat4 const& T, vec4 &normall) {
	// TODO fill this in so it calls your own ray-casting function.
	// See the documentation of this function in stubs.h.
	//vec4 P, V;																	NEED NOT BRING RAY TO OBJECT SPACE, AS Test_RayPolyIntersect DOES THAT
	
	//RAY TRACER
	vec4 normal[7];
	//RAY TRACER
	if(V0.length() == 0)
		return -1;

	double t1 = 1e26, t[7];
	t[1] = Test_RayPolyIntersect(P0, V0, vec3(-0.5, 0.5, 0.5), vec3(-0.5, -0.5, 0.5), vec3(0.5, -0.5, 0.5), T, normal[1]);
	if(t[1] < 0)
		t[1] = Test_RayPolyIntersect(P0, V0, vec3(-0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(0.5, -0.5, 0.5), T, normal[1]);
	t[2] = Test_RayPolyIntersect(P0, V0, vec3(0.5, 0.5, 0.5), vec3(0.5, -0.5, 0.5), vec3(0.5, -0.5, -0.5), T, normal[2]);
	if(t[2] < 0)
		t[2] = Test_RayPolyIntersect(P0, V0, vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, -0.5), vec3(0.5, -0.5, -0.5), T, normal[2]);
	t[3] = Test_RayPolyIntersect(P0, V0, vec3(0.5, 0.5, -0.5), vec3(0.5, -0.5, -0.5), vec3(-0.5, -0.5, -0.5), T, normal[3]);
	if(t[3] < 0)
		t[3] = Test_RayPolyIntersect(P0, V0, vec3(0.5, 0.5, -0.5), vec3(-0.5, 0.5, -0.5), vec3(-0.5, -0.5, -0.5), T, normal[3]);
	t[4] = Test_RayPolyIntersect(P0, V0, vec3(-0.5, 0.5, -0.5), vec3(-0.5, -0.5, -0.5), vec3(-0.5, -0.5, 0.5), T, normal[4]);
	if(t[4] < 0)
		t[4] = Test_RayPolyIntersect(P0, V0, vec3(-0.5, 0.5, -0.5), vec3(-0.5, 0.5, 0.5), vec3(-0.5, -0.5, 0.5), T, normal[4]);
	t[5] = Test_RayPolyIntersect(P0, V0, vec3(-0.5, -0.5, -0.5), vec3(-0.5, -0.5, 0.5), vec3(0.5, -0.5, 0.5), T, normal[5]);
	if(t[5] < 0)
		t[5] = Test_RayPolyIntersect(P0, V0, vec3(-0.5, -0.5, -0.5), vec3(0.5, -0.5, -0.5), vec3(0.5, -0.5, 0.5), T, normal[5]);
	t[6] = Test_RayPolyIntersect(P0, V0, vec3(-0.5, 0.5, -0.5), vec3(-0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), T, normal[6]);
	if(t[6] < 0)
		t[6] = Test_RayPolyIntersect(P0, V0, vec3(-0.5, 0.5, -0.5), vec3(0.5, 0.5, -0.5), vec3(0.5, 0.5, 0.5), T, normal[6]);

	int triangleIndex = -1;
	for(int i = 1; i <= 6; i++)
	{
		if(t[i] >= 0)
		{
			if(t1 > t[i])
			{
				t1 = t[i];
				triangleIndex = i;
			}
		}
		else
		{
		}
	}
	if(t1 != 1e26)
	{
		for(int j = 0; j < 3; j++)
		{
			//std::cout << normal[triangleIndex][j] << "\t";
			normall[j] = normal[triangleIndex][j];
		}
		normall[3] = 0;
		//std::cout << "\n";
		return t1;
	}
	return -1;
}

double RayIntersection::Test_RayCylinderIntersect(vec3 const& P0, vec3 const& V0, mat4 const& T, vec4 &normal) {
	// TODO fill this in so it calls your own ray-casting function.
	// See the documentation of this function in stubs.h.
	if(V0.length() == 0)
		return -1;
	double result = -1.0;
	vec4 P, V;
	double mag = V0[0] * V0[0] + V0[1] * V0[1] + V0[2] * V0[2];
	mag = sqrt(mag);					//Normalizing
	for(int i = 0; i < 3; i++)
	{
		P[i] = P0[i];
		V[i] = V0[i];
	}
	for(int i = 0; i < 3; i++)
	{
		V[i] = V[i] / mag;
	}
	P[3] = 1;
	V[3] = 0;

	mat4 Tinv;
	Tinv = inverse(T);
	P = Tinv * P;

	mat4 TT = T;
	TT[0][3] = 0;
	TT[1][3] = 0;
	TT[2][3] = 0;
	Tinv = inverse(TT);

	V = Tinv * V;

	//Intersection with cylindrical part

	double A = V[0] * V[0] + V[2] * V[2];
	double B = 2 * (P[0] * V[0] + P[2] * V[2]);
	double C = P[0] * P[0] + P[2] * P[2] - 0.25;
	double discr = B * B - 4 * A * C;

	if(discr < 0)
	{
		result = -1;
	}
	else if(abs(discr) <= (0 + EPSILON))
	{
		if((V[0] != 0) || (V[2] != 0))
		{
			result = (-1 * B / (2 * A));
		}
		else
			result = -1;
	}
	else
	{	
		discr = sqrt(discr);
		double t1 = ((-1 * B) + discr) / (2 * A);
		double t2 = ((-1 * B) - discr) / (2 * A);
		if((t1 >= 0 - EPSILON) && (t2 >= 0 - EPSILON))
		{
			if(t1 > t2)
			{
				result = t2;
				if((P[1] + V[1] * result >= (-0.5 - EPSILON)) && (P[1] + V[1] * result <= (0.5 + EPSILON)))
				{}
				else
					result = -1.0;
			}
			if((result == -1.0) || (t1 < t2))
			{
				result = t1;
				if((P[1] + V[1] * result >= (-0.5 - EPSILON)) && (P[1] + V[1] * result <= (0.5 + EPSILON)))
				{}
				else
					result = -1.0;
			}
		}
		else if((t1 <= 0 + EPSILON) && (t2 >= 0 - EPSILON))
		{
			result = t2;
			if((P[1] + V[1] * result >= (-0.5 - EPSILON)) && (P[1] + V[1] * result <= (0.5 + EPSILON)))
			{}
			else
				result = -1;
		}
		else if((t2 <= 0 + EPSILON) && (t1 >= 0 - EPSILON))
		{
			result = t1;
			if((P[1] + V[1] * result >= (-0.5 - EPSILON)) && (P[1] + V[1] * result <= (0.5 + EPSILON)))
			{}
			else
				result = -1;
		}
		else
			result = -1;
	}


	
	//Intersection with bottom and top parts
	double result2 = -1.0;
	if(V[1] != 0.0)
	{
		double t1, t2;
		t1 = (0.5 - P[1]) / V[1];
		t2 = (-0.5 - P[1]) / V[1];
		/*if((t1 >= 0) && (t2 >= 0))
		{
			if(t1 > t2)
			{
				result2 = t2;
				if((A * result2 * result2 + B * result2 + C) <= (0 + EPSILON))
				{}
				else
					result2 = -1;
			}
			if(result2 == -1)
			{
				result2 = t1;
				if((A * result2 * result2 + B * result2 + C) <= (0 + EPSILON))
				{}
				else
					result2 = -1;
			}
		}
		else if((t1 <= 0) && (t2 >= 0))
		{
			result2 = t2;
			if((A * result2 * result2 + B * result2 + C) <= (0 + EPSILON))
			{}
			else
				result2 = -1;
		}
		if((t2 <= 0) && (t1 >= 0) || (result2 == -1))
		{
			result2 = t1;
			if((A * result2 * result2 + B * result2 + C) <= (0 + EPSILON))
			{}
			else
				result2 = -1;
		}
		else
			result2 = -1;*/
		if((t1 >= 0 - EPSILON) && (t2 >= 0 - EPSILON))
		{
			if(t1 > t2)
			{
				result2 = t2;
				if((A * result2 * result2 + B * result2 + C) <= (0 + EPSILON))
				{}
				else
					result2 = -1.0;
			}
			if((result2 == -1.0) || (t1 < t2))
			{
				result2 = t1;
				if((A * result2 * result2 + B * result2 + C) <= (0 + EPSILON))
				{}
				else
					result2 = -1.0;
			}
		}
		else if((t1 <= 0 + EPSILON) && (t2 >= 0 - EPSILON))
		{
			result2 = t2;
			if((A * result2 * result2 + B * result2 + C) <= (0 + EPSILON))
			{}
			else
				result2 = -1;
		}
		else if((t2 <= 0 + EPSILON) && (t1 >= 0 - EPSILON))
		{
			result2 = t1;
			if((A * result2 * result2 + B * result2 + C) <= (0 + EPSILON))
			{}
			else
				result2 = -1;
		}
		else
			result2 = -1;
	}
	else
		result2 = -1;

	//ADDED
	if(abs(result) <= (0 + EPSILON))
		return -1;
	if(abs(result2) <= (0 + EPSILON))
		return -1;
	//ADDED

	//RAY TRACER
	glm::vec4 norm(0.0, 0.0, 0.0, 0.0);
	if(result < 0)
	{
		result = result2;
		if(P[1] < 0)
			norm[1] = -1;
		else
			norm[1] = 1;
	}
	if(result2 < 0)
	{
		norm = P + V * result;
		norm[1] = 0;		//For getting normal
		norm[3] = 0;		//For stopping translation
	}
	if((result >= 0) && (result2 >= 0) && (result > result2))
	{
		result = result2;
		if(P[1] < 0)
			norm[1] = -1;
		else
			norm[1] = 1;
	}

	if(result != -1)
	{
		norm = T * norm;
		float normLength;
		normLength = sqrt(norm[0] * norm[0] + norm[1] * norm[1] + norm[2] * norm[2]);
		for(int i = 0; i < 3; i++)
		{
			norm[i] = norm[i] / normLength;
		}
		for(int i = 0; i < 4; i++)
		{
			//std::cout << norm[i] << "\t";
			normal[i] = norm[i];
		}
		normal[3] = 0;
		//std::cout << "\n";
	}
	//RAY TRACER

	return result;
}



double computeArea(vec3 a, vec3 b, vec3 c)
{
	mat3 A(vec3(a[1], b[1], c[1]), vec3(a[2], b[2], c[2]), vec3(1.0, 1.0, 1.0));
	mat3 B(vec3(a[2], b[2], c[2]), vec3(a[0], b[0], c[0]), vec3(1.0, 1.0, 1.0));
	mat3 C(vec3(a[0], b[0], c[0]), vec3(a[1], b[1], c[1]), vec3(1.0, 1.0, 1.0));
	double x = determinant(A);
	double y = determinant(B);
	double z = determinant(C);
	x = x * x;
	y = y * y;
	z = z * z;
	x = 0.5 * sqrt(x + y + z);
	return x;
}