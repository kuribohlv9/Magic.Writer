#include "stdafx.h"
#include "GameState.h"

#include "ServiceLocator.h"
#include "DrawManager.h"
#include "TextureManager.h"

GameState::GameState()
{
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	m_drawManager = ServiceLocator<DrawManager>::GetService();

	//TMP
	m_sprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/ball_small.png"));
}
GameState::~GameState()
{
	m_textureManager = nullptr;
	m_drawManager = nullptr;
}
bool GameState::Update(float deltaTime)
{
	//TMP
	m_sprite.move(1, 0);

	return true;
}
void GameState::Draw()
{
	//TMP
	m_drawManager->Draw(m_sprite, sf::RenderStates::Default);
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