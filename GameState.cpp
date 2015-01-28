#include "stdafx.h"
#include "GameState.h"

#include "ServiceLocator.h"
#include "DrawManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "CollisionManager.h"

#include "Monster.h"
#include "Collider.h"

GameState::GameState()
{
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	m_drawManager = ServiceLocator<DrawManager>::GetService();
	m_inputManager = ServiceLocator<InputManager >::GetService();

	//TMP
	sf::Texture* texture = m_textureManager->LoadTexture("assets/sprites/ball_small.png");
	m_objectOne = new Monster(texture, 50, 250);

	texture = m_textureManager->LoadTexture("assets/sprites/brick_small.png");
	m_objectTwo = new Monster(texture, 400, 250);

	m_text.setFont(*m_textureManager->LoadFont("assets/fonts/font.ttf"));
	m_text.setPosition(100, 100);
	m_text.setColor(sf::Color::White);
}
GameState::~GameState()
{
	m_textureManager = nullptr;
	m_drawManager = nullptr;
}
bool GameState::Update(float deltaTime)
{
	Collider* collider = new Collider(0, 0);

	collider->SetWidthHeight(1, 1);
	collider->SetPosition(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);

	sf::IntRect intersect;
	if (CollisionManager::Check(collider, m_objectOne->GetCollider(), intersect))
	{
		m_text.setString("Collision: True");
	}
	else
	{
		m_text.setString("Collision: False");
	}
	//TMP
	return true;
}
void GameState::Draw()
{
	//TMP
	m_objectOne->Draw(m_drawManager);
	m_objectTwo->Draw(m_drawManager);
	m_drawManager->Draw(m_text, sf::RenderStates::Default);
}

void GameState::Enter()
{

}
void GameState::Exit()
{

}

ScreenState GameState::NextState()
{
	return STATE_MENU;
}