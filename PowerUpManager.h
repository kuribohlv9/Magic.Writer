#pragma once

class InputManager;
class Monster;
class Item;
class Player;

class PowerUpManager
{
public:
	PowerUpManager(std::vector<Monster*>* monster, std::vector<Item*>* item, Player* player);
	~PowerUpManager();

	void Update(float deltaTime);

	void ActivateFreeze();
	void ActivateBounce();
	void ActivatePierce();

	bool GetFrozen();
	bool GetPierce();

	bool AddItemToPierceList(Monster* monster);

	void RemoveFreeze();

private:
	bool m_frozen;
	bool m_bounce;
	bool m_pierce;

	float m_freezeTimer;

private:
	InputManager* m_inputManager;
	std::vector<Monster*>* m_monster;
	std::vector<Item*>* m_item;
	Item* m_pierceCurrentItem;
	Item* m_bounceCurrentItem;
	Player* m_player;
	std::vector<Monster*> m_monsterPierceList;
};