#include "glew.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../freeglut/include/GL/glut.h"
#include <fstream>
#include <iostream>
#include <time.h>

#include <string>
#include "Node.h"
#include "Furniture.h"
#include "Chair.h"
#include "Table.h"
#include "Lamp.h"
#include "Cabinet.h"
#include "Floor.h"
#include "Mesh.h"
#include "Tree.h"

//RAY TRACER
#include "EasyBMP.h"

#define KS 0.3f
#define KD 0.5f
#define KA 0.2f
#define EPSILON 0.001
//RAY TRACER

using namespace glm;

//vertex arrays needed for drawing
unsigned int vbo;
unsigned int cbo;
unsigned int nbo;
unsigned int ibo;

//attributes
unsigned int positionLocation = 0;
unsigned int colorLocation = 1;
unsigned int normalLocation = 2;

//uniforms
unsigned int u_modelMatrixLocation;
unsigned int u_projMatrixLocation;
unsigned int u_posLocation;
unsigned int u_colLocation;
unsigned int u_eyeLocation;
	
//needed to compile and link and use the shaders     
unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;

//Animation/transformation stuff
clock_t old;
float rotation = 0.0f;


//ADDED
Tree *tree;
Node *selection;
Node *previousSelection;
float previousRGB[3];
int selectionIterator = 0;
//ADDED

//RAY TRACER
std::string rayTracedImage;
int width, height;
glm::vec3 eyep;
glm::vec3 vdir;
glm::vec3 uvec;
float fovy;
struct light
{
	glm::vec3 lpos;
	glm::vec3 lcol;
};
std::vector <light> lights;
int lightIterator = 0;
glm::vec3 acol;
struct material
{
	glm::vec3 dcol;
	float specular, reflectivity, transparency, refraction, eight;
};

void rayTrace();
glm::vec3 rayTrace(glm::vec3 eyep, glm::vec3 direction, int depth, glm::vec3 color);

glm::vec3 backRGB(0.0, 0.0, 0.0);

//RAY TRACER

//helper function to read shader source and put it in a char array
//thanks to Swiftless
char* textFileRead(const char*);

//some other helper functions from CIS 565
void printLinkInfoLog(int);
void printShaderInfoLog(int);

//standard glut-based program functions
void init(void);
void resize(int, int);
void display(void);
void keypress(unsigned char, int, int);
void cleanup(void);

void createRedSquare(mat4);
void createBlueSquare(mat4);
//ADDED
void createMyScene(mat4);
void drawTriangle(mat4, glm::vec3, glm::vec3, glm::vec3, int, float[]);
void drawRectangle(mat4, glm::vec3, glm::vec3, glm::vec3, int, float[]);
void drawCuboid(mat4, glm::vec3, glm::vec3, glm::vec3, glm::vec3, float[]);
void drawCuboid(mat4, float[]);
void drawCylinder(mat4, glm::vec3, float, float, float[]);
void drawCylinder(mat4, float[]);
void drawSphere(mat4, glm::vec3, float, float[]);
void drawSphere(mat4, float[]);
void drawHourGlass(mat4, glm::vec3, float, float[]);
void drawSpecialRectangle(glm::mat4, glm::vec3, glm::vec3, glm::vec3, glm::vec3,float[]);
void drawSpecialRectangle2(glm::mat4, glm::vec3, glm::vec3, glm::vec3, glm::vec3, glm::vec3, float[]);
//ADDED


//ADDED
void readConfigFile(int, char **);
//ADDED

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Starter Code");

	//Call GLEW only _after_ you get the window
	//I had to tell the author of your textbook that ;-)  -Cory
	glewInit();

	init();
	//ADDED
	readConfigFile(argc, argv);
	//ADDED
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keypress);
	glutIdleFunc(display);

	glutMainLoop();
	return 0;
}

void init() {
	//Create the VBOs and IBO we'll be using to render images in OpenGL
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &cbo);
	glGenBuffers(1, &nbo);
	glGenBuffers(1, &ibo);
	
	//Everybody does this
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);

	//here is stuff for setting up our shaders
	const char* fragFile = "diffuseFrag.frag";
	const char* vertFile = "diffuseVert.vert";
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();
	
	//load up the source, compile and link the shader program
	const char* vertSource = textFileRead(vertFile);
	const char* fragSource = textFileRead(fragFile);
	glShaderSource(vertexShader, 1, &vertSource, 0);
	glShaderSource(fragmentShader, 1, &fragSource, 0);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	//For your convenience, i decided to throw in some compiler/linker output helper functions
	//from CIS 565
	GLint compiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printShaderInfoLog(vertexShader);
	} 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printShaderInfoLog(fragmentShader);
	} 

	//set the attribute locations for our shaders
	glBindAttribLocation(shaderProgram, positionLocation, "vs_position");
	glBindAttribLocation(shaderProgram, normalLocation, "vs_normal");
	glBindAttribLocation(shaderProgram, colorLocation, "vs_color");

	//finish shader setup
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//check for linking success
	GLint linked;
	glGetProgramiv(shaderProgram,GL_LINK_STATUS, &linked);
	if (!linked) 
	{
		printLinkInfoLog(shaderProgram);
	}

	//Get the uniform locations for our shaders, unfortunately they can not be set by us, we have
	//to ask OpenGL for them
	u_modelMatrixLocation = glGetUniformLocation(shaderProgram, "u_modelMatrix");
	u_projMatrixLocation = glGetUniformLocation(shaderProgram, "u_projMatrix");
	u_posLocation = glGetUniformLocation(shaderProgram, "u_pos");
	u_colLocation = glGetUniformLocation(shaderProgram, "u_col");
	u_eyeLocation = glGetUniformLocation(shaderProgram, "eye_pos");
	//Always remember that it doesn't do much good if you don't have OpenGL actually use the shaders
	glUseProgram(shaderProgram);

	resize(640, 480);
								old = clock();
}

void cleanup() {
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &cbo);
	glDeleteBuffers(1, &nbo);
	glDeleteBuffers(1, &ibo);

	//Tear down the shader program in reverse of building it
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
}

