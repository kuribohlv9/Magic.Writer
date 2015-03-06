#include "stdafx.h"
#include "TestState.h"
#include "ServiceLocator.h"
#include "DrawManager.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"

TestState::TestState()
{
	m_drawManager = ServiceLocator<DrawManager>::GetService();
	m_inputManager = ServiceLocator<InputManager>::GetService();
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	m_particleManager = ServiceLocator<ParticleManager>::GetService();

	/*sf::Texture* particle = m_textureManager->LoadTexture("assets/sprites/test_particle.png");
	m_emitter = m_particleManager->CreateEmitter(particle, 200);
	m_emitter->SetForce(100, 200);
	m_emitter->SetSpawnRate(0);*/
}
TestState::~TestState()
{

}

bool TestState::Update(float deltaTime)
{
	if (rand() % 2 == 0)
		m_emitter->SetAcceleration(5, 1);
	else
		m_emitter->SetAcceleration(-5, 1);
	m_particleManager->Update(deltaTime);

	sf::Vector2f mousePos = m_inputManager->GetMousePosition();

	m_emitter->SetLifeTime(0.5f, 0.5f);
	m_emitter->SetForce(500, 700);
	m_emitter->SetStartAngle(260, 280);
	m_emitter->SetPosition(mousePos.x, mousePos.y);
	return true;
}
void TestState::Draw()
{
	m_particleManager->Draw(m_drawManager);
}

void TestState::Enter()
{

}
void TestState::Exit()
{

}

ScreenState TestState::NextState()
{
	return STATE_INVALID;
}