#include "PathManager.h"
#include <queue>
#include <math.h>
#include <stack>

PathManager::PathManager() :
	m_TileSize(0),
	m_Node(nullptr),
	m_MapWidth(0),
	m_MapHeight(0),
	m_GridValues(0),
	m_NodeStart(nullptr),
	m_NodeEnd(nullptr)
{}

PathManager::~PathManager()
{}

void PathManager::InitPathManger(int a_MapWidth, int a_MapHeight, int a_TileSize)
{
	m_MapWidth = a_MapWidth;
	m_MapHeight = a_MapHeight;
	m_TileSize = a_TileSize;
	
}

void PathManager::InitGridValue()
{
	for (int x = 0; x < m_MapWidth * m_MapHeight; x++)
	{
		m_GridValues.push_back(0);
	}
}


bool PathManager::BuildNodeGraph(bool a_Connection)
{
	int HalfTile = m_TileSize / 2;
	// Create a 2D array of m_Node - this is for convenience of rendering and construction
	// and is not required for the algorithm to work - the m_Node could be placed anywhere
	// in any space, in multiple dimensions...

	m_Node = new Node[static_cast<long long int>(m_MapWidth) * static_cast<long long int>(m_MapHeight)];

	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{

			m_Node[y * m_MapWidth + x].SetX(((x + 1) * m_TileSize) - HalfTile); // ...because we give each node its own coordinates
			m_Node[y * m_MapWidth + x].SetY(((y + 1) * m_TileSize) - HalfTile);
			if (m_GridValues[y * m_MapWidth + x] == 0)
			{
				m_Node[y * m_MapWidth + x].SetObstacle(false);
			}
			else
			{
				m_Node[y * m_MapWidth + x].SetObstacle(true);
			}
			m_Node[y * m_MapWidth + x].SetParent(nullptr);
			m_Node[y * m_MapWidth + x].SetVisited(false);
			m_Node[y * m_MapWidth + x].SetDistance(100000000);
		}

	// Create connections - in this case m_Node are on a regular grid
	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{
			if (y > 0)
				m_Node[y * m_MapWidth + x].AddVecNeighbours(&m_Node[(y - 1) * m_MapWidth + (x + 0)]);
			if (y < m_MapHeight - 1)
				m_Node[y * m_MapWidth + x].AddVecNeighbours(&m_Node[(y + 1) * m_MapWidth + (x + 0)]);
			if (x > 0)
				m_Node[y * m_MapWidth + x].AddVecNeighbours(&m_Node[(y + 0) * m_MapWidth + (x - 1)]);
			if (x < m_MapWidth - 1)
				m_Node[y * m_MapWidth + x].AddVecNeighbours(&m_Node[(y + 0) * m_MapWidth + (x + 1)]);

			// connect diagonally
			if (a_Connection)
			{
				if (y > 0 && x > 0)
					m_Node[y * m_MapWidth + x].AddVecNeighbours(&m_Node[(y - 1) * m_MapWidth + (x - 1)]);
				if (y < m_MapHeight - 1 && x>0)
					m_Node[y * m_MapWidth + x].AddVecNeighbours(&m_Node[(y + 1) * m_MapWidth + (x - 1)]);
				if (y > 0 && x < m_MapWidth - 1)
					m_Node[y * m_MapWidth + x].AddVecNeighbours(&m_Node[(y - 1) * m_MapWidth + (x + 1)]);
				if (y < m_MapHeight - 1 && x < m_MapWidth - 1)
					m_Node[y * m_MapWidth + x].AddVecNeighbours(&m_Node[(y + 1) * m_MapWidth + (x + 1)]);
			}
			/**/

		}
	return true;
}



