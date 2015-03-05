
class DrawManager;
class InputManager;
class StateManager;
class TextureManager;
class CollisionManager;
class AudioManager;
class HighscoreManager;
class ParticleManager;

class Engine
{
public:
	Engine();
	~Engine();

	bool Initialize();
	void Shutdown();
	void Run();

private:
	void HandleEvents();

private:
	bool m_running;
	sf::RenderWindow m_window;
	DrawManager* m_draw_manager;
	InputManager* m_input_manager;
	TextureManager* m_texture_manager;
	CollisionManager* m_collision_manager;
	AudioManager* m_audio_manager;
	StateManager* m_state_manager;
	HighscoreManager* m_scores_manager;
	ParticleManager* m_particle_manager;
};