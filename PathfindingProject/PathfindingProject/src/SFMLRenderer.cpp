#include "SFMLRenderer.h"
#include <chrono>
#include <thread>


std::vector<glm::vec2> PathToDraw;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;
std::vector<int> values(10000);

int row = 0;
int column = 0;
float Targetrow = 0;
float Targetcolumn = 0;

SFMLRenderer::SFMLRenderer()
{
}

SFMLRenderer::~SFMLRenderer()
{
	ImGui::SFML::Shutdown();
}

void SFMLRenderer::Initialize(glm::vec2 a_ScreenSize, PathManager* a_Pathmanager, AgentManager* a_AgentManger)
{

	m_Pathmanager = a_Pathmanager;
	m_AgentManger = a_AgentManger;
	m_Window.create(sf::VideoMode(a_ScreenSize.x, a_ScreenSize.y), "Pathfinding tool");
	m_Window.setFramerateLimit(240);
	ImGui::SFML::Init(m_Window);
	m_GridSpace.InitGridSpace(sf::Vector2f(360.f, 10.f), 16, 16, 64);
	m_GridSpace.m_Color[0] = (float)255 / 255;
	m_GridSpace.m_Color[1] = (float)0 / 255;
	m_GridSpace.m_Color[2] = (float)0 / 255;
	shape.setFillColor(sf::Color(255, 0, 0));
	m_StartNode = glm::vec2(1,1);
	m_EndNode= glm::vec2(1, 1);
	
}

void SFMLRenderer::Update()
{
	//Update UI
	UpdateUI();

	

	//Render
	Render();
}

std::vector<glm::vec2> SFMLRenderer::ApplyAgorithm()
{

	switch (m_ActualAlgorithm)
	{
	case AStar:
		return m_Pathmanager->SolveAStar(m_StartNode, m_EndNode, m_GridSpace.m_VisitedCells);
		break;
	case BFS:
		return m_Pathmanager->SolveBFS(m_StartNode, m_EndNode, m_GridSpace.m_VisitedCells);
		break;
	case DFS:
		return m_Pathmanager->SolveDFS(m_StartNode, m_EndNode, m_GridSpace.m_VisitedCells);
		break;
	default:
		break;
	}
	
}
//std::chrono::nanoseconds asd = 1000000ns;

void SFMLRenderer::ClearWindow()
{
	m_Window.clear(sf::Color(0, 0, 0));
}

void SFMLRenderer::Render()
{
	//draw elements here
	// 
	
	DrawGrid();
	if (m_DrawPath)
	{
		DrawVisitedCells();
		if (m_GridSpace.m_VisitedCells.size() == 0)
		{
			DrawPath();
			if (m_LastPath.size() >= 1)sleep_until(system_clock::now() + 0.01s);
		}
		else
		{
			sleep_until(system_clock::now() + 0.01s);
		}
	}

	//Draw Sprites
	DrawSprite();
	ImGui::SFML::Render(m_Window);
	m_Window.display();
}

