#pragma once
class Collider;

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	static bool Check(Collider* lhs, Collider* rhs, sf::IntRect& intersect);
};