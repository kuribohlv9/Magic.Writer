#include "stdafx.h"
#include "CollisionManager.h"
#include "Collider.h"

bool CollisionManager::Check(Collider* lhs, Collider* rhs, sf::IntRect& intersect)
{
	sf::IntRect areaOne = lhs->GetArea();
	sf::IntRect areaTwo = rhs->GetArea();

	if (areaOne.intersects(areaTwo))
	{
		return true;
	}
	return false;
}