std::vector<glm::vec2> PathManager::SolveAStar(glm::vec2 a_Start, glm::vec2 a_End, std::vector<glm::vec2> &VisitedCells)
{

	int HalfTile = m_TileSize / 2;
	a_Start.x *= m_TileSize;
	a_Start.y *= m_TileSize;
	a_End.x *= m_TileSize;
	a_End.y *= m_TileSize;

	int counter = 0;
	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{
			if (((a_Start.x >= m_Node[y * m_MapWidth + x].GetX() - HalfTile) && (a_Start.x < m_Node[y * m_MapWidth + x].GetX() + HalfTile)) &&
				((a_Start.y >= m_Node[y * m_MapWidth + x].GetY() - HalfTile) && (a_Start.y < m_Node[y * m_MapWidth + x].GetY() + HalfTile)))
			{
				m_NodeStart = &m_Node[y * m_MapWidth + x];
				counter = 0;
				continue;
			}
		}

	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{
			if (((a_End.x >= m_Node[y * m_MapWidth + x].GetX() - HalfTile) && (a_End.x < m_Node[y * m_MapWidth + x].GetX() + HalfTile)) &&
				((a_End.y >= m_Node[y * m_MapWidth + x].GetY() - HalfTile) && (a_End.y < m_Node[y * m_MapWidth + x].GetY() + HalfTile)))
			{
				m_NodeEnd = &m_Node[y * m_MapWidth + x];
				continue;
			}
		}

	// Reset Navigation Graph - default all node states
	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{
			m_Node[y * m_MapWidth + x].SetVisited(false);
			m_Node[y * m_MapWidth + x].SetGlobalGoal(INFINITY);
			m_Node[y * m_MapWidth + x].SetLocalGoal(INFINITY);
			m_Node[y * m_MapWidth + x].SetParent(nullptr);	// No m_parents
		}

	auto distance = [](Node* a, Node* b) // For convenience
	{
		return sqrt((a->GetX() - b->GetX()) * (a->GetX() - b->GetX()) + (a->GetY() - b->GetY()) * (a->GetY() - b->GetY()));
	};

	auto heuristic = [distance](Node* a, Node* b) // So we can experiment with heuristic
	{
		return distance(a, b);
	};

	// Setup starting conditions
	Node* nodeCurrent = m_NodeStart;
	m_NodeStart->SetLocalGoal(0.0f);
	m_NodeStart->SetGlobalGoal((float)heuristic(m_NodeStart, m_NodeEnd));

	// Add start node to not tested list - this will ensure it gets tested.
	// As the algorithm progresses, newly discovered m_Node get added to this
	// list, and will themselves be tested later
	std::list<Node*> listNotTestedm_Node;
	listNotTestedm_Node.push_back(m_NodeStart);

	// if the not tested list contains m_Node, there may be better paths
	// which have not yet been explored. However, we will also stop 
	// searching when we reach the target - there may well be better
	// paths but this one will do - it wont be the longest.
	while (!listNotTestedm_Node.empty() && nodeCurrent != m_NodeEnd)// Find absolutely shortest path // && nodeCurrent != m_NodeEnd)
	{
		// Sort Untested m_Node by global goal, so lowest is first
		listNotTestedm_Node.sort([](const Node* lhs, const Node* rhs) { return lhs->GetGlobalGoal() < rhs->GetGlobalGoal(); });

		// Front of listNotTestedm_Node is potentially the lowest distance node. Our
		// list may also contain m_Node that have been visited, so ditch these...
		while (!listNotTestedm_Node.empty() && listNotTestedm_Node.front()->GetVisited())
			listNotTestedm_Node.pop_front();

		// ...or abort because there are no valid m_Node left to test
		if (listNotTestedm_Node.empty())
			break;

		nodeCurrent = listNotTestedm_Node.front();
		nodeCurrent->SetVisited(true); // We only explore a node once
		VisitedCells.push_back(glm::vec2(nodeCurrent->GetX() / m_TileSize, nodeCurrent->GetY() / m_TileSize));


		// Check each of this node's neighbours...
		for (auto nodeNeighbour : nodeCurrent->GetVecNeighbours())
		{
			// ... and only if the neighbour is not visited and is 
			// not an obstacle, add it to NotTested List
			if (!nodeNeighbour->GetVisited() && nodeNeighbour->GetObstacle() == 0)
				listNotTestedm_Node.push_back(nodeNeighbour);

			// Calculate the neighbours potential lowest m_parent distance
			float fPossiblyLowerGoal = nodeCurrent->GetLocalGoal() + (float)distance(nodeCurrent, nodeNeighbour);

			// If choosing to path through this node is a lower distance than what 
			// the neighbour currently has set, update the neighbour to use this node
			// as the path source, and set its distance scores as necessary
			if (fPossiblyLowerGoal < nodeNeighbour->GetLocalGoal())
			{
				nodeNeighbour->SetParent(nodeCurrent);
				nodeNeighbour->SetLocalGoal(fPossiblyLowerGoal);

				// The best path length to the neighbour being tested has changed, so
				// update the neighbour's score. The heuristic is used to globally bias
				// the path algorithm, so it knows if its getting better or worse. At some
				// point the algo will realise this path is worse and abandon it, and then go
				// and search along the next best path.
				nodeNeighbour->SetGlobalGoal(nodeNeighbour->GetLocalGoal() + (float)heuristic(nodeNeighbour, m_NodeEnd));
			}
		}
	}
	std::vector<glm::vec2> QueuePathTarget;
	if (m_NodeEnd != nullptr && m_NodeEnd->GetParent() != nullptr)
	{

		Node* p = m_NodeEnd;
		while (p->GetParent() != nullptr)
		{
			QueuePathTarget.push_back(glm::vec2(p->GetParent()->GetX(), p->GetParent()->GetY()));

			// Set next node to this node's parent
			p = p->GetParent();
		}
		QueuePathTarget.push_back(glm::vec2(m_NodeEnd->GetX(), m_NodeEnd->GetY()));
	}
	if (QueuePathTarget.size() >= 1)
	{
	reverse(QueuePathTarget.begin(), QueuePathTarget.end());
	reverse(QueuePathTarget.begin()+1, QueuePathTarget.end());
	reverse(QueuePathTarget.begin(), QueuePathTarget.end());
	}

	return QueuePathTarget;
}




