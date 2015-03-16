#pragma once

class InputManager;
class Monster;
class Item;
class Player;
class TextureManager;
class DrawManager;

class PowerManager
{
public:
	PowerManager(std::vector<Monster*>* monsters, std::vector<Item*>* item, Player* player);
	~PowerManager();

	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);

	//Bounce methods
	void ActivateBounce();
	bool SetNewBounceTarget(Monster* monster);
	Item* GetBounceItem();
	Monster* GetBounceTarget();
	sf::Vector2f GetItemDirection();

	//Freeze methods
	void ActivateFreeze();
	void RemoveFreeze();
	bool IsFrozen();

	//Pierce methods
	void ActivatePierce();
	Item* GetPierceItem();
	bool AddItemToPierceList(Monster* monster);

private:
	void UpdatePowerBar();

private:
	bool m_frozen;
	float m_freezeTimer;
	float m_freezeTime;

private:
	TextureManager* m_textureManager;
	InputManager* m_inputManager;
	Item* m_pierceCurrentItem;
	Item* m_bounceCurrentItem;
	Monster* m_bounceTarget;
	Player* m_player;

	//Powerbar
	sf::Sprite m_pierceSprite;
	sf::Sprite m_bounceSprite;
	sf::Sprite m_freezeSprite;
	sf::Sprite m_frameSprite;
	sf::Sprite m_fillSprite;
	sf::Color m_fadeColor;
	float m_powerupScore;
	float m_fillSpeed;
	float m_stepSize;

	std::vector<Monster*> m_monsterPierceList;
	std::vector<Monster*>* m_monsters;
	std::vector<Item*>* m_activeItems;
	std::vector<int> m_LaneBounceList;
};