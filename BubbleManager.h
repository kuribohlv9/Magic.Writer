#pragma once

class TextureManager;
class DrawManager;
class WordManager;
class Player;
class Bubble;
class SmallBubble;

class BubbleManager
{
public:
	BubbleManager(Player* player, WordManager* wordManager);
	~BubbleManager();

	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);
	Bubble* GetBubble(unsigned int index);

private:
	TextureManager* m_textureManager;
	WordManager* m_wordManager;
	std::vector<SmallBubble*> m_smallBubbles;
	std::vector<Bubble*> m_bubbles;
	Player* m_player;
};

