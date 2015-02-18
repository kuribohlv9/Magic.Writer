#pragma once

class GameObject;

class Collider
{
public:
	Collider(int xOffset, int yOffset);

	bool HasParent();
	void SetParent(GameObject* parent);
	GameObject* GetParent();

	void SetPosition(float x, float y);
	void SetWidthHeight(int width, int height);

	sf::FloatRect GetArea();

	void Refresh();

private:
	GameObject* m_parent;
	sf::FloatRect m_area;
	int m_xOffset;
	int m_yOffset;
};