std::vector<glm::vec2> PathManager::SolveAStar(glm::vec2 a_Start, glm::vec2 a_End)
{
	int HalfTile = m_TileSize / 2;


	int counter = 0;
	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{
			if (((a_Start.x >= m_Node[y * m_MapWidth + x].GetX() - HalfTile) && (a_Start.x < m_Node[y * m_MapWidth + x].GetX() + HalfTile)) &&
				((a_Start.y >= m_Node[y * m_MapWidth + x].GetY() - HalfTile) && (a_Start.y < m_Node[y * m_MapWidth + x].GetY() + HalfTile)))
			{
				m_NodeStart = &m_Node[y * m_MapWidth + x];
				counter = 0;
				continue;
			}
		}

	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{
			if (((a_End.x >= m_Node[y * m_MapWidth + x].GetX() - HalfTile) && (a_End.x < m_Node[y * m_MapWidth + x].GetX() + HalfTile)) &&
				((a_End.y >= m_Node[y * m_MapWidth + x].GetY() - HalfTile) && (a_End.y < m_Node[y * m_MapWidth + x].GetY() + HalfTile)))
			{
				m_NodeEnd = &m_Node[y * m_MapWidth + x];
				continue;
			}
		}

	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{
			m_Node[y * m_MapWidth + x].SetVisited(false);
			m_Node[y * m_MapWidth + x].SetGlobalGoal(INFINITY);
			m_Node[y * m_MapWidth + x].SetLocalGoal(INFINITY);
			m_Node[y * m_MapWidth + x].SetParent(nullptr);	// No m_parents
		}

	auto distance = [](Node* a, Node* b) // For convenience
	{
		return sqrt((a->GetX() - b->GetX()) * (a->GetX() - b->GetX()) + (a->GetY() - b->GetY()) * (a->GetY() - b->GetY()));
	};

	auto heuristic = [distance](Node* a, Node* b) // So we can experiment with heuristic
	{
		return distance(a, b);
	};

	Node* nodeCurrent = m_NodeStart;
	m_NodeStart->SetLocalGoal(0.0f);
	m_NodeStart->SetGlobalGoal((float)heuristic(m_NodeStart, m_NodeEnd));

	std::list<Node*> listNotTestedm_Node;
	listNotTestedm_Node.push_back(m_NodeStart);

	while (!listNotTestedm_Node.empty() && nodeCurrent != m_NodeEnd)// Find absolutely shortest path // && nodeCurrent != m_NodeEnd)
	{
		// Sort Untested m_Node by global goal, so lowest is first
		listNotTestedm_Node.sort([](const Node* lhs, const Node* rhs) { return lhs->GetGlobalGoal() < rhs->GetGlobalGoal(); });

		while (!listNotTestedm_Node.empty() && listNotTestedm_Node.front()->GetVisited())
			listNotTestedm_Node.pop_front();

		if (listNotTestedm_Node.empty())
			break;

		nodeCurrent = listNotTestedm_Node.front();
		nodeCurrent->SetVisited(true); // We only explore a node once

		for (auto nodeNeighbour : nodeCurrent->GetVecNeighbours())
		{

			if (!nodeNeighbour->GetVisited() && nodeNeighbour->GetObstacle() == 0)
				listNotTestedm_Node.push_back(nodeNeighbour);

			float fPossiblyLowerGoal = nodeCurrent->GetLocalGoal() + (float)distance(nodeCurrent, nodeNeighbour);


			if (fPossiblyLowerGoal < nodeNeighbour->GetLocalGoal())
			{
				nodeNeighbour->SetParent(nodeCurrent);
				nodeNeighbour->SetLocalGoal(fPossiblyLowerGoal);

				nodeNeighbour->SetGlobalGoal(nodeNeighbour->GetLocalGoal() + (float)heuristic(nodeNeighbour, m_NodeEnd));
			}
		}
	}
	std::vector<glm::vec2> QueuePathTarget;
	if (m_NodeEnd != nullptr && m_NodeEnd->GetParent() != nullptr)
	{

		Node* p = m_NodeEnd;
		while (p->GetParent() != nullptr)
		{
			QueuePathTarget.push_back(glm::vec2(p->GetParent()->GetX(), p->GetParent()->GetY()));

			p = p->GetParent();
		}
		QueuePathTarget.push_back(glm::vec2(m_NodeEnd->GetX(), m_NodeEnd->GetY()));
	}
	if (QueuePathTarget.size() >= 1)
	{
		reverse(QueuePathTarget.begin(), QueuePathTarget.end());
		reverse(QueuePathTarget.begin() + 1, QueuePathTarget.end());
		reverse(QueuePathTarget.begin(), QueuePathTarget.end());
	}
	return QueuePathTarget;
}





