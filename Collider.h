#pragma once

class GameObjects;

class Collider
{
public:
	Collider(int x, int y);

	bool HasParent();
	void SetParent(GameObjects* parent);
	GameObjects* GetParent();

	void SetPosition(int x, int y);
	void SetWidthHeight(int width, int height);

	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();

	void Refresh();

private:
	GameObjects* m_parent;
	sf::IntRect m_area;
};