glm::vec2 obstaclePosition;
std::string PathfindingUsed;
void SFMLRenderer::UpdateUI()
{
	ClearWindow();
	
	ImGui::SFML::Update(m_Window, m_SFMLDeltaTime.restart());
	
	//execute UI's commands here


	ImGui::SetNextWindowPos(sf::Vector2f(0.f, 0.f));
	ImGui::SetNextWindowSize(sf::Vector2f(350.f, 1080.f));
	ImGui::Begin("Settings");


	ImGui::Text("Select the pathfinding algorithms");

	if (ImGui::Button("A*",sf::Vector2f(100.f,25.f)))
	{
		m_ActualAlgorithm = AStar;
		PathfindingUsed = "A*";
	}

	if (ImGui::Button("BFS", sf::Vector2f(100.f, 25.f)))
	{
		m_ActualAlgorithm = BFS;
		PathfindingUsed = "BFS";
	}

	if (ImGui::Button("DFS", sf::Vector2f(100.f, 25.f)))
		
	{
		m_ActualAlgorithm = DFS;
		PathfindingUsed = "DFS";
	}


	if ((ImGui::Button("Edit Grid", sf::Vector2f(100.f, 25.f))))
	{
		m_OpenEditGrid  = true;

	}

	/// <summary>
	/// Edit grid values
	/// </summary>
	if(m_OpenEditGrid)
	{
		ImGui::ListBoxHeader("", sf::Vector2f(375.f, 300.f));

			ImGui::InputInt("rows", &m_GridSpace.m_GridSpaceWidth);
			ImGui::InputInt("columns", &m_GridSpace.m_GridSpaceHeight);
			ImGui::InputInt("tile size", &m_GridSpace.m_TileSize);
			ImGui::Text("Start point");
			ImGui::InputFloat("column Start", &m_StartNode.x);
			ImGui::InputFloat("row Start", &m_StartNode.y);
			ImGui::Text("End point");
			ImGui::InputFloat("column End", &m_EndNode.x);
			ImGui::InputFloat("row End", &m_EndNode.y);
			EditGridSpace();
			if (ImGui::Button("Done"))
			{
				m_OpenEditGrid = false;
				m_Pathmanager->InitPathManger(m_GridSpace.m_GridSpaceWidth, m_GridSpace.m_GridSpaceHeight, m_GridSpace.m_TileSize);
				m_Pathmanager->BuildNodeGraph(m_EnableConnections);
				m_VisitedCells.clear();
				// Get starting timepoint
				auto start = std::chrono::high_resolution_clock::now();
				m_LastPath = ApplyAgorithm();
				// Get ending timepoint
				auto stop = std::chrono::high_resolution_clock::now();

				// Get duration. Substart timepoints to
				// get duration. To cast it to proper unit
				// use duration cast method
				auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
				printf("duration of execution time of algorithm  %s : ", PathfindingUsed.c_str());
				printf("%d milliseconds\n", duration.count());
				PathToDraw.clear();
			}
			ImGui::Checkbox("Enable diagonally connections ", &m_EnableConnections);
			ImGui::ListBoxFooter();
			
	}

	if ((ImGui::Button("Draw Path", sf::Vector2f(200.f, 25.f))))
	{
		m_DrawPath = true;
	}
	if ((ImGui::Button("Clear path on screen", sf::Vector2f(200.f, 25.f))))
	{
		m_DrawPath = false;
	}
	
	if ((ImGui::Button("Edit Obstacle", sf::Vector2f(100.f, 25.f))))
	{
		m_AddObstacle = true;

	}
	if (m_AddObstacle)
	{
		ImGui::ListBoxHeader("", sf::Vector2f(375.f, 300.f));

		
		ImGui::Text("Obstacle position");
		ImGui::InputFloat("column Obstacle", &obstaclePosition.x);
		ImGui::InputFloat("row Obstacle", &obstaclePosition.y);

		if (ImGui::Button("AddObstacle"))
		{
			m_VisitedCells.clear();
			m_Pathmanager->AddObstacle(obstaclePosition);
			m_GridSpace.AddObstacleToGrid(obstaclePosition);
			m_Pathmanager->InitPathManger(m_GridSpace.m_GridSpaceWidth, m_GridSpace.m_GridSpaceHeight, m_GridSpace.m_TileSize);
			m_Pathmanager->BuildNodeGraph(m_EnableConnections);
			m_LastPath = ApplyAgorithm();
			PathToDraw.clear();

		}
		
		if (ImGui::Button("Clear all Obstacles"))
		{
			ClearObstacles();
			m_Pathmanager->InitPathManger(m_GridSpace.m_GridSpaceWidth, m_GridSpace.m_GridSpaceHeight, m_GridSpace.m_TileSize);
			m_Pathmanager->BuildNodeGraph(m_EnableConnections);
		}
		if (ImGui::Button("Done"))
		{
			m_AddObstacle = false;
		}
		ImGui::ListBoxFooter();
		
	}



	if ((ImGui::Button("Edit Agents", sf::Vector2f(100.f, 25.f))))
	{
		m_EditAgents = true;

	}

	if (m_EditAgents)
	{
		ImGui::ListBoxHeader("", sf::Vector2f(375.f, 300.f));
		ImGui::InputInt("Max number of agents", &m_MaxAgentsInPlay);
		if (ImGui::Button("Manage new Agent"))
		{
			m_AddAgents = true;
		}
		if (m_AddAgents)
		{
			ImGui::Text("Agent Location");
			ImGui::InputFloat("columns", &m_StartNode.x);
			ImGui::InputFloat("row", &m_StartNode.y);
			ImGui::Text("Target Location");
			ImGui::InputFloat("column Target", &m_EndNode.x);
			ImGui::InputFloat("row Target", &m_EndNode.y);

			if (ImGui::Button("Add"))
			{
				if (m_MaxAgentsInPlay > m_AgentManger->GetAgents().size())
				{
					Agent* agent = new Agent();
					agent->Init(((m_StartNode+1.f) * (float)m_GridSpace.m_TileSize ) - ((float)m_GridSpace.m_TileSize / 2.f));
					m_Pathmanager->InitPathManger(m_GridSpace.m_GridSpaceWidth, m_GridSpace.m_GridSpaceHeight, m_GridSpace.m_TileSize);
					m_Pathmanager->BuildNodeGraph(m_EnableConnections);
					agent->SetPath(m_Pathmanager->SolveAStar(m_StartNode * (float)m_GridSpace.m_TileSize, m_EndNode* (float)m_GridSpace.m_TileSize));
				//	agent->SetDestination(glm::vec2(358.f + Targetcolumn, Targetrow + 10.f));
					m_AgentManger->AddAgent(agent);
				}
				m_AddAgents = false;
			}
		}


		if (ImGui::Button("Close"))
		{
			m_EditAgents = false;
			m_AddAgents = false;
		}
		ImGui::ListBoxFooter();

	}
	if (ImGui::Button("Move all agents"))
	{
		for (int x = 0; x < m_AgentManger->GetAgents().size(); x++)
			m_AgentManger->GetAgents()[x]->SetState(1);
	}

ImGui::End();


}

