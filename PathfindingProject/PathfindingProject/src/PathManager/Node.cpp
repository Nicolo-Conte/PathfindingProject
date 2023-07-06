#include "PathManager/Node.h"

Node::Node() :
	m_Obstacle(false),
	m_Visited(false),
	m_GlobalGoal(0.0f),
	m_LocalGoal(0.0f),
	m_X(0),
	m_Y(0),
	m_VecNeighbours(0),
	m_Parent(nullptr)
{}

Node::~Node()
{}

/// <summary>
/// Adds a neighbour to the VecNeighbours
/// </summary>
/// <param name="a_Neighbour">A neighbour to add to the vector</param>
void Node::AddVecNeighbours(Node* a_Neighbour)
{
	m_VecNeighbours.push_back(a_Neighbour);
}