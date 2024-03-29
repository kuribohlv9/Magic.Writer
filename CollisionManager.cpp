#include "stdafx.h"
#include "CollisionManager.h"
#include "Collider.h"

bool CollisionManager::Check(Collider* lhs, Collider* rhs)
{
	//Checks if the hitboxes inside the colliders intersect and returns the answer
	sf::FloatRect areaOne = lhs->GetArea();
	sf::FloatRect areaTwo = rhs->GetArea();

	if (areaOne.intersects(areaTwo))
	{
		return true;
	}
	return false;
}
bool CollisionManager::Check(Collider* lhs, Collider* rhs, sf::FloatRect& intersect)
{
	//Checks if the hitboxes inside the colliders intersect and returns the answer
	//This functions also returns the amount of intersection in width and height via the sf::FloatRect reference
	sf::FloatRect areaOne = lhs->GetArea();
	sf::FloatRect areaTwo = rhs->GetArea();

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

