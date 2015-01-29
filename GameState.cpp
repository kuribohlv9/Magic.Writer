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
	sf::Texture* texture = m_textureManager->LoadTexture("assets/sprites/object.png");
	m_objectOne = new Monster(texture, 300, 250);

	texture = m_textureManager->LoadTexture("assets/sprites/object.png");
	m_objectTwo = new Monster(texture, 500, 250);
	m_objectTwo->GetSprite()->setColor(sf::Color::Red);

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
	float speed = 0.1f;
	if (m_inputManager->IsKeyDown(sf::Keyboard::A))
	{
		m_objectTwo->Move(-speed, 0);
	}
	if (m_inputManager->IsKeyDown(sf::Keyboard::D))
	{
		m_objectTwo->Move(speed, 0);
	}

	if (m_inputManager->IsKeyDown(sf::Keyboard::W))
	{
		m_objectTwo->Move(0, -speed);
	}
	if (m_inputManager->IsKeyDown(sf::Keyboard::S))
	{
		m_objectTwo->Move(0, speed);
	}

	//m_objectTwo->SetPosition(m_inputManager->GetMousePosition().x - m_objectTwo->GetSprite()->getGlobalBounds().width / 2, m_inputManager->GetMousePosition().y - m_objectTwo->GetSprite()->getGlobalBounds().height / 2);

	sf::IntRect intersect;
	if (CollisionManager::Check(m_objectTwo->GetCollider(), m_objectOne->GetCollider(), intersect))
	{
		m_text.setString(
			"X: " + std::to_string(intersect.left)
			+ "\nY: " + std::to_string(intersect.top)
			+ "\nW: " + std::to_string(intersect.width)
			+ "\nH: " + std::to_string(intersect.height));

		m_objectTwo->Move(intersect.width, intersect.height);
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