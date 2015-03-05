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
	sf::Font* font = m_textureManager->LoadFont("assets/fonts/font.ttf");
	m_highscoreManager->SetFont(font);
	m_highscoreManager->SetPosition(300, 100);

	m_poleSprite.setTexture(*texture);
	m_poleSprite.setTextureRect(sf::IntRect(0, 219 * 2, 167, 775));
	m_poleSprite.setPosition(ScreenWidth / 2, ScreenHeight - 775);


	m_playButton = new GUI_Button(m_poleSprite.getPosition().x + 180, m_poleSprite.getPosition().y + 100, nullptr, texture, sf::IntRect(0, 0, 431, 219));
	m_buttons.push_back(m_playButton);
	m_highscoreButton = new GUI_Button(m_poleSprite.getPosition().x + 130, m_poleSprite.getPosition().y + 320, nullptr, texture, sf::IntRect(431, 0, 431, 219));
	m_buttons.push_back(m_highscoreButton);
	m_exitButton = new GUI_Button(m_poleSprite.getPosition().x + 90, m_poleSprite.getPosition().y + 550, nullptr, texture, sf::IntRect(431 * 2, 0, 431, 219));
	m_buttons.push_back(m_exitButton);
	m_helpButton = new GUI_Button(m_poleSprite.getPosition().x - 300, m_poleSprite.getPosition().y + 550, nullptr, texture, sf::IntRect(431 * 2, 0, 431, 219));
	m_buttons.push_back(m_helpButton);
	m_helpExitButton = new GUI_Button(m_poleSprite.getPosition().x - 650, m_poleSprite.getPosition().y + 550, nullptr, texture, sf::IntRect(431 * 2, 0, 431, 219));
	m_buttons.push_back(m_helpExitButton);

	texture = m_textureManager->LoadTexture("assets/sprites/background/background.png");
	m_backgroundSprite.setTexture(*texture);
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
	if (!m_helpScreen)
	{
		for (int i = 0; i < m_buttons.size(); i++)
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
			m_showHighscore = !m_showHighscore;
		}
		else if (m_exitButton->IsPressed())
		{
			m_nextState = STATE_INVALID;
			return false;
		}
		else if (m_helpButton->IsPressed())
		{
			m_helpScreen = true;
		}
		return true;
	}
	else
	{
		m_helpExitButton->Update();

		if (m_helpExitButton->IsPressed())
		{
			m_helpScreen = false;
		}
		return true;
	}
}
void MenuState::Draw()
{
	m_drawManager->Draw(m_backgroundSprite, sf::RenderStates::Default);
	m_drawManager->Draw(m_poleSprite, sf::RenderStates::Default);

	for (int i = 0; i < m_buttons.size(); i++)
	{
		m_buttons[i]->Draw(m_drawManager);
	}

	if (m_helpScreen)
	{
		m_helpExitButton->Draw(m_drawManager);
	}

	if (m_showHighscore)
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