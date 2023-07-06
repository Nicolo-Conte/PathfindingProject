#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Agent
{
public:

	enum AgentState
	{
		Idle = 0,
		Moving = 1,
	};

	Agent();
	~Agent();

	void Update(float dt);
	void Move(float dt);
	void Init(glm::vec2 a_StartPos);
	bool CheckCurrentLocation(glm::vec2 a_vec);
	void SetSprite();
	void SetState(int a_State);
	inline bool GetVisible();
	inline void SetDestination(glm::vec2 a_Destination);
	void SetPath(std::vector<glm::vec2> a_Path);
	sf::Sprite GetSprite();
	glm::vec2 GetCurrentLocation();
	

private:

	glm::vec2 m_CurrentLocation;
	bool m_IsMoving;
	sf::Texture m_texture;
	float m_speed;
	glm::vec2 m_Destination;
	std::vector<glm::vec2> m_Path;
	sf::Sprite m_sprite;
	AgentState m_State = Idle;
	bool m_IsVisible = true;
	


};

inline bool Agent::GetVisible()
{
	return m_IsVisible;
}

inline void Agent::SetDestination(glm::vec2 a_Destination)
{
	m_Destination = a_Destination;
}
