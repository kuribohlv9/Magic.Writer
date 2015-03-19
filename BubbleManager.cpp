#include "stdafx.h"
#include "BubbleManager.h"
#include "TextureManager.h"
#include "WordManager.h"
#include "SmallBubble.h"
#include "Bubble.h"
#include "ServiceLocator.h"
#include "Player.h"


BubbleManager::BubbleManager(Player* player, WordManager* wordManager)
{
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	m_wordManager = wordManager;
	m_player = player;

	//Instantiate bubbles
	sf::Texture* texture = m_textureManager->LoadTexture("assets/sprites/bubbles_spritesheet.png");
	for (int i = 0; i < 3; i++)
	{
		int yOffset = 0;
		if (i == 1)
			yOffset = 50;
		Bubble* bubble = new Bubble(725.0f + i * 200.0f, 910.0f + yOffset, texture, m_player);

		m_bubbles.push_back(bubble);
	}

	//Instantiate small bubles
	texture = m_textureManager->LoadTexture("assets/sprites/small_bubble.png");
	int bubbleIndex = 0;
	float percentage = 0.3f;
	for (unsigned int i = 0; i < 6; i++)
	{
		bubbleIndex++;
		if (bubbleIndex > 2)
			bubbleIndex = 0;
		if (i > 2)
		{
			percentage = 0.5f;
		}
		m_smallBubbles.push_back(new SmallBubble(texture, m_player, m_bubbles[bubbleIndex], percentage));
	}
}
BubbleManager::~BubbleManager()
{
	//Delete small bubbles
	auto it = m_smallBubbles.begin();

	while (it != m_smallBubbles.end())
	{
		delete *it;
		++it;
	}
	m_smallBubbles.clear();

	//Delete bubbles
	auto itr = m_bubbles.begin();

	while (itr != m_bubbles.end())
	{
		delete *itr;
		++itr;
	}
	m_bubbles.clear();
}

void BubbleManager::Update(float deltaTime)
{
	//Update bubbles
	for (int i = 0; i < m_bubbles.size(); i++)
	{
		m_bubbles[i]->Update(deltaTime);
		m_wordManager->SetWordPosition(m_bubbles[i]->GetPosition(), i);

		bool active = m_wordManager->GetActiveBubbles().at(i);
		int alpha = (active) ? 255 : 80;
		m_bubbles[i]->SetAlpha(alpha);
	}

	//Update small bubbles
	for (unsigned int i = 0; i < m_smallBubbles.size(); i++)
	{
		if (!m_smallBubbles[i]->IsActive())
			continue;
		m_smallBubbles[i]->Update(deltaTime);
	}
}
void BubbleManager::Draw(DrawManager* drawManager)
{
	//Draw small bubbles
	for (unsigned int i = 0; i < m_smallBubbles.size(); i++)
	{
		if (!m_smallBubbles[i]->IsActive())
			continue;

		m_smallBubbles[i]->Draw(drawManager);
	}

	//Draw bubbles
	for (unsigned int i = 0; i < m_bubbles.size(); i++)
	{
		m_bubbles[i]->Draw(drawManager);
	}
}
Bubble* BubbleManager::GetBubble(unsigned int index)
{
	if (index < 0 || index > m_bubbles.size() - 1)
		return nullptr;

	return m_bubbles[index];
}
