#include "Agent.h"

Agent::Agent()
{
    m_speed = 300.f;
    m_IsMoving = false;
    SetSprite();
    
}

void Agent::Update(float dt)
{
    switch (m_State)
    {
    case 0:
        break;
    case 1:
        Move(dt);
        break;
    default:
        break;
    }
    m_sprite.setPosition(sf::Vector2f(342.f + m_CurrentLocation.x, m_CurrentLocation.y -16.f));
}

void Agent::Move(float dt)
{
    glm::vec2 Dir = m_Destination - m_CurrentLocation;
    if (!CheckCurrentLocation(Dir))
    {
        
        float Dir_length = sqrt(Dir.x* Dir.x + Dir.y * Dir.y);
        Dir = glm::vec2(Dir.x / Dir_length, Dir.y / Dir_length);
        m_CurrentLocation = m_CurrentLocation + ( Dir * m_speed * dt);
       
    }
    else
    {
        if (m_Path.size() >= 1)
        {
            m_Destination = m_Path[0];
            m_Path.erase(m_Path.begin());
        
        }
        
        else
        {
            SetState(Idle);
           
        }
    }
}

void Agent::Init(glm::vec2 a_StartPos)
{
    m_CurrentLocation = a_StartPos;
}

bool Agent::CheckCurrentLocation(glm::vec2 a_vec)
{
    float Dir_length = sqrt(a_vec.x * a_vec.x + a_vec.y * a_vec.y);
    if (Dir_length <= 0.5f)
    {
        return true;
    }
    else
       return false;
}

void Agent::SetSprite()
{ 
    m_texture.loadFromFile("../Assets/Character.png", sf::IntRect(9, 14, 90, 115));
    m_texture.setSmooth(true);
    m_sprite.setTexture(m_texture);
    m_sprite.setScale(0.4f, 0.45f);
}

 void Agent::SetState(int a_State)
{
    switch (a_State)
    {
    case 0:
        m_State = Idle;
        m_IsMoving = false;
        break;
    case 1:
        m_State = Moving;
        m_IsMoving = true;
        break;
    default:
        m_State = Idle;
        m_IsMoving = false;
        break;
    }
  
}

sf::Sprite Agent::GetSprite()
{
    return m_sprite;
}

glm::vec2 Agent::GetCurrentLocation()
{
    return m_CurrentLocation;
}



void Agent::SetPath(std::vector<glm::vec2> a_Path)
{
    m_Path = a_Path;
    if (m_Path.size() >= 1)
    {
        m_Path.erase(m_Path.begin());
        SetDestination(a_Path[0]);
        return;
    }
    else
    {
        m_Path.push_back(m_CurrentLocation);
        return;
    }
}