#include "stdafx.h"
#include "InstructionState.h"

//Managers
#include "DrawManager.h"
#include "TextureManager.h"
#include "ServiceLocator.h"

//GUI elements
#include "GUI_Button.h"
#include "GUI_Label.h"

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

	//Next page texture
	texture = m_textureManager->LoadTexture("assets/sprites/next_page.png");

	//GUI buttons
	m_nextPage = new GUI_Button(ScreenWidth - 100, ScreenHeight - 100, nullptr, texture, sf::IntRect(0, 0, 73, 74));
	m_buttons.push_back(m_nextPage);
	m_lastPage = new GUI_Button(100, ScreenHeight - 100, nullptr, texture, sf::IntRect(0, 0, 73, 74));
	m_buttons.push_back(m_lastPage);
	
	//Exit button texture
	texture = m_textureManager->LoadTexture("assets/sprites/sign_spritesheet.png");
	m_exitToMenuButton = new GUI_Button(ScreenWidth / 2, ScreenHeight - 100, nullptr, texture, sf::IntRect(421 * 2 + 60, 0, 431 - 60, 219));
	m_buttons.push_back(m_exitToMenuButton);
}

InstructionState::~InstructionState()
{
	auto itr = m_buttons.begin();
	while (itr != m_buttons.end())
	{
		if (*itr)
		{
			delete *itr;
		}
		itr++;
	}
	m_buttons.clear();

	m_textureManager = nullptr;
	m_drawManager = nullptr;
}

bool InstructionState::Update(float deltaTime)
{
	for (unsigned int i = 0; i < m_buttons.size(); i++)
	{
		m_buttons[i]->Update();
	}

	if (m_nextPage->IsPressed() && m_firstPage)
		m_firstPage = false;
	
	if (m_lastPage->IsPressed() && !m_firstPage)
		m_firstPage = true;

	if (m_exitToMenuButton->IsPressed())
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
		m_nextPage->Draw(m_drawManager);
	}
	else
	{
		m_drawManager->Draw(m_secondHelpPage, sf::RenderStates::Default);
		m_lastPage->Draw(m_drawManager);
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