std::vector<glm::vec2> PathManager::SolveBFS(glm::vec2 a_Start, glm::vec2 a_End, std::vector<glm::vec2>& VisitedCells)
{
	int HalfTile = m_TileSize / 2;
	a_Start.x *= m_TileSize;
	a_Start.y *= m_TileSize;
	a_End.x *= m_TileSize;
	a_End.y *= m_TileSize;

	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{
			if (((a_Start.x >= m_Node[y * m_MapWidth + x].GetX() - HalfTile) && (a_Start.x < m_Node[y * m_MapWidth + x].GetX() + HalfTile)) &&
				((a_Start.y >= m_Node[y * m_MapWidth + x].GetY() - HalfTile) && (a_Start.y < m_Node[y * m_MapWidth + x].GetY() + HalfTile)))
			{
				m_NodeStart = &m_Node[y * m_MapWidth + x];
				m_Node[y * m_MapWidth + x].SetVisited(true);
				continue;
			}
		}

	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{
			if (((a_End.x >= m_Node[y * m_MapWidth + x].GetX() - HalfTile) && (a_End.x < m_Node[y * m_MapWidth + x].GetX() + HalfTile)) &&
				((a_End.y >= m_Node[y * m_MapWidth + x].GetY() - HalfTile) && (a_End.y < m_Node[y * m_MapWidth + x].GetY() + HalfTile)))
			{
				m_NodeEnd = &m_Node[y * m_MapWidth + x];
				continue;
			}
		}

	std::vector<Node*> Queue;
	Queue.push_back(m_NodeStart);

	while (!Queue.empty())
	{
		Node* curNode = Queue.front();
		Queue.erase(Queue.begin());

		if(curNode == m_NodeEnd)
			break;

		// Handle neighbors
		std::vector<Node*> neighbors;
		neighbors = curNode->GetVecNeighbours();

		for (auto i = 0; i < neighbors.size(); ++i)
		{
			if (neighbors[i]->GetObstacle() == 0 && neighbors[i]->GetVisited() == false)
			{
			neighbors[i]->SetVisited(true);
			VisitedCells.push_back(glm::vec2(neighbors[i]->GetX() / m_TileSize, neighbors[i]->GetY() / m_TileSize));
			neighbors[i]->SetParent(curNode);
			Queue.push_back(neighbors[i]);
			}
			
		}
	}

		Queue.clear();
		Node* p = m_NodeEnd;
		while (p->GetParent() != nullptr)
		{
			Queue.push_back((p));

			// Set next node to this node's parent
			p = p->GetParent();
		}
	
	
	std::vector<glm::vec2> QueuePathTarget;

	
	for (int x = 0; x < Queue.size(); x++)
	{
		QueuePathTarget.push_back(glm::vec2(Queue[x]->GetX(), Queue[x]->GetY()));
	}
	QueuePathTarget.push_back(glm::vec2(m_NodeStart->GetX(), m_NodeStart->GetY()));
	
	return QueuePathTarget;

}

