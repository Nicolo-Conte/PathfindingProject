#include "AgentManager.h"

AgentManager::AgentManager()
{
}

void AgentManager::AddAgent(Agent* a_Agent)
{
	m_Agents.push_back(a_Agent);
}

void AgentManager::RemoveAgnet(Agent* a_Agent)
{
	//erase-remove idiom
	m_Agents.erase(std::remove(m_Agents.begin(), m_Agents.end(), a_Agent), m_Agents.end());
}

void AgentManager::CreateAgent()
{
	Agent* newAgent = new Agent();
	AddAgent(newAgent);
}

void AgentManager::Update(float a_dt)
{
	for (int x = 0; x < m_Agents.size(); x++)
		m_Agents[x]->Update(a_dt);
}
