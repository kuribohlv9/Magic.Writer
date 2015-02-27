#include "stdafx.h"
#include "ServiceLocator.h"
#include "PowerUpManager.h"
#include "InputManager.h"
#include "Item.h"
#include "Monster.h"

PowerUpManager::PowerUpManager(std::vector<Monster*>* monster, std::vector<Item*>* item)
{
	m_inputManager = ServiceLocator<InputManager >::GetService();

	m_item = item;
	m_monster = monster;

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
		ActivatePierce(0);

	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Num3))
		ActivateFreeze();

	if (m_frozen)
	{
		m_freezeTimer += deltaTime;

		if (m_freezeTimer >= 1.5f)
		{
			m_freezeTimer = 0;
			RemoveFreeze();
		}
	}
	
}

void PowerUpManager::ActivateBounce()
{
	
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

void PowerUpManager::ActivatePierce(int pierce)
{
	if (pierce == 0)
		m_pierce = true;
	else if (pierce == 1)
		m_pierce = false;
}

bool PowerUpManager::GetPierce()
{
	return m_pierce;
}

bool PowerUpManager::GetFrozen()
{
	return m_frozen;
}