#pragma once

#include <iostream>
#include "Node.h"

class Tree
{
public:
	Node *root;
	Node *treeTemp;

	Tree(Node *);
	~Tree(void);

	void add(Node *);
	void deleteNode(Node *);
	bool preOrderTraversal(Node*, int, int);
	void printTree(Node *);
	void printTree();
	void drawSceneGraph();
	void drawSceneGraph(Node*, float, glm::mat4);
	void Tree::adjustValues();
	void Tree::adjustValues(Node*);
	//void Tree::findNearestPrimitiveInRay(glm::vec3 eye, glm::vec3 direction, double &t, glm::vec4 &normal, Node *primitive);
	/*void Tree::findNearestPrimitiveInRay(Node *roots, float yIndex, glm::mat4 modelView,
			glm::vec3 eye, glm::vec3 direction, double &t, glm::vec4 &normal, Node *primitive);*/
	Node* Tree::findNearestPrimitiveInRay(glm::vec3 eye, glm::vec3 direction, double &t, glm::vec4 &normal);
	Node* Tree::findNearestPrimitiveInRay(Node *roots, float yIndex, glm::mat4 modelView,
			glm::vec3 eye, glm::vec3 direction, double &t, glm::vec4 &normal);
};