std::vector<glm::vec2> PathManager::SolveDFS(glm::vec2 a_Start, glm::vec2 a_End, std::vector<glm::vec2>& VisitedCells)
{

	std::stack<Node*> Queue;


	int HalfTile = m_TileSize / 2;
	a_Start.x *= m_TileSize;
	a_Start.y *= m_TileSize;
	a_End.x *= m_TileSize;
	a_End.y *= m_TileSize;

	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{
			if (((a_Start.x >= m_Node[y * m_MapWidth + x].GetX() - HalfTile) && (a_Start.x < m_Node[y * m_MapWidth + x].GetX() + HalfTile)) &&
				((a_Start.y >= m_Node[y * m_MapWidth + x].GetY() - HalfTile) && (a_Start.y < m_Node[y * m_MapWidth + x].GetY() + HalfTile)))
			{
				m_NodeStart = &m_Node[y * m_MapWidth + x];
				m_NodeStart->SetVisited(true);
				continue;
			}
		}

	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{
			if (((a_End.x >= m_Node[y * m_MapWidth + x].GetX() - HalfTile) && (a_End.x < m_Node[y * m_MapWidth + x].GetX() + HalfTile)) &&
				((a_End.y >= m_Node[y * m_MapWidth + x].GetY() - HalfTile) && (a_End.y < m_Node[y * m_MapWidth + x].GetY() + HalfTile)))
			{
				m_NodeEnd = &m_Node[y * m_MapWidth + x];
				continue;
			}
		}

	Queue.push(m_NodeStart);


	while (!Queue.empty())
	{
		Node* curNode = Queue.top();
		
		curNode->SetVisited(true);

		Queue.pop();
		
		if (curNode == m_NodeEnd)
					break;
		
			std::vector<Node*> neighbors;
			neighbors = curNode->GetVecNeighbours();
			for (int i = 0; i < neighbors.size(); ++i)
			{
				
				if (neighbors[i]->GetObstacle() == 0 && neighbors[i]->GetVisited() == false)
				{
					neighbors[i]->SetVisited(true);
					VisitedCells.push_back(glm::vec2(neighbors[i]->GetX() / m_TileSize, neighbors[i]->GetY() / m_TileSize));
					neighbors[i]->SetParent(curNode);
					Queue.push(neighbors[i]);
					if (neighbors[i] == m_NodeEnd)
					{
						curNode = neighbors[i];
						break;
					}
						
				}
			}
	}

	//Queue.empty();
	std::vector<Node*> Queue1;
	Node* p = m_NodeEnd;
	while (p->GetParent() != nullptr)
	{
		Queue1.push_back(p);

		// Set next node to this node's parent
		p = p->GetParent();
	}


	std::vector<glm::vec2> QueuePathTarget;

	
	for (int x = 0; x < Queue1.size(); x++)
	{
		QueuePathTarget.push_back(glm::vec2(Queue1[x]->GetX(), Queue1[x]->GetY()));
		//Queue1.pop_back();

	}
	QueuePathTarget.push_back(glm::vec2(m_NodeStart->GetX(), m_NodeStart->GetY()));

	return QueuePathTarget;


}

