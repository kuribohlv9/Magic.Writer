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
	void SetPosition(int x, int y);

	virtual void SetParent(GUI_Element* parent);
	virtual void Refresh();
	virtual void Draw(DrawManager* drawManager);
	virtual void Update();

protected:
	int m_x;
	int m_y;
	int m_localX;
	int m_localY;
	bool m_visible;

	GUI_Element* m_parent;
};