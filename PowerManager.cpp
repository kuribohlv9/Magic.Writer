#include "stdafx.h"
#include "ServiceLocator.h"
#include "PowerManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "DrawManager.h"
#include "InputManager.h"
#include "Item.h"
#include "Player.h"
#include "Monster.h"

PowerManager::PowerManager(std::vector<Monster*>* monster, std::vector<Item*>* item, Player* player)
{
	m_inputManager = ServiceLocator<InputManager >::GetService();
	m_textureManager = ServiceLocator<TextureManager>::GetService();

	m_frameSprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/hud/powerup_frame.png"));
	m_fillSprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/hud/powerup_fill.png"));

	m_frameSprite.setPosition(0, 600);
	m_fillSprite.setOrigin(0, m_frameSprite.getTextureRect().height);
	m_fillSprite.setPosition(0, 1000);

	m_item = item;
	m_monster = monster;
	m_player = player;
	m_nextBounceTarget = nullptr;

	m_frozen = false;
	m_pierce = false;
	m_bounce = false;
	
	m_powerupScore = 0;
}

PowerManager::~PowerManager()
{

}

void PowerManager::Update(float deltaTime)
{
	m_fillSprite.setScale(1, (m_powerupScore / 100.0f));
	if (m_powerupScore < 100)
		m_powerupScore += 0.1f;

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
}
void PowerManager::Draw(DrawManager* drawManager)
{
	drawManager->Draw(m_fillSprite, sf::RenderStates::Default);
	drawManager->Draw(m_frameSprite, sf::RenderStates::Default);
}

void PowerManager::ActivateBounce()
{
	if (m_player != nullptr)
		m_bounceCurrentItem = m_player->GetItem();
}

void PowerManager::ActivateFreeze()
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

void PowerManager::RemoveFreeze()
{
	m_frozen = false;

	for (int i = 0; i < m_monster->size(); i++)
	{
		if (!m_monster->at(i)->IsActive())
			continue;

		m_monster->at(i)->Freeze(false);
	}

}

void PowerManager::ActivatePierce()
{
	if (m_player->GetItem() != nullptr)
		m_pierceCurrentItem = m_player->GetItem();

}

bool PowerManager::GetPierce()
{
	if (m_pierceCurrentItem != nullptr)
	{
		if (m_pierceCurrentItem->IsActive())
			return true;
	}
	return false;
}

bool PowerManager::GetFrozen()
{
	return m_frozen;
}

bool PowerManager::AddItemToPierceList(Monster* monster)
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

bool PowerManager::GetBounce()
{
	if (m_bounceCurrentItem != nullptr)
	{
		if (m_bounceCurrentItem->IsActive())
			return true;
	}

	return false;
}

bool PowerManager::NextBounce(Monster* monster)
{
	m_LaneBounceList.push_back(monster->GetX());
	if (m_bounceCurrentItem != nullptr)
	{

		for (int i = 0; i < m_monster->size(); i++)
		{
			if (!m_monster->at(i)->IsActive())
				continue;

			if (m_monster->at(i)->GetX() != monster->GetX() && m_monster->at(i)->GetY() >= -15)
			{
				bool isInList = false;
				for (int k = 0; k < m_LaneBounceList.size(); k++)
				{
					if (m_LaneBounceList.at(k) == m_monster->at(i)->GetX())
					{
						isInList = true;
						break;
					}
				}

				if (!isInList)
				{
					m_nextBounceTarget = m_monster->at(i);
					return true;
				}
			}
		}
	}

	m_nextBounceTarget = nullptr;
	m_bounceCurrentItem = nullptr;
	m_LaneBounceList.clear();

	return false;
}

Item* PowerManager::BounceItem()
{
	if (m_bounceCurrentItem != nullptr)
		return m_bounceCurrentItem;
	return nullptr;
}

Monster* PowerManager::NextBounceTarget()
{
	if (m_nextBounceTarget != nullptr)
		return m_nextBounceTarget;
	return nullptr;
}

sf::Vector2f PowerManager::ItemDirection()
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
