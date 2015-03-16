#include "stdafx.h"
#include "InstructionState.h"

//Managers
#include "DrawManager.h"
#include "TextureManager.h"
#include "ServiceLocator.h"

//GUI elements
#include "GUI_Button.h"
#include "GUI_Label.h"
#include <iostream>

InstructionState::InstructionState()
{
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	m_drawManager = ServiceLocator<DrawManager>::GetService();
	m_nextState = STATE_MENU;

	//First help screen texture
	sf::Texture* texture = m_textureManager->LoadTexture("assets/sprites/splashscreen.png");
	m_firstHelpPage.setTexture(*texture);

	//Second help screen texture
	texture = m_textureManager->LoadTexture("assets/sprites/splashscreen-2.png");
	m_secondHelpPage.setTexture(*texture);

	//GUI button textures
	texture = m_textureManager->LoadTexture("assets/sprites/back_next_exit.png");

	//GUI buttons
	m_nextPageButton = new GUI_Button(ScreenWidth - 150, ScreenHeight - 100, nullptr, texture, sf::IntRect(0, 0, 312, 178));
	m_lastPageButton = new GUI_Button(150, ScreenHeight - 100, nullptr, texture, sf::IntRect(312, 0, 312, 178));
	m_exitToMenuButton = new GUI_Button(ScreenWidth - 150, ScreenHeight - 100, nullptr, texture, sf::IntRect(624, 0, 312, 178));
}

InstructionState::~InstructionState()
{
	m_textureManager = nullptr;
	m_drawManager = nullptr;
}

bool InstructionState::Update(float deltaTime)
{
	m_nextPageButton->Update();
	m_lastPageButton->Update();
	m_exitToMenuButton->Update();

	if (m_nextPageButton->IsPressed() && m_firstPage || m_lastPageButton->IsPressed() && !m_firstPage)
	{
		m_firstPage = !m_firstPage;
	}
	else if (m_exitToMenuButton->IsPressed())
	{
		m_nextState = STATE_MENU;
		return false;
	}
	return true;
}

void InstructionState::Draw()
{
	if (m_firstPage)
	{
		m_drawManager->Draw(m_firstHelpPage, sf::RenderStates::Default);
		m_nextPageButton->Draw(m_drawManager);
	}
	else
	{
		m_drawManager->Draw(m_secondHelpPage, sf::RenderStates::Default);
		m_lastPageButton->Draw(m_drawManager);
		m_exitToMenuButton->Draw(m_drawManager);
	}
}

void InstructionState::Enter()
{
	m_firstPage = true;
}

void InstructionState::Exit()
{

}

ScreenState InstructionState::NextState()
{
	return m_nextState;
}