void keypress(unsigned char key, int x, int y) {
	switch(key) {
	case 'q':
		cleanup();
		exit(0);
		break;

	//RAY TRACER
	case 'p':
		rayTrace();
		break;
	//RAY TRACER

	//ADDED
	case 'n':

		//Selection algorithm assumes the special tree that gets constructed for the Scene Graph   
		if(tree->root->children.size() == 0)
			return;

		if(previousSelection == 0)
		{
		}
		else
		{
			previousSelection->r = previousRGB[0];
			previousSelection->g = previousRGB[1];
			previousSelection->b = previousRGB[2];
		}
		previousRGB[0] = selection->r;
		previousRGB[1] = selection->g;
		previousRGB[2] = selection->b;
		selection->r = 1;
		selection->g = 1;
		selection->b = 1;
		previousSelection = selection;
		if(selection == tree->root)
		{
			selection = selection->children[selectionIterator];
			selectionIterator++;
		}
		else
		{
			if(selection->children.size() != 0)
				selection = selection->children[0];
			else
			{
				if(selectionIterator == (tree->root->children.size()))
					selectionIterator = 0;
				selection = tree->root;
			}
		}
		
		break;
	case 'a':
		previousSelection->xIndex = previousSelection->xIndex - 1;
		break;
	case 'd':
		previousSelection->xIndex = previousSelection->xIndex + 1;
		break;
	case 'w':
		previousSelection->zIndex = previousSelection->zIndex + 1;
		break;
	case 's':
		previousSelection->zIndex = previousSelection->zIndex - 1;
		break;
	case 'x':
		previousSelection->xScale = previousSelection->xScale + 0.5;
		break;
	case 'X':
		previousSelection->xScale = previousSelection->xScale - 0.5;
		break;
	case 'y':
		previousSelection->yScale = previousSelection->yScale + 0.5;
		break;
	case 'Y':
		previousSelection->yScale = previousSelection->yScale - 0.5;
		break;
	case 'z':
		previousSelection->zScale = previousSelection->zScale + 0.5;
		break;
	case 'Z':
		previousSelection->zScale = previousSelection->zScale - 0.5;
		break;
	case 'r':
		previousSelection->rotation = previousSelection->rotation + 10;
		break;
	case 'R':
		previousSelection->rotation = previousSelection->rotation - 10;
		break;
	case 'e':
		if(previousSelection != tree->root)
		{
			tree->deleteNode(previousSelection);
			previousSelection = tree->root;
			selection = tree->root;
			previousRGB[0] = selection->r;
			previousRGB[1] = selection->g;
			previousRGB[2] = selection->b;
			selectionIterator--;
			if(selectionIterator > tree->root->children.size())
				selectionIterator = 0;
		}
		break;
	//ADDED
	}

	

	glutPostRedisplay();
}



void readConfigFile(int argc, char **argv)
{
	Furniture *furnitureType;
	Floor *floor = new Floor();
	Chair *chair = new Chair();
	Table *table = new Table();
	Cabinet *cabinet = new Cabinet();
	Lamp *lamp = new Lamp();

	//RAY TRACER
	material mate[3];
	std::ifstream configFile(argv[1]);
	if (configFile.is_open())
	{
		std::string temp;
		configFile >> temp >> rayTracedImage;
		configFile >> temp >> width >> height;
		configFile >> temp >> eyep[0] >> eyep[1] >> eyep[2];
		configFile >> temp >> vdir[0] >> vdir[1] >> vdir[2];
		configFile >> temp >> uvec[0] >> uvec[1] >> uvec[2];
		configFile >> temp >> fovy;
		fovy = fovy * 3.14159 / 180;
		while(true)
		{
			glm::vec3 pos, col;
			configFile >> temp;
			if(temp == "LPOS")
			{
				configFile >> pos[0] >> pos[1] >> pos[2];
				configFile >> temp >> col[0] >> col[1] >> col[2];
				light l;
				l.lpos = pos;
				l.lcol = col;
				lights.push_back(l);
			}
			else
			{
				break;
			}
		}
		configFile >> acol[0] >> acol[1] >> acol[2];
		for(int i = 0; i < 3; i++)
		{
			configFile >> temp >> mate[i].dcol[0] >> mate[i].dcol[1] >> mate[i].dcol[2];
			configFile >> mate[i].specular >> mate[i].reflectivity >> mate[i].transparency >> mate[i].refraction >> mate[i].eight;
		}
	}
	//RAY TRACER

	int size = 0, xSize, zSize, numItems, xIndex, zIndex;
	float rotation, xScale, yScale, zScale;
	for(int i = 2; i < argc; i++)
	{
		size += strlen(argv[i]);
	}
	char* fileName = new char(size + 2);
	strcpy(fileName, argv[2]);
	for(int i = 3; i < argc; i++)
	{
		strcat(fileName, " ");
		strcat(fileName, argv[i]);
	}
	std::ifstream testFile(fileName);
	if (testFile.is_open())
	{
		std::string fur, data;
		//RAY TRACER
		int matNumber;
		//RAY TRACER
		testFile >> xSize >> zSize >> numItems;
		floor->xSize = xSize;
		floor->zSize = zSize;
		glm::vec3 dcol(0, 0, 1);
		Node *root = new Node("floor", floor, 0, 0,
			0, 1, 1, 1, dcol, 5, 0.8, 0, 0, 0);
					//new Node ("floor", floor, 0, 0, 0, 0, 1, 0, 1, 1, 1);
		tree = new Tree(root);
		selection = tree->root;
		for(int i = 0; ((i < numItems) && (!testFile.eof())); i++)
		{
			testFile >> fur;
			if(fur == "chair")
			{
				furnitureType = chair;
				testFile >> matNumber;
			}
			else if(fur == "table")
			{
				furnitureType = table;
				testFile >> matNumber;
			}
			else if(fur == "cabinet")
			{
				furnitureType = cabinet;
				testFile >> matNumber;
			}
			else if(fur == "lamp")
			{
				furnitureType = lamp;
				testFile >> matNumber;
			}
			else if(fur == "mesh")
			{
				std::string meshType;
				float lengthOfExtrusion = 0.0f;
				int noOfSlices;
				std::vector <vec3> vect;
				int noOfPoints = 0;
				testFile >> data >> matNumber;
				std::ifstream testData(data);
				if(testData.is_open())
				{
					testData >> meshType;
					if(meshType == "extrusion")
					{
						testData >> lengthOfExtrusion >> noOfPoints;
						--noOfPoints;
					}
					else if(meshType == "surfrev")
					{
						testData >> noOfSlices >> noOfPoints;
					}
					else
					{
						std::cout << "Wrong Type of Mesh\n";
						return;
					}
					
					if(meshType == "surfrev")
						for(int i = 0; i < noOfPoints; i++)
						{
							float a, b;
							testData >> a >> b;
							glm::vec3 vtemp (a, b, 0.0);
							vect.push_back(vtemp);
						}

					if(meshType == "extrusion")
						for(int i = 0; i < noOfPoints; i++)
						{
							float a, b;
							testData >> a >> b;
							glm::vec3 vtemp (a, 0.0, b);
							vect.push_back(vtemp);
						}

					if(meshType == "extrusion")
					{
						Mesh *mesh = new Mesh(meshType, noOfPoints, lengthOfExtrusion, vect);
						furnitureType = mesh;
					}
					else if(meshType == "surfrev")
					{
						if(vect[noOfPoints - 1][0] != 0.0)
						{
							glm::vec3 vtemp (0.0, vect[noOfPoints - 1][1], 0.0);
							vect.push_back(vtemp);
							noOfPoints++;
						}
						Mesh *mesh = new Mesh(meshType, noOfPoints, noOfSlices, vect);
						furnitureType = mesh;
					}
				}
				else
				{
					std::cout << "File " << data.c_str() << " not present\n";
					return;
				}
			}
			//RAY TRACER
			testFile >> xIndex >> zIndex >> rotation >> xScale >> yScale >> zScale;

			rotation = (rotation * 3.14159) / 180;
			Node *n = new Node(fur, furnitureType, xIndex, zIndex, rotation, xScale, yScale, zScale, 
				mate[matNumber - 1].dcol, mate[matNumber - 1].specular, mate[matNumber - 1].reflectivity, mate[matNumber - 1].transparency,
				mate[matNumber - 1].refraction, mate[matNumber - 1].eight);
			tree->add(n);
			//RAY TRACER
		}
		tree->adjustValues();	// For making xIndex and zIndex of children = 0
	}
	else
	{
		std::cout << fileName << " File missing\n";
		//exit(1);
	}
	//tree->printTree();
	//tree->drawSceneGraph();
}

