#include "stdafx.h"
#include "ServiceLocator.h"
#include "PowerUpManager.h"
#include "InputManager.h"
#include "Item.h"
#include "Player.h"
#include "Monster.h"

PowerUpManager::PowerUpManager(std::vector<Monster*>* monster, std::vector<Item*>* item, Player* player)
{
	m_inputManager = ServiceLocator<InputManager >::GetService();

	m_item = item;
	m_monster = monster;
	m_player = player;
	m_nextBounceTarget = nullptr;

	m_frozen = false;
	m_pierce = false;
	m_bounce = false;
}

PowerUpManager::~PowerUpManager()
{

}

void PowerUpManager::Update(float deltaTime)
{
	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Num1))
		ActivateBounce();

	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Num2))
	{

		ActivatePierce();
	}

	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Num3))
		ActivateFreeze();

	if (m_pierce)
	{
		
	}

	if (m_frozen)
	{
		m_freezeTimer += deltaTime;

		if (m_freezeTimer >= 1.5f)
		{
			m_freezeTimer = 0;
			RemoveFreeze();
		}
	}
	if (m_pierceCurrentItem != nullptr)
	{
		if (!m_pierceCurrentItem->IsActive() && m_player->GetItem() != m_pierceCurrentItem)
		{
			m_pierceCurrentItem = nullptr;
			m_monsterPierceList.clear();
		}
	}
	if (m_bounceCurrentItem != nullptr)
	{
		if (!m_bounceCurrentItem->IsActive() && m_player->GetItem() != m_bounceCurrentItem)
			m_bounceCurrentItem = nullptr;
	}
}

void PowerUpManager::ActivateBounce()
{
	if (m_player != nullptr)
		m_bounceCurrentItem = m_player->GetItem();
}

void PowerUpManager::ActivateFreeze()
{
	if (!m_frozen)
	{
		m_frozen = true;

		for (int i = 0; i < m_monster->size(); i++)
		{
			if (!m_monster->at(i)->IsActive())
				continue;

			m_monster->at(i)->Freeze(true);

		}
	}
	
}

void PowerUpManager::RemoveFreeze()
{
	m_frozen = false;

	for (int i = 0; i < m_monster->size(); i++)
	{
		if (!m_monster->at(i)->IsActive())
			continue;

		m_monster->at(i)->Freeze(false);
	}
	
}

void PowerUpManager::ActivatePierce()
{
	if (m_player->GetItem() != nullptr)
		m_pierceCurrentItem = m_player->GetItem();
	
}

bool PowerUpManager::GetPierce()
{
	if (m_pierceCurrentItem != nullptr)
	{
		if (m_pierceCurrentItem->IsActive())
			return true;
	}
	return false;
}

bool PowerUpManager::GetFrozen()
{
	return m_frozen;
}

bool PowerUpManager::AddItemToPierceList(Monster* monster)
{
	bool isInPool = false;
	if (monster != nullptr)
	{
		for (int i = 0; i < m_monsterPierceList.size(); i++)
		{
			if (m_monsterPierceList[i] == monster)
			{
				isInPool = true;
				break;
			}
		}

		if (!isInPool)
		{
			m_monsterPierceList.push_back(monster);
			return true;
		}
	}
	return false;
}

bool PowerUpManager::GetBounce()
{
	if (m_bounceCurrentItem != nullptr)
	{
		if (m_bounceCurrentItem->IsActive())
			return true;
	}
	return false;
}

void PowerUpManager::NextBounce(Monster* monster)
{
	if (m_bounceCurrentItem != nullptr)
	{
		bool foundNewTarget = false;

		for (int i = 0; i < m_monster->size(); i++)
		{
			if (!m_monster->at(i)->IsActive())
				continue;

			for (int j = 0; j < 5; j++)
			{
				if (m_monster->at(i)->GetX() != monster->GetX())
				{
					for (int k = 0; k < m_LaneBounceList.size(); k++)
					{
						if (m_LaneBounceList.at(i) == m_monster->at(i)->GetX());
					}
					m_nextBounceTarget = m_monster->at(i);
					foundNewTarget = true;
					break;
				}
			}

			if (foundNewTarget)
			{
				break;
			}
		}
	}
}

Item* PowerUpManager::BounceItem()
{
	if (m_bounceCurrentItem != nullptr)
		return m_bounceCurrentItem;
	return nullptr;
}

Monster* PowerUpManager::NextBounceTarget()
{
	if (m_nextBounceTarget != nullptr)
		return m_nextBounceTarget;
	return nullptr;
}

sf::Vector2f PowerUpManager::ItemDirection()
{	
	if (m_bounceCurrentItem != nullptr)
		m_itemPos = sf::Vector2f(m_bounceCurrentItem->GetX(), m_bounceCurrentItem->GetY());

	if (m_nextBounceTarget != nullptr)
		m_monsterPos = sf::Vector2f(m_nextBounceTarget->GetX(), m_nextBounceTarget->GetY());

	sf::Vector2f m_itemDir = m_monsterPos - m_itemPos;
	
	float length = sqrt(m_itemDir.x * m_itemDir.x + m_itemDir.y * m_itemDir.y);

	m_itemDir /= length;
	
	return m_itemDir;
}

void PowerUpManager::AddLaneToBounceList(int x)
{
	for (int i = 0; i < m_LaneBounceList.size(); i++)
	{
		if (m_LaneBounceList.at(i) == x)
			continue;
		m_LaneBounceList.push_back(x);
	}
}