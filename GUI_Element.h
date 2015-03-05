#pragma once

class DrawManager;

class GUI_Element
{
public:
	GUI_Element();
	virtual ~GUI_Element();

	int GetX();
	int GetY();
	bool IsVisible();
	void SetVisible(bool state);
	void SetPosition(float x, float y);

	virtual void SetParent(GUI_Element* parent);
	virtual void Refresh();
	virtual void Draw(DrawManager* drawManager);
	virtual void Update();

protected:
	float m_x;
	float m_y;
	float m_localX;
	float m_localY;
	bool m_visible;

	GUI_Element* m_parent;
};