void drawCuboid(glm::mat4 modelView, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, float color[])
{
	drawRectangle(modelView, a, b, c, -1, color);
	drawRectangle(modelView, (d + (a - b)), d, (d + (c - b)), 1, color);
	drawRectangle(modelView, (a + (c - b)), c, (d + (c - b)), -1, color);
	drawRectangle(modelView, a, b, d, 1, color);
	drawRectangle(modelView, (a + (c - b)), a, (d + (a - b)), 1, color);
	drawRectangle(modelView, c, b, d, -1, color);
}

void drawCuboid(glm::mat4 modelView, float color[])
{
	vec3 a(-0.5, 0.5, -0.5), b(-0.5, -0.5, -0.5), c(0.5, -0.5, -0.5), d(-0.5, -0.5, 0.5);
	drawRectangle(modelView, a, b, c, -1, color);
	drawRectangle(modelView, (d + (a - b)), d, (d + (c - b)), 1, color);
	drawRectangle(modelView, (a + (c - b)), c, (d + (c - b)), -1, color);
	drawRectangle(modelView, a, b, d, 1, color);
	drawRectangle(modelView, (a + (c - b)), a, (d + (a - b)), 1, color);
	drawRectangle(modelView, c, b, d, -1, color);
}

void drawHourGlass(glm::mat4 modelView, glm::vec3 a, float radius, float color[])
{
	glm::vec3 b, c, d, aa;
	for(float phi = 0; phi < 360; phi += 15)
	{
		for(float theta = 0; theta < 360; theta += 15)
		{
			float x = a[0] + radius * cos(3.14159 * phi / 180) * cos(3.14159 * theta / 180);
			float y = a[1] + radius * cos(3.14159 * phi / 180);
			float z = a[2] + radius * cos(3.14159 * phi / 180) * sin(3.14159 * theta / 180);
			b[0] = x;
			b[1] = y;
			b[2] = z;
			x = a[0] + radius * cos(3.14159 * phi / 180) * cos(3.14159 * (theta + 15) / 180);
			z = a[2] + radius * cos(3.14159 * phi / 180) * sin(3.14159 * (theta + 15) / 180);
			c[0] = x;
			c[1] = y;
			c[2] = z;
			x = a[0] + radius * cos(3.14159 * (phi + 15)/ 180) * cos(3.14159 * (theta + 15) / 180);
			y = a[1] + radius * cos(3.14159 * (phi + 15)/ 180);
			z = a[2] + radius * cos(3.14159 * (phi + 15)/ 180) * sin(3.14159 * (theta + 15) / 180);
			d[0] = x;
			d[1] = y;
			d[2] = z;
			drawRectangle(modelView, b, c, d, -1, color);
		}
	}
}

void drawSphere(glm::mat4 modelView, glm::vec3 a, float radius, float color[])
{
	glm::vec3 b, c, d, aa;
	/*for(float phi = 0; phi < 360; phi += 15)
	{
		for(float theta = 0; theta < 360; theta += 15)
		{
			float x = a[0] + radius * sin(3.14159 * phi / 180) * cos(3.14159 * theta / 180);
			float y = a[1] + radius * sin(3.14159 * phi / 180);
			float z = a[2] + radius * sin(3.14159 * phi / 180) * sin(3.14159 * theta / 180);
			b[0] = x;
			b[1] = y;
			b[2] = z;
			x = a[0] + radius * sin(3.14159 * phi / 180) * cos(3.14159 * (theta + 15) / 180);
			z = a[2] + radius * sin(3.14159 * phi / 180) * sin(3.14159 * (theta + 15) / 180);
			c[0] = x;
			c[1] = y;
			c[2] = z;
			x = a[0] + radius * sin(3.14159 * (phi + 15)/ 180) * cos(3.14159 * (theta + 15) / 180);
			y = a[1] + radius * sin(3.14159 * (phi + 15)/ 180);
			z = a[2] + radius * sin(3.14159 * (phi + 15)/ 180) * sin(3.14159 * (theta + 15) / 180);
			d[0] = x;
			d[1] = y;
			d[2] = z;
			drawRectangle(modelView, b, c, d, 1, color);
		}
	}*/
	for(float phi = 0; phi < 360; phi += 5)
	{
		for(float theta = 0; theta < 360; theta += 5)
		{
			float x = a[0] + radius * cos(3.14159 * phi / 180) * cos(3.14159 * theta / 180);
			float y = a[1] + radius * sin(3.14159 * phi / 180);
			float z = a[2] + radius * cos(3.14159 * phi / 180) * sin(3.14159 * theta / 180);
			b[0] = x;
			b[1] = y;
			b[2] = z;
			x = a[0] + radius * cos(3.14159 * phi / 180) * cos(3.14159 * (theta + 5) / 180);
			z = a[2] + radius * cos(3.14159 * phi / 180) * sin(3.14159 * (theta + 5) / 180);
			c[0] = x;
			c[1] = y;
			c[2] = z;
			x = a[0] + radius * cos(3.14159 * (phi + 5)/ 180) * cos(3.14159 * (theta + 5) / 180);
			y = a[1] + radius * sin(3.14159 * (phi + 5)/ 180);
			z = a[2] + radius * cos(3.14159 * (phi + 5)/ 180) * sin(3.14159 * (theta + 5) / 180);
			d[0] = x;
			d[1] = y;
			d[2] = z;
			drawSpecialRectangle(modelView, b, c, d, a, color);
		}
	}
}

void drawSphere(glm::mat4 modelView, float color[])
{
	glm::vec3 a(0.0, 0.0, 0.0);
	float radius = 1;
	glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 b, c, d, aa;
	for(float phi = 0; phi < 360; phi += 5)
	{
		float cosPhi = cos(3.14159 * phi / 180);
		float sinPhi = sin(3.14159 * phi / 180);
		float cosNextPhi = cos(3.14159 * (phi + 5) / 180);
		float sinNextPhi = sin(3.14159 * (phi + 5) / 180);
		for(float theta = 0; theta < 360; theta += 5)
		{
			float cosTheta = cos(3.14159 * theta / 180);
			float sinTheta = sin(3.14159 * theta / 180);
			float cosNextTheta = cos(3.14159 * (theta + 5) / 180);
			float sinNextTheta = sin(3.14159 * (theta + 5) / 180);

			float x = a[0] + radius * cosPhi * cosTheta;
			float y = a[1] + radius * sinPhi;
			float z = a[2] + radius * cosPhi * sinTheta;
			b[0] = x;
			b[1] = y;
			b[2] = z;
			x = a[0] + radius * cosPhi * cosNextTheta;
			z = a[2] + radius * cosPhi * sinNextTheta;
			c[0] = x;
			c[1] = y;
			c[2] = z;
			x = a[0] + radius * cosNextPhi * cosNextTheta;
			y = a[1] + radius * sinNextPhi;
			z = a[2] + radius * cosNextPhi * sinNextTheta;
			d[0] = x;
			d[1] = y;
			d[2] = z;
			drawSpecialRectangle(modelView, b, c, d, a, color);
		}
	}
}

