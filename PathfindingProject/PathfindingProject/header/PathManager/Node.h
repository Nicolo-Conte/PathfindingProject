#pragma once
#include <vector>
#include <memory>


class Node
{
public:
	Node();
	~Node();

	//Getters
	int GetX() const;
	int GetY() const;
	int GetDistance() const;
	bool GetObstacle() const;
	bool GetVisited() const;
	float GetGlobalGoal() const;
	float GetLocalGoal() const;
	const std::vector<Node*>& GetVecNeighbours() const;
	Node* GetParent() const;

	//Setters
	void SetX(int a_X);
	void SetY(int a_Y);
	void SetDistance(int a_Distance);
	void SetObstacle(bool a_Obstacle);
	void SetVisited(bool a_Visited);
	void SetGlobalGoal(float a_GlobalGoal);
	void SetLocalGoal(float a_LocalGoal);
	void SetVecNeighbours(std::vector<Node*> a_VecNeighbours);
	void SetParent(Node* a_Parent);

	void AddVecNeighbours(Node* a_Neighbour);

private:
	//Public variables
	bool m_Obstacle;						// Is the node an obstruction?
	bool m_Visited;							// Have we searched this node before?
	float m_GlobalGoal;						// Distance to goal so far
	float m_LocalGoal;						// Distance to goal if we took the alternative route
	int m_X;								// X Nodes position in 2D space
	int m_Y;								// Y Nodes position in 2D space
	std::vector<Node*> m_VecNeighbours;		// Connections to neighbours
	Node* m_Parent;							// Node connecting to this node that offers shortest parent
	int m_Distance;							//distance used for Dijkstra algorithm

};


//X
inline int Node::GetX() const
{
	return m_X;
}

inline void Node::SetX(int a_X)
{
	m_X = a_X;
}

//Y
inline int Node::GetY() const
{
	return m_Y;
}

inline int Node::GetDistance() const
{
	return m_Distance;
}

inline void Node::SetY(int a_Y)
{
	m_Y = a_Y;
}

inline void Node::SetDistance(int a_Distance)
{
	m_Distance = a_Distance;
}

//Obstacle
inline bool Node::GetObstacle() const
{
	return m_Obstacle;
}

inline void Node::SetObstacle(bool a_Obstacle)
{
	m_Obstacle = a_Obstacle;
}

//Visited
inline bool Node::GetVisited() const
{
	return m_Visited;
}

inline void Node::SetVisited(bool a_Visited)
{
	m_Visited = a_Visited;
}

//GlobalGoal
inline float Node::GetGlobalGoal() const
{
	return m_GlobalGoal;
}

inline void Node::SetGlobalGoal(float a_GlobalGoal)
{
	m_GlobalGoal = a_GlobalGoal;
}

//LocalGoal
inline float Node::GetLocalGoal() const
{
	return m_LocalGoal;
}

inline void Node::SetLocalGoal(float a_LocalGoal)
{
	m_LocalGoal = a_LocalGoal;
}

//VecNeighbours
inline const std::vector<Node*>& Node::GetVecNeighbours() const
{
	return m_VecNeighbours;
}

inline void Node::SetVecNeighbours(std::vector<Node*> a_VecNeighbours)
{
	m_VecNeighbours = a_VecNeighbours;
}

//Parent
inline Node* Node::GetParent() const
{
	return m_Parent;
}

inline void Node::SetParent(Node* a_Parent)
{
	m_Parent = a_Parent;
}