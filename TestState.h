#pragma once

#include "State.h"

class InputManager;
class DrawManager;
class TextureManager;
class ParticleEmitter;
class ParticleManager;

class TestState : public State
{
public:
	TestState();
	~TestState();

	bool Update(float deltaTime);
	void Draw();

	void Enter();
	void Exit();

	virtual ScreenState NextState();

private:
	InputManager* m_inputManager;
	DrawManager* m_drawManager;
	TextureManager* m_textureManager;

	ParticleEmitter* m_emitter;
	ParticleManager* m_particleManager;
};