void PathManager::ClearObstacles()
{

	int HalfTile = m_TileSize / 2;

	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{
				if (m_Node[y * m_MapWidth + x].GetObstacle() == true)
				{
					m_Node[y * m_MapWidth + x].SetObstacle(false);
					m_GridValues[y * m_MapWidth + x] = 0;

				}
		}
}






glm::vec2 PathManager::SearchNode(glm::vec2 a_Position)
{
	int HalfTile = m_TileSize / 2;
	
	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{
			if (((a_Position.x >= m_Node[y * m_MapWidth + x].GetX() - HalfTile) && (a_Position.x < m_Node[y * m_MapWidth + x].GetX() + HalfTile)) &&
				((a_Position.y >= m_Node[y * m_MapWidth + x].GetY() - HalfTile) && (a_Position.y < m_Node[y * m_MapWidth + x].GetY() + HalfTile)))
			{
				return glm::vec2(m_Node[y * m_MapWidth + x].GetX(), m_Node[y * m_MapWidth + x].GetY());

			}
		}
	return glm::vec2(-1, -1);
}

Node* PathManager::GetNode(glm::vec2 a_Position)
{
	int HalfTile = m_TileSize / 2;

	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{
			if (((a_Position.x >= m_Node[y * m_MapWidth + x].GetX() - HalfTile) && (a_Position.x < m_Node[y * m_MapWidth + x].GetX() + HalfTile)) &&
				((a_Position.y >= m_Node[y * m_MapWidth + x].GetY() - HalfTile) && (a_Position.y < m_Node[y * m_MapWidth + x].GetY() + HalfTile)))
			{
				return &m_Node[y * m_MapWidth + x];

			}
		}
	return nullptr;
}

void PathManager::AddObstacle(glm::vec2 a_Node)
{
	int HalfTile = m_TileSize / 2;
	a_Node.x *= m_TileSize;
	a_Node.y *= m_TileSize;

	for (int x = 0; x < m_MapWidth; x++)
		for (int y = 0; y < m_MapHeight; y++)
		{
			if (((a_Node.x >= m_Node[y * m_MapWidth + x].GetX() - HalfTile) && (a_Node.x < m_Node[y * m_MapWidth + x].GetX() + HalfTile)) &&
				((a_Node.y >= m_Node[y * m_MapWidth + x].GetY() - HalfTile) && (a_Node.y < m_Node[y * m_MapWidth + x].GetY() + HalfTile)))
			{
				if(&m_Node[y * m_MapWidth + x] != m_NodeStart && &m_Node[y * m_MapWidth + x] != m_NodeEnd)
				{ 
					m_Node[y * m_MapWidth + x].SetObstacle(true);
					m_GridValues[y * m_MapWidth + x] = 1;
					//printf("obstacle added \n");
					continue;
				}
					

				
			}
		}
}




