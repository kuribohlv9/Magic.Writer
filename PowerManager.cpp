#include "stdafx.h"
#include "ServiceLocator.h"
#include "PowerManager.h"
#include "TextureManager.h"
#include "DrawManager.h"
#include "InputManager.h"
#include "Item.h"
#include "Player.h"
#include "Monster.h"

PowerManager::PowerManager(std::vector<Monster*>* monster, std::vector<Item*>* item, Player* player)
{
	m_inputManager = ServiceLocator<InputManager >::GetService();
	m_textureManager = ServiceLocator<TextureManager>::GetService();

	m_pierceSprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/hud/pierce.png"));
	m_bounceSprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/hud/bounce.png"));
	m_freezeSprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/hud/freeze.png"));
	m_frameSprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/hud/powerup_frame.png"));
	m_fillSprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/hud/powerup_fill.png"));

	m_frameSprite.setPosition(20, 600);
	m_fillSprite.setOrigin(0, m_frameSprite.getTextureRect().height);
	m_fillSprite.setPosition(20, 1000);

	m_item = item;
	m_monster = monster;
	m_player = player;
	m_nextBounceTarget = nullptr;

	m_frozen = false;
	m_pierce = false;
	m_bounce = false;
	
	m_powerupScore = 0;
	m_fillSpeed = .1f;
	m_stepSize = 100.0f / 3.0f;
	m_freezeTime = 5.0f;

	m_freezeSprite.setPosition(105, 620);
	m_bounceSprite.setPosition(105, 750);
	m_pierceSprite.setPosition(105, 880);
}

PowerManager::~PowerManager()
{

}

void PowerManager::Update(float deltaTime)
{
	UpdatePowerBar();

	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Num1) && m_powerupScore >= m_stepSize)
		ActivatePierce();

	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Num2) && m_powerupScore >= m_stepSize * 2.0f)
		ActivateBounce();

	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Num3) && m_powerupScore >= m_stepSize * 3.0f)
		ActivateFreeze();

	if (m_frozen)
	{
		m_freezeTimer += deltaTime;

		if (m_freezeTimer >= m_freezeTime)
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
void PowerManager::UpdatePowerBar()
{
	//Fill powerbar
	if (m_powerupScore < 100.0f)
		m_powerupScore += m_fillSpeed;

	//Scale fill sprite
	m_fillSprite.setScale(1, (m_powerupScore / 100.0f));

	//Set colors for powerup icons
	m_freezeSprite.setColor(sf::Color::White);
	m_bounceSprite.setColor(sf::Color::White);
	m_pierceSprite.setColor(sf::Color::White);
	sf::Color fadeColor = sf::Color(100, 100, 150, 255);
	if (m_powerupScore < m_stepSize * 3)
	{
		m_freezeSprite.setColor(fadeColor);
	}
	if (m_powerupScore < m_stepSize * 2)
	{
		m_bounceSprite.setColor(fadeColor);
	}
	if (m_powerupScore < m_stepSize)
	{
		m_pierceSprite.setColor(fadeColor);
	}
}
void PowerManager::Draw(DrawManager* drawManager)
{
	drawManager->Draw(m_fillSprite, sf::RenderStates::Default);
	drawManager->Draw(m_frameSprite, sf::RenderStates::Default);

	drawManager->Draw(m_freezeSprite, sf::RenderStates::Default);
	drawManager->Draw(m_bounceSprite, sf::RenderStates::Default);
	drawManager->Draw(m_pierceSprite, sf::RenderStates::Default);
}

void PowerManager::ActivatePierce()
{
	if (m_player->GetItem() != nullptr)
	{
		m_pierceCurrentItem = m_player->GetItem();
		m_powerupScore -= m_stepSize;
	}
}
void PowerManager::ActivateBounce()
{
	if (m_player->GetItem() != nullptr)
	{
		m_bounceCurrentItem = m_player->GetItem();
		m_powerupScore -= m_stepSize * 2;
	}
}
void PowerManager::ActivateFreeze()
{
	if (!m_frozen)
	{
		m_frozen = true;
		m_powerupScore -= m_stepSize * 3;

		for (unsigned int i = 0; i < m_monster->size(); i++)
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

	for (unsigned int i = 0; i < m_monster->size(); i++)
	{
		if (!m_monster->at(i)->IsActive())
			continue;

		m_monster->at(i)->Freeze(false);
	}

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
		for (unsigned int i = 0; i < m_monsterPierceList.size(); i++)
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

		for (unsigned int i = 0; i < m_monster->size(); i++)
		{
			if (!m_monster->at(i)->IsActive())
				continue;

			if (m_monster->at(i)->GetX() != monster->GetX() && m_monster->at(i)->GetY() >= -15)
			{
				bool isInList = false;
				for (unsigned int k = 0; k < m_LaneBounceList.size(); k++)
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
