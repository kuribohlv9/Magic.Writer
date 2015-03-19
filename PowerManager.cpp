#include "stdafx.h"
#include "ServiceLocator.h"
#include "PowerManager.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "TextureManager.h"
#include "DrawManager.h"
#include "InputManager.h"
#include "Item.h"
#include "Player.h"
#include "Monster.h"
#include <iostream>

PowerManager::PowerManager(std::vector<Monster*>* monsters, std::vector<Item*>* activeItems, Player* player)
{
	m_inputManager = ServiceLocator<InputManager >::GetService();
	m_textureManager = ServiceLocator<TextureManager>::GetService();

	m_pierceSprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/hud/pierce.png"));
	m_bounceSprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/hud/bounce.png"));
	m_freezeSprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/hud/freeze.png"));

	m_activeItems = activeItems;
	m_monsters = monsters;
	m_player = player;
	m_bounceTarget = nullptr;

	m_frozen = false;
	

	m_freezeTime = 9;
	m_timerDelay = 1.5f;
	m_pluppTimer = 0;
	m_activePlupps = 0;
	m_fadeColor = sf::Color(100, 100, 150, 255);
	
	m_pierceSprite.setOrigin(96 / 2, 96 / 2);
	m_pierceSprite.setScale(0.8, 0.8);
	m_pierceSprite.setPosition(123, 596);

	m_bounceSprite.setOrigin(96 / 2, 96 / 2);
	m_bounceSprite.setScale(0.8, 0.8);
	m_bounceSprite.setPosition(123, 320);

	m_freezeSprite.setOrigin(96 / 2, 96 / 2);
	m_freezeSprite.setScale(0.8, 0.8);
	m_freezeSprite.setPosition(123, 48);

	sf::Texture* texture = m_textureManager->LoadTexture("assets/sprites/hud/pluppen.png");
	int extraHeight = 0;
	for (unsigned int i = 0; i < 24; i++)
	{
		sf::Sprite s;
		s.setTexture(*texture);
		s.setOrigin(texture->getSize().x / 2, texture->getSize().y / 2);

		if (i == 7 || i == 15 || i == 23)
			extraHeight += 50;

		s.setPosition(125, (texture->getSize().y * 22) - i * texture->getSize().y - i * -8 - extraHeight);

		m_plupps.push_back(s);
	}
}
PowerManager::~PowerManager()
{

}

void PowerManager::Update(float deltaTime)
{
	//Update power up bar
	m_pluppTimer += deltaTime;
	UpdatePowerBar();

	//Check for power up input
	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Num1) && m_activePlupps >= 7)
		ActivatePierce();
	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Num2) && m_activePlupps >= 15)
		ActivateBounce();
	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Num3) && m_activePlupps >= 23)
		ActivateFreeze();

	//Reset frozen
	if (m_frozen)
	{
		m_freezeTimer += deltaTime;

		if (m_freezeTimer >= m_freezeTime)
		{
			m_freezeTimer = 0;
			RemoveFreeze();
		}
	}

	//Reset pierce
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
	if (m_pluppTimer >= m_timerDelay)
	{
		m_pluppTimer -= m_timerDelay;

		AddPowerupPlupps(1);
	}
	for (unsigned int i = 0; i < m_plupps.size(); i++)
	{
		sf::Color color = sf::Color(241, 132, 132, 255); // Red

		if (i < m_activePlupps)
			color = sf::Color::White;

		m_plupps[i].setColor(color);
	}

	//Set colors for powerup icons
	m_freezeSprite.setColor(m_fadeColor);
	m_bounceSprite.setColor(m_fadeColor);
	m_pierceSprite.setColor(m_fadeColor);
	if (m_activePlupps >= 7)
	{
		m_pierceSprite.setColor(sf::Color::White);
	}
	if (m_activePlupps >= 15)
	{
		m_bounceSprite.setColor(sf::Color::White);
	}
	if (m_activePlupps >= 23)
	{
		m_freezeSprite.setColor(sf::Color::White);
	}
	std::cout << m_activePlupps << std::endl;
}
void PowerManager::Draw(DrawManager* drawManager)
{
	//Draw plupps
	for (unsigned int i = 0; i < m_plupps.size(); i++)
	{
		drawManager->Draw(m_plupps[i], sf::RenderStates::Default);
	}

	//Draw power up icons
	drawManager->Draw(m_freezeSprite, sf::RenderStates::Default);
	drawManager->Draw(m_bounceSprite, sf::RenderStates::Default);
	drawManager->Draw(m_pierceSprite, sf::RenderStates::Default);
}
void PowerManager::AddPowerupPlupps(int amount)
{
	m_activePlupps += amount;
	if (m_activePlupps > 23)
		m_activePlupps = 23;
}
bool PowerManager::GetReadyPowerup(int powerupIndex)
{
	switch (powerupIndex)
	{
	case 0:
		return (m_activePlupps >= 7);
		break;
	case 1:
		return (m_activePlupps >= 15);
		break;
	case 2:
		return (m_activePlupps >= 23);
		break;
	}
}

