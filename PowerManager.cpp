#include "stdafx.h"
#include "ServiceLocator.h"
#include "PowerManager.h"
#include "TextureManager.h"
#include "DrawManager.h"
#include "InputManager.h"
#include "Item.h"
#include "Player.h"
#include "Monster.h"

PowerManager::PowerManager(std::vector<Monster*>* monsters, std::vector<Item*>* activeItems, Player* player)
{
	m_inputManager = ServiceLocator<InputManager >::GetService();
	m_textureManager = ServiceLocator<TextureManager>::GetService();

	m_pierceSprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/hud/pierce.png"));
	m_bounceSprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/hud/bounce.png"));
	m_freezeSprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/hud/freeze.png"));
	m_frameSprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/hud/powerup_frame.png"));
	m_fillSprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/hud/powerup_fill.png"));

	m_frameSprite.setPosition(20, 400);
	m_fillSprite.setOrigin(0, m_frameSprite.getTextureRect().height);
	m_fillSprite.setPosition(20, 800);

	m_activeItems = activeItems;
	m_monsters = monsters;
	m_player = player;
	m_bounceTarget = nullptr;

	m_frozen = false;
	
	m_powerupScore = 0;
	m_fillSpeed = .1f;
	m_stepSize = 100.0f / 3.0f;
	m_freezeTime = 5.0f;
	m_fadeColor = sf::Color(100, 100, 150, 255);

	m_freezeSprite.setPosition(105, 420);
	m_bounceSprite.setPosition(105, 550);
	m_pierceSprite.setPosition(105, 680);
}

PowerManager::~PowerManager()
{

}

void PowerManager::Update(float deltaTime)
{
	//Update power up bar
	UpdatePowerBar();

	//Check for power up input
	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Num1) && m_powerupScore >= m_stepSize)
		ActivatePierce();
	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Num2) && m_powerupScore >= m_stepSize * 2.0f)
		ActivateBounce();
	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Num3) && m_powerupScore >= m_stepSize * 3.0f)
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
	//Fill powerbar
	if (m_powerupScore < 100.0f)
		m_powerupScore += m_fillSpeed;

	//Scale fill sprite
	m_fillSprite.setScale(1, (m_powerupScore / 100.0f));

	//Set colors for powerup icons
	m_freezeSprite.setColor(sf::Color::White);
	m_bounceSprite.setColor(sf::Color::White);
	m_pierceSprite.setColor(sf::Color::White);
	if (m_powerupScore < m_stepSize * 3)
	{
		m_freezeSprite.setColor(m_fadeColor);
	}
	if (m_powerupScore < m_stepSize * 2)
	{
		m_bounceSprite.setColor(m_fadeColor);
	}
	if (m_powerupScore < m_stepSize)
	{
		m_pierceSprite.setColor(m_fadeColor);
	}
}
void PowerManager::Draw(DrawManager* drawManager)
{
	//Draw power up bar
	drawManager->Draw(m_fillSprite, sf::RenderStates::Default);
	drawManager->Draw(m_frameSprite, sf::RenderStates::Default);

	//Draw power up icons
	drawManager->Draw(m_freezeSprite, sf::RenderStates::Default);
	drawManager->Draw(m_bounceSprite, sf::RenderStates::Default);
	drawManager->Draw(m_pierceSprite, sf::RenderStates::Default);
}

//Bounce methods
void PowerManager::ActivateBounce()
{
	if (m_player->GetItem() != nullptr)
	{
		//Get bounce item
		m_bounceCurrentItem = m_player->GetItem();

		//Remove power
		m_powerupScore -= m_stepSize * 2;

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

			if (monster->GetY() >= -15 && monster->GetY() <= 800)
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
		m_powerupScore -= m_stepSize * 3;

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
		m_powerupScore -= m_stepSize;

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
