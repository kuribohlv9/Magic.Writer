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

	sf::Texture* buttonTexture = m_textureManager->LoadTexture("assets/sprites/spritesheet_menu.png");
	sf::Font* font = m_textureManager->LoadFont("assets/fonts/font.ttf");

	m_playButton = new GUI_Button(100, 100, nullptr, buttonTexture, 250, 100);
	m_playButton->SetLabel(new GUI_Label(90, 30, m_playButton, font, "Play"));

	m_exitButton = new GUI_Button(100, 220, nullptr, buttonTexture, 250, 100);
	m_exitButton->SetLabel(new GUI_Label(90, 30, m_exitButton, font, "Exit"));
}
MenuState::~MenuState()
{
	m_textureManager = nullptr;
	m_drawManager = nullptr;
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