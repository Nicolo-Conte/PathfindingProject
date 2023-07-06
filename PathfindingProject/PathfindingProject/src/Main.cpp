#include <iostream>
#include "SFMLRenderer.h"
#include <chrono>
#include <memory>

int main()
{

	AgentManager* agentManager = new AgentManager();
	std::chrono::system_clock::time_point m_StartTime;
	std::chrono::duration<float> m_DeltaTime;
	float m_ElapsedTime = 0.0;
	unsigned int m_Frames = 0;

	//Init Window
	SFMLRenderer* renderer = new SFMLRenderer();

	PathManager* pathmanager = new PathManager();
	renderer->Initialize(glm::vec2(1920.f, 1080.f), pathmanager, agentManager);
	pathmanager->InitPathManger(renderer->m_GridSpace.m_GridSpaceWidth, renderer->m_GridSpace.m_GridSpaceHeight, renderer->m_GridSpace.m_TileSize);
	pathmanager->InitGridValue();
	pathmanager->BuildNodeGraph(false);


	sf::Event event;
	while (renderer->GetWindow().isOpen()) {

		//Get input
		while (renderer->GetWindow().pollEvent(event))
		{
			renderer->PollEventUI(event);

			if (event.type == sf::Event::Closed)
				renderer->GetWindow().close();
		}
		m_ElapsedTime += m_DeltaTime.count();
		m_Frames++;
		if (m_ElapsedTime >= 1.0f) //1 second
		{
		m_ElapsedTime = 0.0f;
			m_Frames = 0;
		}
		agentManager->Update(m_DeltaTime.count());
		renderer->Update();

		////Deltatime
		m_DeltaTime = std::chrono::system_clock::now() - m_StartTime;
		m_StartTime = std::chrono::system_clock::now();
	
	}
		
	return 0;
}

