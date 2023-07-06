#pragma once
#include "Agent.h"

class AgentManager
{
public:
	AgentManager();
	~AgentManager();

	void AddAgent(Agent* a_Agent);
	void RemoveAgnet(Agent* a_Agent);
	void CreateAgent();
	void Update(float a_dt);
	inline std::vector<Agent*> GetAgents();

private:

	std::vector<Agent*> m_Agents;
};

std::vector<Agent*> AgentManager::GetAgents()
{
	return m_Agents;
}
