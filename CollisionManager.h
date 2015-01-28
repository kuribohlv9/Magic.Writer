#pragma once
class Collider;

class CollisionManager
{
public:
	static bool Check(Collider* lhs, Collider* rhs, sf::IntRect& intersect);
	
};