void drawCylinder(glm::mat4 modelView, glm::vec3 a, float radius, float height, float color[])
{
	glm::vec3 b, c, d, aa;
	for(float theta = 0; theta < 360; theta += 15)
	{
		float x = a[0] + radius * cos(3.14159 * theta / 180);
		float z = a[2] + radius * sin(3.14159 * theta / 180);
		b[0] = x;
		b[1] = a[1];
		b[2] = z;
		x = a[0] + radius * cos(3.14159 * (theta + 15) / 180);
		z = a[2] + radius * sin(3.14159 * (theta + 15) / 180);
		c[0] = x;
		c[1] = a[1];
		c[2] = z;
		d[0] = c[0];
		d[1] = a[1] + height;
		d[2] = c[2];

		aa[0] = a[0];
		aa[1] = a[1] + height;
		aa[2] = a[2];
		drawTriangle(modelView, a, b, c, 1, color);
		drawSpecialRectangle2(modelView, b, c, d, a, aa, color);
		//drawRectangle(modelView, b, c, d, -1, color);
		b[1] = aa[1];
		c[1] = aa[1];
		drawTriangle(modelView, aa, b, c, 1, color);
	}
}

void drawCylinder(glm::mat4 modelView, float color[])
{
	glm::vec3 a(0.0, -0.5, 0.0);
	float radius = 0.5;
	float height = 1;
	glm::vec3 b, c, d, aa;
	for(float theta = 0; theta < 360; theta += 15)
	{
		float cosTheta = cos(3.14159 * theta / 180);
		float sinTheta = sin(3.14159 * theta / 180);
		float cosNextTheta = cos(3.14159 * (theta + 15) / 180);
		float sinNextTheta = sin(3.14159 * (theta + 15) / 180);

		float x = a[0] + radius * cosTheta;
		float z = a[2] + radius * sinTheta;
		b[0] = x;
		b[1] = a[1];
		b[2] = z;
		x = a[0] + radius * cosNextTheta;
		z = a[2] + radius * sinNextTheta;
		c[0] = x;
		c[1] = a[1];
		c[2] = z;
		d[0] = c[0];
		d[1] = a[1] + height;
		d[2] = c[2];

		aa[0] = a[0];
		aa[1] = a[1] + height;
		aa[2] = a[2];
		drawTriangle(modelView, a, b, c, 1, color);
		drawSpecialRectangle2(modelView, b, c, d, a, aa, color);
		//drawRectangle(modelView, b, c, d, -1, color);
		b[1] = aa[1];
		c[1] = aa[1];
		drawTriangle(modelView, aa, b, c, 1, color);
	}
}

void drawTriangle(glm::mat4 modelView, glm::vec3 a, glm::vec3 b, glm::vec3 c, int normal, float color[])
{
	float *vertices = new float[12];
	vertices[0] = a[0]; vertices[1] = a[1]; vertices[2] = a[2]; vertices[3] = 1.0f;
	vertices[4] = b[0]; vertices[5] = b[1]; vertices[6] = b[2]; vertices[7] = 1.0f;
	vertices[8] = c[0]; vertices[9] = c[1]; vertices[10] = c[2]; vertices[11] = 1.0f;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);
	delete [] vertices;

	float* colors = new float[9];
	colors[0] = color[0]; colors[1] = color[1]; colors[2] = color[2];
	colors[3] = color[0]; colors[4] = color[1]; colors[5] = color[2];
	colors[6] = color[0]; colors[7] = color[1]; colors[8] = color[2];
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colors, GL_STREAM_DRAW);
	delete [] colors;

	float* normals = new float[12];
	glm::vec3 n0 = (b - a);
	glm::vec3 n1 = (c - b);
	n0 = glm::cross(n0, n1);
	float mag = sqrt(n0[0] * n0[0] + n0[1] * n0[1] + n0[2] * n0[2]);
	n0 = n0 / mag;
	if(normal == -1)
		n0 = -n0;

	normals[0] = n0[0]; normals[1] = n0[1]; normals[2] = n0[2]; normals[3] = 0.0f;
	normals[4] = n0[0]; normals[5] = n0[1]; normals[6] = n0[2]; normals[7] = 0.0f;
	normals[8] = n0[0]; normals[9] = n0[1]; normals[10] = n0[2]; normals[11] = 0.0f;

	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), normals, GL_STATIC_DRAW);
	delete [] normals;

	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(normalLocation);
	
	//we're using the vertex data first
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//define how the vertex pointer should work, in our case we're accessing floats 4 at a time with no special pattern
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//now use color data, remember we're not using 4 at a time anymore
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//one more time with the normals
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//the last thing we need to do is setup our indices
	unsigned short* indices = new unsigned short[3];
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned short), indices, GL_STATIC_DRAW);
	delete [] indices;

	//set the modelview uniform
	glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &modelView[0][0]);

	//draw the elements
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	
	//shut off the information since we're done drawing
	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(normalLocation);
}

void drawTriangleOfMesh(glm::mat4 modelView, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 normal, float color[])
{
	float *vertices = new float[12];
	vertices[0] = a[0]; vertices[1] = a[1]; vertices[2] = a[2]; vertices[3] = 1.0f;
	vertices[4] = b[0]; vertices[5] = b[1]; vertices[6] = b[2]; vertices[7] = 1.0f;
	vertices[8] = c[0]; vertices[9] = c[1]; vertices[10] = c[2]; vertices[11] = 1.0f;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);
	delete [] vertices;

	float* colors = new float[9];
	colors[0] = color[0]; colors[1] = color[1]; colors[2] = color[2];
	colors[3] = color[0]; colors[4] = color[1]; colors[5] = color[2];
	colors[6] = color[0]; colors[7] = color[1]; colors[8] = color[2];
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colors, GL_STREAM_DRAW);
	delete [] colors;

	float* normals = new float[12];

	normals[0] = normal[0]; normals[1] = normal[1]; normals[2] = normal[2]; normals[3] = 0.0f;
	normals[4] = normal[0]; normals[5] = normal[1]; normals[6] = normal[2]; normals[7] = 0.0f;
	normals[8] = normal[0]; normals[9] = normal[1]; normals[10] = normal[2]; normals[11] = 0.0f;

	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), normals, GL_STATIC_DRAW);
	delete [] normals;

	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(normalLocation);
	
	//we're using the vertex data first
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//define how the vertex pointer should work, in our case we're accessing floats 4 at a time with no special pattern
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//now use color data, remember we're not using 4 at a time anymore
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//one more time with the normals
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//the last thing we need to do is setup our indices
	unsigned short* indices = new unsigned short[3];
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned short), indices, GL_STATIC_DRAW);
	delete [] indices;

	//set the modelview uniform
	glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &modelView[0][0]);

	//draw the elements
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	
	//shut off the information since we're done drawing
	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(normalLocation);
}

