#pragma once

class InputManager;
class Monster;
class Item;
class Player;
class TextureManager;
class DrawManager;
class ParticleEmitter;

class PowerManager
{
public:
	PowerManager(std::vector<Monster*>* monster, std::vector<Item*>* item, Player* player);
	~PowerManager();

	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);

	void ActivateFreeze();
	void ActivateBounce();
	void ActivatePierce();

	bool GetFrozen();
	bool GetPierce();
	bool GetBounce();

	Item* BounceItem();
	Monster* NextBounceTarget();

	bool NextBounce(Monster* monster);
	bool AddItemToPierceList(Monster* monster);

	sf::Vector2f ItemDirection();

	void RemoveFreeze();

private:
	bool m_frozen;
	bool m_bounce;
	bool m_pierce;

	float m_freezeTimer;

private:
	TextureManager* m_textureManager;
	InputManager* m_inputManager;
	Item* m_pierceCurrentItem;
	Item* m_bounceCurrentItem;
	Monster* m_nextBounceTarget;
	Player* m_player;

	sf::Sprite m_frameSprite;
	sf::Sprite m_fillSprite;
	float m_powerupScore;
	float m_targetScore;
	ParticleEmitter* m_emitter;

	std::vector<Monster*> m_monsterPierceList;
	std::vector<Monster*>* m_monster;
	std::vector<Item*>* m_item;
	std::vector<int> m_LaneBounceList;

	sf::Vector2f m_monsterPos;
	sf::Vector2f m_itemPos;
	sf::Vector2f m_itemDir;
};