#include "stdafx.h"
#include "CollisionManager.h"
#include "Collider.h"

bool CollisionManager::Check(Collider* lhs, Collider* rhs)
{
	sf::IntRect areaOne = lhs->GetArea();
	sf::IntRect areaTwo = rhs->GetArea();

	if (areaOne.intersects(areaTwo))
	{
		return true;
	}
	return false;
}
bool CollisionManager::Check(Collider* lhs, Collider* rhs, sf::IntRect& intersect)
{
	sf::IntRect areaOne = lhs->GetArea();
	sf::IntRect areaTwo = rhs->GetArea();

	if (areaOne.intersects(areaTwo, intersect))
	{
		sf::Vector2f areaOneCenter;
		areaOneCenter.x = areaOne.left + areaOne.width / 2;
		areaOneCenter.y = areaOne.top + areaOne.height / 2;

		if (abs(intersect.width) < abs(intersect.height))
		{
			if (areaTwo.left + areaTwo.width >= areaOneCenter.x)
				intersect.width *= -1;
			intersect.height = 0;
		}
		else
		{
			if (areaTwo.top + areaTwo.height >= areaOneCenter.y)
				intersect.height *= -1;
			intersect.width = 0;
		}

		return true;
	}
	return false;
}

