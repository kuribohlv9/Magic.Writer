#include "stdafx.h"
#include "LosingState.h"
#include "ServiceLocator.h"
#include "DrawManager.h"

LosingState::LosingState()
{
	m_drawManager = ServiceLocator<DrawManager>::GetService();
}


LosingState::~LosingState()
{
}

bool LosingState::Update(float deltaTime)
{
	return true;
}

void LosingState::Draw()
{
	sf::CircleShape derp;
	derp.setRadius(50);
	derp.setPosition(100, 100);
	m_drawManager->Draw(derp, sf::RenderStates::Default);
}

void LosingState::Enter()
{

}

void LosingState::Exit()
{

}

ScreenState LosingState::NextState()
{
	return STATE_INVALID;
}