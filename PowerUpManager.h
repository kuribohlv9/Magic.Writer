#pragma once

class InputManager;
class Monster;
class Item;

class PowerUpManager
{
public:
	PowerUpManager(std::vector<Monster*>* monster, std::vector<Item*>* item);
	~PowerUpManager();

	void Update(float deltaTime);

	void ActivateFreeze();
	void ActivateBounce();
	void ActivatePierce(int pierce);

	bool GetFrozen();
	bool GetPierce();

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
};