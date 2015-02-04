#include "stdafx.h"
#include "MenuState.h"

//Managers
#include "ServiceLocator.h"
#include "DrawManager.h"
#include "TextureManager.h"

//GUI elements
#include "GUI_Button.h"
#include "GUI_Label.h"
#include "GUI_Panel.h"

MenuState::MenuState()
{
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	m_drawManager = ServiceLocator<DrawManager>::GetService();

	m_nextState = STATE_INVALID;

	sf::Texture* menuTexture = m_textureManager->LoadTexture("assets/sprites/spritesheet_menu.png");
	sf::Font* font = m_textureManager->LoadFont("assets/fonts/font.ttf");

	m_playButton = new GUI_Button(20, 20, nullptr, menuTexture, 250, 100);
	m_playButton->SetLabel(new GUI_Label(90, 30, m_playButton, font, "Play"));

	m_exitButton = new GUI_Button(20, 150, nullptr, menuTexture, 250, 100);
	m_exitButton->SetLabel(new GUI_Label(90, 30, m_exitButton, font, "Exit"));

	m_panel = new GUI_Panel(200, 200, nullptr, menuTexture);
	m_panel->AddChild(m_playButton);
	m_panel->AddChild(m_exitButton);
}
MenuState::~MenuState()
{
	m_textureManager = nullptr;
	m_drawManager = nullptr;
}
bool MenuState::Update(float deltaTime)
{
	m_panel->Update();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		m_panel->SetPosition(300, 100);
	}

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
	m_panel->Draw(m_drawManager);
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