void SFMLRenderer::DrawGrid()
{
	int counter = 0;

	//draw background
	shape.setSize(sf::Vector2f(m_GridSpace.m_GridSpaceHeight * m_GridSpace.m_TileSize, m_GridSpace.m_GridSpaceWidth * m_GridSpace.m_TileSize));
	shape.setPosition(m_GridSpace.m_Position);
	m_Window.draw(shape);


	//
	for (int x = 0; x <= m_GridSpace.m_GridSpaceWidth; x++)
	{
		sf::RectangleShape line;
		line.setSize(sf::Vector2f(m_GridSpace.m_GridSpaceHeight * m_GridSpace.m_TileSize, 3.f));
		line.setPosition(358.f, x * m_GridSpace.m_TileSize + 10.f);
		line.setFillColor(sf::Color(sf::Color::Black));
		m_Window.draw(line);
	}

	for (int x = 0; x <= m_GridSpace.m_GridSpaceHeight; x++)
	{
		sf::RectangleShape line;
		line.setSize(sf::Vector2f(3.f, m_GridSpace.m_GridSpaceWidth * m_GridSpace.m_TileSize));
		line.setPosition(358.f + (x * m_GridSpace.m_TileSize) , 10.f);
		line.setFillColor(sf::Color(sf::Color::Black));
		m_Window.draw(line);
	}

	
	
	for (int x = 0; x < m_GridSpace.m_Obstacles.size(); x++)
	{
		sf::CircleShape obstacle;
		obstacle.setRadius((m_GridSpace.m_TileSize / 2) -3.f);
		obstacle.setPosition(sf::Vector2f(361.f + (m_GridSpace.m_Obstacles[x].x * m_GridSpace.m_TileSize), (m_GridSpace.m_Obstacles[x].y * m_GridSpace.m_TileSize) + 13.f));
		m_Window.draw(obstacle);
	}

	

}

void SFMLRenderer::DrawSprite()
{

		for (int x = 0; x < m_AgentManger->GetAgents().size(); x++)
		{
			if(m_AgentManger->GetAgents()[x]->GetVisible())
				m_Window.draw(m_AgentManger->GetAgents()[x]->GetSprite());
		}
}

void SFMLRenderer::PollEventUI(sf::Event a_Event)
{
	ImGui::SFML::ProcessEvent(a_Event);
}


void SFMLRenderer::EditGridSpace()
{
	ImGui::ColorEdit3("Color Background", m_GridSpace.m_Color);
	shape.setFillColor(sf::Color
	(
		(int)(m_GridSpace.m_Color[0] * 255),
		(int)(m_GridSpace.m_Color[1] * 255),
		(int)(m_GridSpace.m_Color[2] * 255)
	)); // Background grid color
}

void SFMLRenderer::SpawnRandomObstacles()
{

}

void SFMLRenderer::AddSprite(sf::Sprite a_Sprite)
{
	m_SpriteToDraw.push_back(a_Sprite);
}



void SFMLRenderer::DrawPath()
{
	if (m_LastPath.size() >= 1)
	{
	PathToDraw.push_back(glm::vec2(m_LastPath[0].x, m_LastPath[0].y));
	m_LastPath.erase(m_LastPath.begin());
	}
	else {
		m_LastPath.clear();
	}
	if (PathToDraw.size() >= 1 )
	{

	
		for (int x = 0; x < PathToDraw.size()-1; x++)
		{
			
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(358.f + (PathToDraw[x].x )  , (PathToDraw[x].y ) + 10.f )),
				sf::Vertex(sf::Vector2f(sf::Vector2f(358.f + (PathToDraw[x+ 1].x )  , (PathToDraw[ x+1].y ) + 10.f )))
				
			};

			m_Window.draw(line, 2, sf::Lines);
			
		}

		
	}
}

void SFMLRenderer::DrawVisitedCells()
{
	if (m_GridSpace.m_VisitedCells.size() >= 1)
	{
		m_VisitedCells.push_back(glm::vec2(m_GridSpace.m_VisitedCells[0].x, m_GridSpace.m_VisitedCells[0].y));
		m_GridSpace.m_VisitedCells.erase(m_GridSpace.m_VisitedCells.begin());
	}
	else {
		m_GridSpace.m_VisitedCells.clear();
	}
	if (m_VisitedCells.size() >= 1)
	{


		for (int x = 0; x < m_VisitedCells.size() - 1; x++)
		{
			sf::RectangleShape Cell;
			Cell.setSize(sf::Vector2f(m_GridSpace.m_TileSize - 3, m_GridSpace.m_TileSize - 3));
			Cell.setPosition(361.f + (m_VisitedCells[x].x * m_GridSpace.m_TileSize), (m_VisitedCells[x].y * m_GridSpace.m_TileSize) + 13.f);
			Cell.setFillColor(sf::Color(55, 55, 55));
			m_Window.draw(Cell);
		}
	}
}



void SFMLRenderer::ClearObstacles()
{
	m_GridSpace.ClearObstacle();
	m_Pathmanager->ClearObstacles();
}