//Bounce methods
void PowerManager::ActivateBounce()
{
	if (m_player->GetItem() != nullptr)
	{
		//Get bounce item
		m_bounceCurrentItem = m_player->GetItem();

		//Remove plupps
		m_activePlupps -= 15;

		//Activate bounce item
		m_bounceCurrentItem->Activate();
		m_activeItems->push_back(m_bounceCurrentItem);
		m_player->SetItem(nullptr);
	}
}
bool PowerManager::SetNewBounceTarget(Monster* hitMonster)
{
	m_LaneBounceList.push_back(hitMonster->GetX());
	if (m_bounceCurrentItem != nullptr)
	{
		for (unsigned int i = 0; i < m_monsters->size(); i++)
		{
			Monster* monster = m_monsters->at(i);
			if (!monster->IsActive())
				continue;

			if (monster->GetY() >= -15 && monster->GetY() <= 760)
			{
				bool isInList = false;
				for (unsigned int k = 0; k < m_LaneBounceList.size(); k++)
				{
					if (m_LaneBounceList.at(k) == monster->GetX())
					{
						isInList = true;
						break;
					}
				}
				if (!isInList)
				{
					m_bounceTarget = monster;
					return true;
				}
			}
		}
	}
	m_bounceTarget = nullptr;
	m_bounceCurrentItem = nullptr;
	m_LaneBounceList.clear();

	return false;
}
Item* PowerManager::GetBounceItem()
{
	if (m_bounceCurrentItem != nullptr)
	{
		if (m_bounceCurrentItem->IsActive())
			return m_bounceCurrentItem;
	}
	return nullptr;
}
Monster* PowerManager::GetBounceTarget()
{
	if (m_bounceTarget != nullptr)
		return m_bounceTarget;

	return nullptr;
}
sf::Vector2f PowerManager::GetItemDirection()
{
	if (m_bounceCurrentItem == nullptr || m_bounceTarget == nullptr)
		return sf::Vector2f(0, 0);


	sf::Vector2f itemPos = sf::Vector2f(m_bounceCurrentItem->GetX(), m_bounceCurrentItem->GetY());
	sf::Vector2f monsterPos = sf::Vector2f(m_bounceTarget->GetX(), m_bounceTarget->GetY());

	sf::Vector2f m_itemDir = monsterPos - itemPos;

	float length = sqrt(m_itemDir.x * m_itemDir.x + m_itemDir.y * m_itemDir.y);

	m_itemDir /= length;

	return m_itemDir;
}

//Freeze methods
void PowerManager::ActivateFreeze()
{
	if (!m_frozen)
	{
		//Activate freeze
		m_frozen = true;

		//Remove power
		m_activePlupps -= 23;

		//Freeze all monsters
		for (unsigned int i = 0; i < m_monsters->size(); i++)
		{
			if (!m_monsters->at(i)->IsActive())
				continue;

			m_monsters->at(i)->Freeze(true);

		}
	}

}
void PowerManager::RemoveFreeze()
{
	m_frozen = false;

	//Unfreeze all monsters
	for (unsigned int i = 0; i < m_monsters->size(); i++)
	{
		if (!m_monsters->at(i)->IsActive())
			continue;

		m_monsters->at(i)->Freeze(false);
	}

}
bool PowerManager::IsFrozen()
{
	return m_frozen;
}

//Pierce methods
void PowerManager::ActivatePierce()
{
	if (m_player->GetItem() != nullptr)
	{
		//Get pierce item
		m_pierceCurrentItem = m_player->GetItem();

		//Remove power
		m_activePlupps -= 7;

		//Activate pierce item
		m_pierceCurrentItem->Activate();
		m_activeItems->push_back(m_pierceCurrentItem);
		m_player->SetItem(nullptr);
	}
}
Item* PowerManager::GetPierceItem()
{
	if (m_pierceCurrentItem != nullptr)
	{
		if (m_pierceCurrentItem->IsActive())
			return m_pierceCurrentItem;
	}
	return nullptr;
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
