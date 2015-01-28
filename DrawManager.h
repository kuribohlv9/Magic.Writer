#pragma once

class DrawManager
{
public:
	DrawManager(sf::RenderWindow* window);
	~DrawManager();

	bool Initialize();
	void Shutdown();

	void Draw(const sf::Drawable& drawable, const sf::RenderStates& states);

private:
	sf::RenderWindow* m_window;
};