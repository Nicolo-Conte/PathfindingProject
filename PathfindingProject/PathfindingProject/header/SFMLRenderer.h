#pragma once
#include "../ImGui/Headers/imgui.h"
#include "../ImGui/Headers/imgui-SFML.h"

#include "glm/glm.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "PathManager.h"

#include "AgentManager.h"

struct GridSpace
{
	sf::Vector2f m_Position;
	int m_GridSpaceHeight;
	int m_GridSpaceWidth;
	float m_Color[3];
	int m_TileSize;
	std::vector<glm::vec2> m_Obstacles;
	std::vector<glm::vec2> m_VisitedCells;
	GridSpace(){}
	void InitGridSpace(sf::Vector2f a_Position, int a_GridSpaceHeight, int a_GridSpaceWidth, int a_TileSize)
	{
		m_Position = a_Position;
		m_GridSpaceHeight = a_GridSpaceHeight;
		m_GridSpaceWidth = a_GridSpaceWidth;
		m_TileSize = a_TileSize;
	}
	void AddObstacleToGrid(glm::vec2 a_Obstacle)
	{
		m_Obstacles.push_back(a_Obstacle);
	}
	void AddVisitedCell(glm::vec2 a_Cell)
	{
		m_VisitedCells.push_back(a_Cell);
	}
	void ClearObstacle()
	{
		m_Obstacles.clear();
	}
};

enum AlgorithmChoose
{
	AStar = 0,
	BFS,
	DFS
};

class SFMLRenderer
{
public:

    SFMLRenderer();
	~SFMLRenderer();

	void Initialize(glm::vec2 a_ScreenSize, PathManager* a_Pathmanager, AgentManager* a_AgentManger);
	void Update();
	std::vector<glm::vec2> ApplyAgorithm();
	void ClearWindow();
	void PollEventUI(sf::Event a_Event);
	void Render();
	void UpdateUI();
	void DrawGrid();
	void DrawSprite();
	sf::RenderWindow& GetWindow();
	void EditGridSpace();
	void SpawnRandomObstacles();
	GridSpace m_GridSpace;
	void AddSprite(sf::Sprite);


private:

	
	sf::RectangleShape shape;
	sf::RenderWindow m_Window;
	sf::Event m_Event;
	sf::Clock m_SFMLDeltaTime;
	bool m_OpenEditGrid = false;
	bool m_DrawPath = false;
	bool m_AddObstacle = false;
	bool m_EditAgents = false;
	bool m_AddAgents = false;
	int m_MaxAgentsInPlay = 1;
	PathManager* m_Pathmanager;
	std::vector<glm::vec2> m_LastPath;
	std::vector<glm::vec2> m_VisitedCells;
	void DrawPath();
	void DrawVisitedCells();
	void ClearObstacles();
	glm::vec2 m_StartNode;
	glm::vec2 m_EndNode;
	AlgorithmChoose m_ActualAlgorithm = AStar;
	bool m_EnableConnections = false;
	std::vector<sf::Sprite> m_SpriteToDraw;
	AgentManager* m_AgentManger;
};

inline sf::RenderWindow& SFMLRenderer::GetWindow()
{
	return m_Window;
}