void drawRectangle(glm::mat4 modelView, glm::vec3 a, glm::vec3 b, glm::vec3 c, int normal, float color[])
{
	//set up a transformation matrix, in this case we're doing rotation only
				//modelView = glm::rotate(modelView, rotation, glm::vec3(1, 0, 0));

	//these four points define where the quad would be BEFORE transformations
	//this is referred to as object-space and it's best to center geometry at the origin for easier transformations
	float* vertices = new float[16];
	
	vertices[0] = a[0]; vertices[1] = a[1]; vertices[2] = a[2]; vertices[3] = 1.0f;
	vertices[4] = b[0]; vertices[5] = b[1]; vertices[6] = b[2]; vertices[7] = 1.0f;
	vertices[8] = c[0]; vertices[9] = c[1]; vertices[10] = c[2]; vertices[11] = 1.0f;
	glm::vec3 d = c + a - b;
	vertices[12] = d[0]; vertices[13] = d[1]; vertices[14] = d[2]; vertices[15] = 1.0f;

	//now we put the data into the Vertex Buffer Object for the graphics system to use
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_STATIC_DRAW); //the square vertices don't need to change, ever,
																				 //while the program runs

	//once the data is loaded, we can delete the float arrays, the data is safely stored with OpenGL
	delete [] vertices;

	//again with colors
	float* colors = new float[12];
	
	colors[0] = color[0]; colors[1] = color[1]; colors[2] = color[2];
	colors[3] = color[0]; colors[4] = color[1]; colors[5] = color[2];
	colors[6] = color[0]; colors[7] = color[1]; colors[8] = color[2];
	colors[9] = color[0]; colors[10] = color[1]; colors[11] = color[2];

	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	//always make sure you are telling OpenGL the right size to make the buffer, color data doesn't have as much data!
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STREAM_DRAW);	//the color is going to change every frame
																				//as it bounces between squares
	delete [] colors;

	//once more, this time with normals
	float* normals = new float[16];
	
	glm::vec3 n0 = (b - a);
	glm::vec3 n1 = (c - b);
	n0 = glm::cross(n0, n1);
	float mag = sqrt(n0[0] * n0[0] + n0[1] * n0[1] + n0[2] * n0[2]);
	n0 = n0 / mag;
	if(normal == -1)
		n0 = -n0;

	normals[0] = n0[0]; normals[1] = n0[1]; normals[2] = n0[2]; normals[3] = 0.0f;
	normals[4] = n0[0]; normals[5] = n0[1]; normals[6] = n0[2]; normals[7] = 0.0f;
	normals[8] = n0[0]; normals[9] = n0[1]; normals[10] = n0[2]; normals[11] = 0.0f;
	normals[12] = n0[0]; normals[13] = n0[1]; normals[14] = n0[2]; normals[15] = 0.0f;

	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), normals, GL_STATIC_DRAW); //the square normals don't need to change, ever,
																				 //while the program runs
	delete [] normals;

	//activate our three kinds of information
	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(normalLocation);
	
	//we're using the vertex data first
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//define how the vertex pointer should work, in our case we're accessing floats 4 at a time with no special pattern
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//now use color data, remember we're not using 4 at a time anymore
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//one more time with the normals
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//the last thing we need to do is setup our indices
	unsigned short* indices = new unsigned short[6];

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 3; indices[4] = 0; indices[5] = 2;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), indices, GL_STATIC_DRAW);

	delete [] indices;

	//set the modelview uniform
	glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &modelView[0][0]);

	//draw the elements
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	
	//shut off the information since we're done drawing
	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(normalLocation);
}


void drawSpecialRectangle(glm::mat4 modelView, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 center, float color[])
{
	//set up a transformation matrix, in this case we're doing rotation only
				//modelView = glm::rotate(modelView, rotation, glm::vec3(1, 0, 0));

	//these four points define where the quad would be BEFORE transformations
	//this is referred to as object-space and it's best to center geometry at the origin for easier transformations
	float* vertices = new float[16];
	
	vertices[0] = a[0]; vertices[1] = a[1]; vertices[2] = a[2]; vertices[3] = 1.0f;
	vertices[4] = b[0]; vertices[5] = b[1]; vertices[6] = b[2]; vertices[7] = 1.0f;
	vertices[8] = c[0]; vertices[9] = c[1]; vertices[10] = c[2]; vertices[11] = 1.0f;
	glm::vec3 d = c + a - b;
	vertices[12] = d[0]; vertices[13] = d[1]; vertices[14] = d[2]; vertices[15] = 1.0f;

	//now we put the data into the Vertex Buffer Object for the graphics system to use
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_STATIC_DRAW); //the square vertices don't need to change, ever,
																				 //while the program runs

	//once the data is loaded, we can delete the float arrays, the data is safely stored with OpenGL
	delete [] vertices;

	//again with colors
	float* colors = new float[12];
	
	colors[0] = color[0]; colors[1] = color[1]; colors[2] = color[2];
	colors[3] = color[0]; colors[4] = color[1]; colors[5] = color[2];
	colors[6] = color[0]; colors[7] = color[1]; colors[8] = color[2];
	colors[9] = color[0]; colors[10] = color[1]; colors[11] = color[2];

	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	//always make sure you are telling OpenGL the right size to make the buffer, color data doesn't have as much data!
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STREAM_DRAW);	//the color is going to change every frame
																				//as it bounces between squares
	delete [] colors;

	//once more, this time with normals
	float* normals = new float[16];

	glm::vec3 n0, n1, n2, n3;
	n0 = a - center;
	n1 = b - center;
	n2 = c - center;
	n3 = d - center;

	normals[0] = n0[0]; normals[1] = n0[1]; normals[2] = n0[2]; normals[3] = 0.0f;
	normals[4] = n1[0]; normals[5] = n1[1]; normals[6] = n1[2]; normals[7] = 0.0f;
	normals[8] = n2[0]; normals[9] = n2[1]; normals[10] = n2[2]; normals[11] = 0.0f;
	normals[12] = n3[0]; normals[13] = n3[1]; normals[14] = n3[2]; normals[15] = 0.0f;

	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), normals, GL_STATIC_DRAW); //the square normals don't need to change, ever,
																				 //while the program runs
	delete [] normals;

	//activate our three kinds of information
	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(normalLocation);
	
	//we're using the vertex data first
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//define how the vertex pointer should work, in our case we're accessing floats 4 at a time with no special pattern
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//now use color data, remember we're not using 4 at a time anymore
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//one more time with the normals
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//the last thing we need to do is setup our indices
	unsigned short* indices = new unsigned short[6];

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 3; indices[4] = 0; indices[5] = 2;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), indices, GL_STATIC_DRAW);

	delete [] indices;

	//set the modelview uniform
	glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &modelView[0][0]);

	//draw the elements
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	
	//shut off the information since we're done drawing
	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(normalLocation);
}

