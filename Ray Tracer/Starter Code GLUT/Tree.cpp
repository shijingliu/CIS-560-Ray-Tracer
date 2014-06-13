#include "Tree.h"

Tree::Tree(Node *root)
{
	this->root = root;
}

Tree::~Tree(void)
{   
}

void Tree::add(Node* newbie)
{
	bool temp = preOrderTraversal(root, newbie->xIndex, newbie->zIndex);
	if(temp == false)
	{	
		root->children.push_back(newbie);
		newbie->parent = root;
	}
	else
	{
		treeTemp->children.push_back(newbie);
		newbie->parent = treeTemp;
	}
}

bool Tree::preOrderTraversal(Node *roots, int xIndex, int zIndex)
{
	bool b = false;  
	if(roots == 0)
		return false;
	for(int i = 0; i < (roots->children.size()); i++)
	{
		b = preOrderTraversal(roots->children[i], xIndex, zIndex);
		if(b)
			break;
	}
	if(!b)
	{
		if((roots->xIndex == xIndex) && (roots->zIndex == zIndex))
		{
			treeTemp = roots;
			b = true;
		}
	}
	return b;
}

void Tree::printTree()
{
	std::cout << "Pre Order Tree\n";
	printTree(root);
	std::cout << "Pre Order Tree\n";
}

void Tree::printTree(Node *roots)
{
	if(roots == 0)
		return;
	std::cout << (roots->fur.c_str()) << std::endl;
	for(int i = 0; i < roots->children.size(); i++)
	{
		printTree(roots->children[i]);
	}
}

void Tree::deleteNode(Node * n)
{
	Node* par = n->parent;
	int i = 0;
	std::vector <Node*> v;
	for(int i = 0; i < par->children.size(); i++)
	{
		if(par->children[i] != n)
		{
			v.push_back(par->children[i]);
		}
		else
		{
			delete par->children[i];
		}
	}
	par->children.clear();
	for(int i = 0; i < v.size(); i++)
	{
		par->children.push_back(v[i]);
	}
}

void Tree::drawSceneGraph()
{
	glm::mat4 modelView = glm::mat4(1.0);
	drawSceneGraph(root, 0.0f, modelView);
}

void Tree::drawSceneGraph(Node *roots, float yIndex, glm::mat4 modelView)
{
	if(roots == 0)
		return;
	float color[3];
	color[0] = roots->r;
	color[1] = roots->g;
	color[2] = roots->b;
	roots->furnitureType->drawFurniture(modelView, roots->xIndex, yIndex, roots->zIndex,
			roots->rotation, roots->xScale, roots->yScale, roots->zScale, color);

	yIndex += roots->furnitureType->giveHeight();
	
	glm::vec3 a;
	a[0] = roots->xIndex;
	a[1] = 0;
	a[2] = roots->zIndex;
	modelView = glm::translate(modelView, a);
	modelView = glm::rotate(modelView, roots->rotation, glm::vec3(0, 1, 0));
	a[0] = roots->xScale;
	a[1] = roots->yScale;
	a[2] = roots->zScale;
	modelView = glm::scale(modelView, a);

	for(int i = 0; i < roots->children.size(); i++)
	{
		float color[3];
		color[0] = roots->children[i]->r;
		color[1] = roots->children[i]->g;
		color[2] = roots->children[i]->b;
		drawSceneGraph(roots->children[i], yIndex, modelView);
	}
}

void Tree::adjustValues()
{
	for(int i = 0; i < (root->children.size()); i++)
		adjustValues(root->children[i]); 
}

void Tree::adjustValues(Node *roots)
{
	if(roots == 0)
		return;
	for(int i = 0; i < roots->children.size(); i++)
	{
		roots->children[i]->xIndex = 0;
		roots->children[i]->zIndex = 0;
		adjustValues(roots->children[i]);
	}
}

//void Tree::findNearestPrimitiveInRay(glm::vec3 eye, glm::vec3 direction, double &t, glm::vec4 &normal, Node *primitive)   
Node* Tree::findNearestPrimitiveInRay(glm::vec3 eye, glm::vec3 direction, double &t, glm::vec4 &normal)
{
	Node * primitive = root;
	glm::mat4 modelView = glm::mat4(1.0);
	primitive = findNearestPrimitiveInRay(root, 0.0f, modelView, eye, direction, t, normal);  
	return primitive;
}

//void Tree::findNearestPrimitiveInRay(Node *roots, float yIndex, glm::mat4 modelView,
//	glm::vec3 eye, glm::vec3 direction, double &t, glm::vec4 &normal, Node *primitive)  
Node* Tree::findNearestPrimitiveInRay(Node *roots, float yIndex, glm::mat4 modelView,
	glm::vec3 eye, glm::vec3 direction, double &t, glm::vec4 &normal)
{
	Node *primitive = root;
	double currentT = 0;
	glm::vec4 currentNorm;
	if(roots == 0)
		return 0;

	roots->furnitureType->intersect(eye, direction, modelView, roots->xIndex, yIndex, roots->zIndex,
			roots->rotation, roots->xScale, roots->yScale, roots->zScale, currentT, currentNorm);

	yIndex += roots->furnitureType->giveHeight();
	
	glm::vec3 a;
	a[0] = roots->xIndex;
	a[1] = 0;
	a[2] = roots->zIndex;
	modelView = glm::translate(modelView, a);
	modelView = glm::rotate(modelView, roots->rotation, glm::vec3(0, 1, 0));
	a[0] = roots->xScale;
	a[1] = roots->yScale;
	a[2] = roots->zScale;
	modelView = glm::scale(modelView, a);

	double tt, leastTT = 1e26;
	glm::vec4 tempNorm, norm;
	Node *child = root, *intersectingPrimitive = root;

	for(int i = 0; i < roots->children.size(); i++)
	{
		child = findNearestPrimitiveInRay(roots->children[i], yIndex, modelView,
			eye, direction, tt, tempNorm);
		if((tt < leastTT) && (tt > 0))
		{
			leastTT = tt;
			norm = tempNorm;
			intersectingPrimitive = child;
		}
	}
	
	if(leastTT == 1e26)
		leastTT = -1;

	if((currentT < 0) && (leastTT < 0))
	{
		t = -1;
	}

	else if((currentT < 0))
	{
		t = leastTT;
		normal = norm;
		primitive = intersectingPrimitive;
	}

	else if((leastTT < 0))
	{
		t = currentT;
		normal = currentNorm;
		primitive = roots;
	}

	else
	{
		if((currentT < leastTT))
		{
			t = currentT;
			normal = currentNorm;
			primitive = roots;
		}
		else
		{
			t = leastTT;
			normal = norm;
			primitive = intersectingPrimitive;
		}
	}
	return primitive;
}