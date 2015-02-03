#include "stdafx.h"
#include "GameState.h"

#include "ServiceLocator.h"
#include "DrawManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "WordManager.h"
#include "ItemManager.h"

#include "Monster.h"
#include "Collider.h"

GameState::GameState()
{
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	m_drawManager = ServiceLocator<DrawManager>::GetService();
	m_inputManager = ServiceLocator<InputManager >::GetService();

	m_wordManager = new WordManager();
	m_itemManager = new ItemManager();
}
GameState::~GameState()
{
	m_textureManager = nullptr;
	m_drawManager = nullptr;
}
bool GameState::Update(float deltaTime)
{
	m_wordManager->Update(deltaTime);
	return true;
}
void GameState::Draw()
{
	m_wordManager->Draw(m_drawManager);
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