void drawSpecialRectangle2(glm::mat4 modelView, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 center, glm::vec3 center2, float color[])
{
	//these four points define where the quad would be BEFORE transformations
	//this is referred to as object-space and it's best to center geometry at the origin for easier transformations
	float* vertices = new float[16];
	
	vertices[0] = a[0]; vertices[1] = a[1]; vertices[2] = a[2]; vertices[3] = 1.0f;
	vertices[4] = b[0]; vertices[5] = b[1]; vertices[6] = b[2]; vertices[7] = 1.0f;
	vertices[8] = c[0]; vertices[9] = c[1]; vertices[10] = c[2]; vertices[11] = 1.0f;
	glm::vec3 d = c + a - b;
	vertices[12] = d[0]; vertices[13] = d[1]; vertices[14] = d[2]; vertices[15] = 1.0f;

	//now we put the data into the Vertex Buffer Object for the graphics system to use
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_STATIC_DRAW); //the square vertices don't need to change, ever,
																				 //while the program runs

	//once the data is loaded, we can delete the float arrays, the data is safely stored with OpenGL
	delete [] vertices;

	//again with colors
	float* colors = new float[12];
	
	colors[0] = color[0]; colors[1] = color[1]; colors[2] = color[2];
	colors[3] = color[0]; colors[4] = color[1]; colors[5] = color[2];
	colors[6] = color[0]; colors[7] = color[1]; colors[8] = color[2];
	colors[9] = color[0]; colors[10] = color[1]; colors[11] = color[2];

	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	//always make sure you are telling OpenGL the right size to make the buffer, color data doesn't have as much data!
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STREAM_DRAW);	//the color is going to change every frame
																				//as it bounces between squares
	delete [] colors;

	//once more, this time with normals
	float* normals = new float[16];

	glm::vec3 n0, n1, n2, n3;
	n0 = a - center;
	n1 = b - center;
	n2 = c - center2;
	n3 = d - center2;

	normals[0] = n0[0]; normals[1] = n0[1]; normals[2] = n0[2]; normals[3] = 0.0f;
	normals[4] = n1[0]; normals[5] = n1[1]; normals[6] = n1[2]; normals[7] = 0.0f;
	normals[8] = n2[0]; normals[9] = n2[1]; normals[10] = n2[2]; normals[11] = 0.0f;
	normals[12] = n3[0]; normals[13] = n3[1]; normals[14] = n3[2]; normals[15] = 0.0f;

	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), normals, GL_STATIC_DRAW); //the square normals don't need to change, ever,
																				 //while the program runs
	delete [] normals;

	//activate our three kinds of information
	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(normalLocation);
	
	//we're using the vertex data first
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//define how the vertex pointer should work, in our case we're accessing floats 4 at a time with no special pattern
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//now use color data, remember we're not using 4 at a time anymore
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//one more time with the normals
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glVertexAttribPointer(normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));
	
	//the last thing we need to do is setup our indices
	unsigned short* indices = new unsigned short[6];

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 3; indices[4] = 0; indices[5] = 2;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), indices, GL_STATIC_DRAW);

	delete [] indices;

	//set the modelview uniform
	glUniformMatrix4fv(u_modelMatrixLocation, 1, GL_FALSE, &modelView[0][0]);

	//draw the elements
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	
	//shut off the information since we're done drawing
	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(normalLocation);
}

void resize(int width, int height) {
	//set the viewport, more boilerplate
	glViewport(0, 0, width, height);
	glm::vec3 locpos = glm::vec3(0.0, 0, 5.0);
	glm::vec3 loccol = glm::vec3(1.0, 1.0, 1.0);
	glm::vec3 epos = glm::vec3(0.0, 5.0, 10.0);
	//RAY TRACER
	if(lights.size() != 0)
	{
		locpos = lights[lightIterator].lpos;
		loccol = lights[lightIterator].lcol;
		epos = eyep;
		lightIterator++;
		if(lightIterator == lights.size())
			lightIterator = 0;
	}
	//RAY TRACER

	//
	glm::mat4 projection = glm::perspective(60.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 30.0f);
	//glm::mat4 camera = glm::lookAt(glm::vec3(0, 0, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 camera = glm::lookAt(epos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	projection = projection * camera;

	//set the projection matrix here, it only needs to be changed if the screen is resized otherwise it can stay the same
	glUniformMatrix4fv(u_projMatrixLocation, 1, GL_FALSE, &projection[0][0]);
	glUniform3fv(u_posLocation, 1, &locpos[0]);
	glUniform3fv(u_colLocation, 1, &loccol[0]);
	glUniform3fv(u_eyeLocation, 1, &epos[0]);
	glutPostRedisplay();
}

void display() {
	//Always and only do this at the start of a frame, it wipes the slate clean
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
					//clock_t newTime = clock();

	//part of the animation
								//rotation += 150 * (static_cast<float>(newTime - old) / static_cast<float>(CLOCKS_PER_SEC));

	//create an identity matrix for the modelview matrix
	glm::mat4 modelView = glm::mat4(1.0);

	//Draw the two components of our scene separately, for your scenegraphs it will help
	//your sanity to do separate draws for each type of primitive geometry, otherwise your VBOs will
	//get very, very complicated fast
	//createRedSquare(modelView);	//PASS MODELVIEW BY COPY to get the nice scoping for hierarchical (i'm sure i spelled that wrong) transformations
								//like Norm mentioned in his Scene Graph lecture
	//createBlueSquare(modelView);


	//ADDED
	
	//modelView = glm::rotate(modelView, rotation, glm::vec3(0, 0, 1));
	//createMyScene(modelView);
	tree->drawSceneGraph();
	//tree->printTree();
	//ADDED

	glutSwapBuffers();
						//old = newTime;
}

//ADDED

void createMyScene(mat4 modelView)
{
	//modelView = glm::rotate(modelView, rotation, glm::vec3(0, 0, 1));
	//vec3 a(1, 1, -2), b(1, 0, -2), c(2, 0, -2), d(1, 0, -1);   

	vec3 a(0, 0, 0);
	float color[] = {1, 0, 0};
	float colorFloor[] = {0, 1, 0};
	//drawCuboid(modelView, color);
	//drawCuboid(modelView, a, b, c, d, color);
	//drawCylinder(modelView, a, 1, 5, color);
	//drawCylinder(modelView, color);
	//drawTriangle(modelView, a, b, c, 1, color);
	//drawSphere(modelView, a, 1, color);
	//drawSphere(modelView, color);
	//drawRectangle(modelView, vec3(-5, 0, -5), vec3(5, 0, -5), vec3(5, 0, 5), 1, colorFloor);
	//drawRectangle(modelView, vec3(0, 0, 0), vec3(5, 0, 0), vec3(5, 0, 5), 1, colorFloor);
	//Lamp *lamp = new Lamp();
	//lamp->drawFurniture(modelView, 0, 0, 0, 0, 1, 1, 1, color);
	/*Floor *floor = new Floor();
	floor->xSize = 5;
	floor->zSize =  5;
	floor->drawFurniture(modelView, 0, 0, 0, 0, 1, 1, 1, color);*/
	//Chair *chair = new Chair();
	//chair->drawFurniture(modelView, 0, 0, 0, 0, 1, 1, 1, color);
	Table *table = new Table();
	table->drawFurniture(modelView, 0, 0, 0, 0, 1, 1, 1, color);
	//Cabinet *cabinet = new Cabinet();
	//cabinet->drawFurniture(modelView, 0, 0, 0, 0, 1, 1, 1, color);
	//drawRectangle(modelView, vec3(0, 3.3, 0), vec3(5, 3.3, 0), vec3(5, 3.3, 5), 1, colorFloor);
}

//from swiftless.com
char* textFileRead(const char* fileName) {
    char* text;
    
    if (fileName != NULL) {
        FILE *file = fopen(fileName, "rt");
        
        if (file != NULL) {
            fseek(file, 0, SEEK_END);
            int count = ftell(file);
            rewind(file);
            
            if (count > 0) {
                text = (char*)malloc(sizeof(char) * (count + 1));
                count = fread(text, sizeof(char), count, file);
                text[count] = '\0';	//cap off the string with a terminal symbol, fixed by Cory
            }
            fclose(file);
        }
    }
    return text;
}

void printLinkInfoLog(int prog) 
{
	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogLen);

	// should additionally check for OpenGL errors here

	if (infoLogLen > 0)
	{
		infoLog = new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetProgramInfoLog(prog,infoLogLen, &charsWritten, infoLog);
		std::cout << "InfoLog:" << std::endl << infoLog << std::endl;
		delete [] infoLog;
	}
}

