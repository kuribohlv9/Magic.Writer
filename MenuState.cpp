#include "stdafx.h"
#include "MenuState.h"

//Managers
#include "ServiceLocator.h"
#include "DrawManager.h"
#include "TextureManager.h"
#include "HighscoreManager.h"

//GUI elements
#include "GUI_Button.h"
#include "GUI_Label.h"

MenuState::MenuState()
{
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	m_drawManager = ServiceLocator<DrawManager>::GetService();
	m_highscoreManager = ServiceLocator<HighscoreManager>::GetService();
	m_nextState = STATE_INVALID;

	sf::Texture* texture = m_textureManager->LoadTexture("assets/sprites/sign_spritesheet.png");
	sf::Font* font = m_textureManager->LoadFont("assets/fonts/menu.ttf");
	m_highscoreManager->SetFont(font);
	m_highscoreManager->SetPosition(135, 130);

	m_poleSprite.setTexture(*texture);
	m_poleSprite.setTextureRect(sf::IntRect(0, 305, 116, 539));
	m_poleSprite.setPosition(ScreenWidth / 2 - 150, ScreenHeight - 600);


	m_playButton = new GUI_Button(m_poleSprite.getPosition().x + 120, m_poleSprite.getPosition().y + 100, nullptr, texture, sf::IntRect(0, 0, 296, 140));
	m_buttons.push_back(m_playButton);
	m_highscoreButton = new GUI_Button(m_poleSprite.getPosition().x + 75, m_poleSprite.getPosition().y + 250, nullptr, texture, sf::IntRect(312, 0, 272, 120));
	m_buttons.push_back(m_highscoreButton);
	m_exitButton = new GUI_Button(m_poleSprite.getPosition().x + 50, m_poleSprite.getPosition().y + 400, nullptr, texture, sf::IntRect(637, 0, 238, 132));
	m_buttons.push_back(m_exitButton);

	texture = m_textureManager->LoadTexture("assets/sprites/background_menu.png");
	m_backgroundSprite.setTexture(*texture);

	//Highscore sign
	texture = m_textureManager->LoadTexture("assets/sprites/score_screen.png");
	m_highscoreSprite.setTexture(*texture);
	m_highscoreSprite.setPosition(50, 0);
}
MenuState::~MenuState()
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
bool MenuState::Update(float deltaTime)
{
	for (unsigned int i = 0; i < m_buttons.size(); i++)
	{
		m_buttons[i]->Update();
	}

	if (m_playButton->IsPressed())
	{
		m_nextState = STATE_GAME;
		return false;
	}
	else if (m_highscoreButton->IsPressed())
	{
		m_nextState = STATE_INSTRUCTION;
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

	for (unsigned int i = 0; i < m_buttons.size(); i++)
	{
		m_buttons[i]->Draw(m_drawManager);
	}

	m_drawManager->Draw(m_highscoreSprite, sf::RenderStates::Default);
	m_highscoreManager->Draw(m_drawManager);
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