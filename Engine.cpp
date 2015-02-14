#include "stdafx.h"
#include "Engine.h"
#include "DrawManager.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "CollisionManager.h"
#include "AudioManager.h"
#include "StateManager.h"
#include "WordManager.h"
#include "ServiceLocator.h"

Engine::Engine()
{
	m_running = false;
	m_draw_manager = nullptr;
	m_input_manager = nullptr;
	m_texture_manager = nullptr;
	m_audio_manager = nullptr;
	m_state_manager = nullptr;
}

Engine::~Engine()
{
}

bool Engine::Initialize()
{
	m_window.create(sf::VideoMode(1920, 1080), "Magic Writer");
	m_window.setFramerateLimit(60);
	if (!m_window.isOpen())
		return false;

	m_draw_manager = new DrawManager(&m_window);
	if (!m_draw_manager || !m_draw_manager->Initialize())
		return false;
	ServiceLocator<DrawManager>::SetService(m_draw_manager);

	m_input_manager = new InputManager();
	if (!m_input_manager || !m_input_manager->Initialize())
		return false;
	ServiceLocator<InputManager>::SetService(m_input_manager);

	m_texture_manager = new TextureManager();
	if (!m_texture_manager || !m_texture_manager->Initialize())
		return false;
	ServiceLocator<TextureManager>::SetService(m_texture_manager);

	m_audio_manager = new AudioManager();
	if (!m_audio_manager || !m_audio_manager->Initialize())
		return false;
	ServiceLocator<AudioManager>::SetService(m_audio_manager);

	m_state_manager = new StateManager;
	if (!m_state_manager || !m_state_manager->Initialize())
		return false;

	return m_running = true;
}

void Engine::Shutdown()
{
	// note(tommi): here we delete the managers in the reverse order 
	// because of potential dependencies 
	if (m_state_manager)
	{
		m_state_manager->Shutdown();
		delete m_state_manager;
		m_state_manager = nullptr;
	}

	if (m_audio_manager)
	{
		m_audio_manager->Shutdown();
		delete m_audio_manager;
		m_audio_manager = nullptr;
	}

	if (m_texture_manager)
	{
		m_texture_manager->Shutdown();
		delete m_texture_manager;
		m_texture_manager = nullptr;
	}

	if (m_input_manager)
	{
		m_input_manager->Shutdown();
		delete m_input_manager;
		m_input_manager = nullptr;
	}

	if (m_draw_manager)
	{
		m_draw_manager->Shutdown();
		delete m_draw_manager;
		m_draw_manager = nullptr;
	}
}

void Engine::Run()
{
	sf::Clock clock;
	while (m_running)
	{
		sf::Time time = clock.restart();
		float deltaTime = time.asSeconds();
		HandleEvents();

		if (!m_state_manager->Update(deltaTime))
		{
			m_running = false;
		}

		m_window.clear(sf::Color(50, 50, 50, 255));
		m_state_manager->Draw();
		m_window.display();
	}
	m_window.close();
}

// private
void Engine::HandleEvents()
{
	// note(tommi): we want to take care of more events
	// but we only care about clearing the queue for now

	sf::Event event;
	m_input_manager->SetLastInputs();
	m_input_manager->SetInputChar(' ');
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			m_input_manager->SetKey(event.key.code, true);
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			m_input_manager->SetKey(event.key.code, false);
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			m_input_manager->SetButton(event.mouseButton.button, true);
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			m_input_manager->SetButton(event.mouseButton.button, false);
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			m_input_manager->SetMousePosition(event.mouseMove.x, event.mouseMove.y);
		}
		else if (event.type == sf::Event::TextEntered)
		{
			if (event.text.unicode >= 97 && event.text.unicode <= 122)
			{
				m_input_manager->SetInputChar(static_cast<char>(event.text.unicode));
			}
		}
	}
}