void printShaderInfoLog(int shader)
{
	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

	// should additionally check for OpenGL errors here

	if (infoLogLen > 0)
	{
		infoLog = new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetShaderInfoLog(shader,infoLogLen, &charsWritten, infoLog);
		std::cout << "InfoLog:" << std::endl << infoLog << std::endl;
		delete [] infoLog;
	}

	// should additionally check for OpenGL errors here
}


void Lamp::drawFurniture(glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, float color[])
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

	////BOUNDING SPHERE
	//float bind[3] = {0, 0, 1};
	//zz[0] = 0;
	//zz[1] = 1.5;
	//zz[2] = 0;
	//glm::mat4 bSphere = glm::translate(modelView, zz);
	//zz[0] = 2;
	//zz[1] = 2;
	//zz[2] = 2;
	//bSphere = glm::scale(bSphere, zz);
	//drawSphere(bSphere, bind);
	////BOUNDING SPHERE

	zz[0] = 1.5;
	zz[1] = 0.2;
	zz[2] = 1.5;
	glm::mat4 Base = glm::scale(modelView, zz);
	zz[0] = 0.0;
	zz[1] = 0.4;
	zz[2] = 0.0;
	Base = glm::translate(Base, zz);

	zz[0] = 0.0;
	zz[1] = 0.8;
	zz[2] = 0.0;
	glm::mat4 Rod = glm::translate(modelView, zz);
	zz[0] = 0.5;
	zz[1] = 1.0;
	zz[2] = 0.5;
	Rod = glm::scale(Rod, zz);

	zz[0] = 0.0;
	zz[1] = 2.0;
	zz[2] = 0.0;
	glm::mat4 Ball = glm::translate(modelView, zz);
	zz[0] = 1.0;
	zz[1] = 1.0;
	zz[2] = 1.0;
	Ball = glm::scale(Ball, zz);

	drawCuboid(Base, color);
	drawCylinder(Rod, color);
	drawSphere(Ball, color);
}


void Floor::drawFurniture(glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, float color[])
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

	zz[0] = (float)xSize / 2;
	zz[1] = 0.5;
	zz[2] = (float)zSize / 2;
	modelView = glm::translate(modelView, zz);
	zz[0] = xSize;
	zz[1] = 0.8;
	zz[2] = zSize;
	modelView = glm::scale(modelView, zz);
	

	drawCuboid(modelView, color);
}


void Chair::drawFurniture(glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, float color[])
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

	drawCuboid(LeftBackLeg, color);
	drawCuboid(RightBackLeg, color);
	drawCuboid(LeftFrontLeg, color);
	drawCuboid(RightFrontLeg, color);

	zz[0] = 0.6;
	zz[1] = 0.1;
	zz[2] = 0.6;
	Seat = glm::scale(modelView, zz);
	zz[0] = 0.5;
	zz[1] = 10;
	zz[2] = 0.5;
	Seat = glm::translate(Seat, zz);
	drawCuboid(Seat, color);

	zz[0] = 0.6;
	zz[1] = 0.6;
	zz[2] = 0.1;
	BackSupport = glm::scale(modelView, zz);
	zz[0] = 0.5;
	zz[1] = 2;
	zz[2] = 0.5;
	BackSupport = glm::translate(BackSupport, zz);
	drawCuboid(BackSupport, color);
}


void Cabinet::drawFurniture(glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, float color[])
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

	////BOUNDING SPHERE
	//float bind[3] = {0, 0, 1};
	//zz[0] = 0.95;
	//zz[1] = 1.65;
	//zz[2] = 0.95;
	//glm::mat4 bSphere = glm::translate(modelView, zz);
	//zz[0] = 2.2;
	//zz[1] = 2.2;
	//zz[2] = 2.2;
	//bSphere = glm::scale(bSphere, zz);
	//drawSphere(bSphere, bind);
	////BOUNDING SPHERE

	glm::mat4 BigCube, BottomSmallCube, MidSmallCube, TopSmallCube;
	zz[0] = 2;
	zz[1] = 3.3;
	zz[2] = 1;
	BigCube = glm::scale(modelView, zz);
	zz[0] = 0.5;
	zz[1] = 0.5;
	zz[2] = 0.5;
	BigCube = glm::translate(BigCube, zz);
	drawCuboid(BigCube, color);

	zz[0] = 1.8;
	zz[1] = 0.8;
	zz[2] = 0.2;
	BottomSmallCube = glm::scale(modelView, zz);
	zz[0] = 0.535;
	zz[1] = 0.6;
	zz[2] = 5.7;
	BottomSmallCube = glm::translate(BottomSmallCube, zz);
	drawCuboid(BottomSmallCube, color);

	zz[0] = 1.8;
	zz[1] = 0.8;
	zz[2] = 0.2;
	MidSmallCube = glm::scale(modelView, zz);
	zz[0] = 0.535;
	zz[1] = 2.0;
	zz[2] = 5.7;
	MidSmallCube = glm::translate(MidSmallCube, zz);
	drawCuboid(MidSmallCube, color);

	zz[0] = 1.8;
	zz[1] = 0.8;
	zz[2] = 0.2;
	TopSmallCube = glm::scale(modelView, zz);
	zz[0] = 0.535;
	zz[1] = 3.4;
	zz[2] = 5.7;
	TopSmallCube = glm::translate(TopSmallCube, zz);
	drawCuboid(TopSmallCube, color);
}


void Table::drawFurniture(glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, float color[])
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

	////BOUNDING SPHERE
	//float bind[3] = {0, 0, 1};
	//zz[0] = 0;
	//zz[1] = 0.525;
	//zz[2] = 0;
	//glm::mat4 bSphere = glm::translate(modelView, zz);
	//zz[0] = 1.1;
	//zz[1] = 1.1;
	//zz[2] = 1.1;
	//bSphere = glm::scale(bSphere, zz);
	//drawSphere(bSphere, bind);
	////BOUNDING SPHERE

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

	drawCuboid(LeftBackLeg, color);
	drawCuboid(RightBackLeg, color);
	drawCuboid(LeftFrontLeg, color);
	drawCuboid(RightFrontLeg, color);

	zz[0] = 0.75;
	zz[1] = 0.1;
	zz[2] = 0.75;
	Seat = glm::scale(modelView, zz);
	zz[0] = 0.4;
	zz[1] = 10;
	zz[2] = 0.4;
	Seat = glm::translate(Seat, zz);
	drawCuboid(Seat, color);
}

void Mesh::drawFurniture(glm::mat4 modelView, int xIndex, int yIndex, int zIndex, float rotation, float xScale, float yScale, float zScale, float color[])
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

	

	for(int i = 0; i < faces.size(); i++)
	{
		drawTriangleOfMesh(modelView, faces[i]->vertices[0], faces[i]->vertices[1], faces[i]->vertices[2], faces[i]->normal, color);
	}
}

