#include "stdafx.h"
#include "MenuState.h"

//Managers
#include "ServiceLocator.h"
#include "DrawManager.h"
#include "TextureManager.h"

//GUI elements
#include "GUI_Button.h"
#include "GUI_Label.h"

MenuState::MenuState()
{
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	m_drawManager = ServiceLocator<DrawManager>::GetService();
	m_nextState = STATE_INVALID;

	sf::Texture* texture = m_textureManager->LoadTexture("assets/sprites/sign_spritesheet.png");;

	m_poleSprite.setTexture(*texture);
	m_poleSprite.setTextureRect(sf::IntRect(0, 219 * 2, 167, 775));
	m_poleSprite.setPosition(ScreenWidth / 2, ScreenHeight - 775);

	m_playButton = new GUI_Button(ScreenWidth / 2 - 30, 300, nullptr, texture, sf::IntRect(0, 0, 431, 219));
	m_exitButton = new GUI_Button(ScreenWidth / 2 - 110, 700, nullptr, texture, sf::IntRect(431 * 2, 0, 431, 219));

	texture = m_textureManager->LoadTexture("assets/sprites/background/background.png");
	m_backgroundSprite.setTexture(*texture);
}
MenuState::~MenuState()
{
	m_textureManager = nullptr;
	m_drawManager = nullptr;
	
	if (m_playButton)
	{
		delete m_playButton;
	}
	if (m_exitButton)
	{
		delete m_exitButton;
	}
}
bool MenuState::Update(float deltaTime)
{
	m_playButton->Update();
	m_exitButton->Update();
	if (m_playButton->IsPressed())
	{
		m_nextState = STATE_GAME;
		return false;
	}
	else if (m_exitButton->IsPressed())
	{
		m_nextState = STATE_INVALID;
		return false;
	}
	return true;
}
void MenuState::Draw()
{
	m_drawManager->Draw(m_backgroundSprite, sf::RenderStates::Default);
	m_drawManager->Draw(m_poleSprite, sf::RenderStates::Default);

	m_playButton->Draw(m_drawManager);
	m_exitButton->Draw(m_drawManager);
}

void MenuState::Enter()
{

}
void MenuState::Exit()
{

}

ScreenState MenuState::NextState()
{
	return m_nextState;
}