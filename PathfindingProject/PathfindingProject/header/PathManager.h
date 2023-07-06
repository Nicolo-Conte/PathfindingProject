#pragma once
#include "PathManager/Node.h"
#include "glm/vec2.hpp"
#include <vector>
#include <list>

class PathManager
{

public:
	PathManager();
	~PathManager();

	void InitPathManger(int a_MapWidth, int a_MapHeight, int a_TileSize);
	bool BuildNodeGraph(bool a_Connection); 
	void InitGridValue();
	Node* GetNode() { return m_Node; }
	glm::vec2 SearchNode(glm::vec2 a_Position);
	Node* GetNode(glm::vec2 a_Position);
	void AddObstacle(glm::vec2 a_Node);

	std::vector<glm::vec2> SolveAStar(glm::vec2 a_Start, glm::vec2 a_End, std::vector<glm::vec2> &VisitedCells);
	std::vector<glm::vec2> SolveAStar(glm::vec2 a_Start, glm::vec2 a_End);
	std::vector<glm::vec2> SolveBFS(glm::vec2 a_Start, glm::vec2 a_End, std::vector<glm::vec2>& VisitedCells);
	std::vector<glm::vec2> SolveDFS(glm::vec2 a_Start, glm::vec2 a_End, std::vector<glm::vec2>& VisitedCells);
	std::vector<glm::vec2> SolveRecursive_DFS(glm::vec2 a_Start, glm::vec2 a_End);
	void ClearObstacles();
	


private:

	//Private variables
	int m_TileSize;
	Node* m_Node;
	int m_MapWidth;
	int m_MapHeight;
	std::vector<int> m_GridValues;

	Node* m_NodeStart;
	Node* m_NodeEnd;
	

};