void rayTrace()
{
	float*** color;
	color = new float**[height];
	for(int i = 0; i < height; i++)
	{
		color[i] = new float*[width];
		for(int j = 0; j < width; j++)
		{
			color[i][j] = new float[3];
		}
	}


	float c[3];								//Vector C
	float dist = (height/2.0)/(tan(fovy));	//Distance of screen from eye

	for(int i = 0; i < 3; i++)
	{
		c[i] = dist * vdir[i];
	}

	float a[3];								// Vector A
	a[0] = c[1] * uvec[2] - c[2] * uvec[1];
	a[1] = c[2] * uvec[0] - c[0] * uvec[2];
	a[2] = c[0] * uvec[1] - c[1] * uvec[0];
	float b[3];								// Vector B
	b[0] = a[1] * c[2] - a[2] * c[1];
	b[1] = a[2] * c[0] - a[0] * c[2];
	b[2] = a[0] * c[1] - a[1] * c[0];

	float m[3];								//Vector M
	for(int i = 0; i < 3; i++)
	{
		m[i] = c[i] + eyep[i];
	}

	float h[3];								//Vector H
	float v[3];								//Vector V
	
	for(int i = 0; i < 3; i++)
	{
		h[i] = ((width/2) * a[i]) / (sqrt((a[0] * a[0] + a[1] * a[1] + a[2] * a[2])));
	}
	
	for(int i = 0; i < 3; i++)
	{
		v[i] = (dist * tan(fovy) * b[i]) / (sqrt((b[0] * b[0] + b[1] * b[1] + b[2] * b[2])));
	}

	float p[3];								//Arbitrary point on screen

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			float diff[3];					//(p[i] - eye[i])/|p[i] - eye[i]|  The normal vector
			glm::vec3 direction;			//For using in RAY TRACER
			for(int d = 0; d < 3; d++)
			{
				p[d] = m[d] + (2 * ((float)j / (width - 1)) - 1) * h[d] + (2 * ((float)i / (height - 1)) - 1) * v[d];
				diff[d] = p[d] - eyep[d];
			}
			float magDiff = sqrt((diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]));
			for(int d = 0; d < 3; d++)
			{
				diff[d] = diff[d] / magDiff;
				direction[d] = diff[d];
			}

			int depth = 0;
			glm::vec3 pointColor;
			glm::vec3 initialColor = glm::vec3(0.0, 0.0, 0.0);
			pointColor = rayTrace(eyep, direction, depth, initialColor);

			for(int g = 0; g < 3; g++)
			{
				color[i][j][g] = pointColor[g];
			}


		}
		std::cout << "Row " << i << " complete\n";
	}







	BMP output;
	output.SetSize(width, height);
	output.SetBitDepth(24);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			output(x, y) -> Red = ((color[height - y - 1][x][0]) * 255);
			output(x, y) -> Green = ((color[height - y - 1][x][1]) * 255);
			output(x, y) -> Blue = ((color[height - y - 1][x][2]) * 255);
		}
	}

	output.WriteToFile(rayTracedImage.c_str());
}

glm::vec3 rayTrace(glm::vec3 eyep, glm::vec3 direction, int depth, glm::vec3 color)
{
	Node *primitive = tree->root;
	glm::vec3 returnColor(0.0, 0.0, 0.0);
	if(depth > 5)
	{
		return backRGB;
	}
	glm::vec4 normal;
	double t = 0;
	primitive = tree->findNearestPrimitiveInRay(eyep, direction, t, normal);
	glm::vec3 normal3(normal[0], normal[1], normal[2]);
	if(t == -1)
	{
		bool lighto = true;
		for(int i = 0; i < lights.size(); i++)
		{
			glm::vec3 onLightDir = lights[i].lpos - eyep;
			onLightDir = glm::normalize(onLightDir);
			if((onLightDir[0] >= (direction[0] - EPSILON)) && (onLightDir[0] <= (direction[0] + EPSILON))
				&& (onLightDir[1] >= (direction[1] - EPSILON)) && (onLightDir[1] <= (direction[1] + EPSILON))
				&& (onLightDir[2] >= (direction[2] - EPSILON)) && (onLightDir[2] <= (direction[2] + EPSILON)))
			{
				lighto = false;
				returnColor = returnColor + lights[i].lcol;
			}
		}
		if(lighto)
			returnColor = backRGB;
	}
	else
	{
		for(int i = 0; i < lights.size(); i++)
		{
			glm::vec3 pointOfIntersection;
			pointOfIntersection = eyep + direction * (float)t;
			double lightT = 0, obstacleT = 0;
			glm::vec4 lightNorm;
			Node *lightPrimitive = tree->root;
			glm::vec3 lightDirection = lights[i].lpos - pointOfIntersection;
			lightDirection = glm::normalize(lightDirection);
			lightPrimitive = tree->findNearestPrimitiveInRay(pointOfIntersection, lightDirection, obstacleT, lightNorm);
			if(obstacleT == -1)
			{
				returnColor = returnColor + KD * glm::dot(lightDirection, normal3) * (primitive->dcol * lights[i].lcol);
				glm::vec3 reflectedDirection;
				glm::vec3 Ri = pointOfIntersection - eyep;
				Ri = glm::normalize(Ri);
				float dotProduct = glm::dot(Ri, normal3);
				reflectedDirection = Ri - 2 * dotProduct * normal3;
				reflectedDirection = glm::normalize(reflectedDirection);
				glm::vec3 view = eyep - pointOfIntersection;
				view = glm::normalize(view);
				float temp = glm::dot(reflectedDirection, view);
				temp = glm::pow(temp, primitive->specular);
				if(temp > 0.0f)
				{
					returnColor = returnColor + KS * temp * lights[i].lcol;		//SPECULAR
				}
				else
				{
					temp = 0.0f;
				}
				if(primitive->reflectivity != 0)							//REFLECTION
				{
					glm::vec3 reflectedColor = rayTrace(pointOfIntersection, 
						reflectedDirection, depth + 1, color);
					reflectedColor = reflectedColor * primitive->reflectivity;
					returnColor = returnColor + reflectedColor;
				}
			}
			else
			{
				//Finding whether the obstacle object is actually in between and not beyond light    
				lightT = (lights[i].lpos[0] - pointOfIntersection[0]);
				lightT /= lightDirection[0];
				if(lightT < obstacleT)
				{
					returnColor = KD * glm::dot(lightDirection, normal3) * (primitive->dcol * lights[i].lcol);
					glm::vec3 reflectedDirection;
					glm::vec3 Ri = pointOfIntersection - eyep;
					Ri = glm::normalize(Ri);
					float dotProduct = glm::dot(Ri, normal3);
					reflectedDirection = Ri - 2 * dotProduct * normal3;
					reflectedDirection = glm::normalize(reflectedDirection);
					reflectedDirection = glm::normalize(reflectedDirection);
					glm::vec3 view = eyep - pointOfIntersection;
					view = glm::normalize(view);
					float temp = glm::dot(reflectedDirection, view);
					temp = glm::pow(temp, primitive->specular);
					if(temp > 0.0f)
					{
						returnColor = returnColor + KS * temp * lights[i].lcol;		//SPECULAR   
					}
					else
					{
						temp = 0.0f;
					}
					if(primitive->reflectivity != 0)							//REFLECTION
					{
						glm::vec3 reflectedColor = rayTrace(pointOfIntersection, 
							reflectedDirection, depth + 1, color);
						reflectedColor = reflectedColor * primitive->reflectivity;
						returnColor = returnColor + reflectedColor;
					}
				}
			}
		}
		returnColor = returnColor + KA * acol * primitive->dcol;
	}


	if(returnColor[0] > 1)
		returnColor[0] = 1.0;
	if(returnColor[1] > 1)
		returnColor[1] = 1.0;
	if(returnColor[2] > 1)
		returnColor[2] = 1.0;
	if(returnColor[0] < 0)
		returnColor[0] = 0.0;
	if(returnColor[1] < 0)
		returnColor[1] = 0.0;
	if(returnColor[2] < 0)
		returnColor[2] = 0.0;
	return returnColor;
}