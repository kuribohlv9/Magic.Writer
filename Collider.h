#pragma once

class GameObject;

class Collider
{
public:
	Collider(int x, int y);

	bool HasParent();
	void SetParent(GameObject* parent);
	GameObject* GetParent();

	void SetPosition(int x, int y);
	void SetWidthHeight(int width, int height);

	sf::IntRect GetArea();

	void Refresh();

private:
	GameObject* m_parent;
	sf::IntRect m_area;
};