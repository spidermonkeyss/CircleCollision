#pragma once

#include "CircleCollision.h"
#include "CircleCollider.h"
#include "../Math/Vector2.h"
#include "../Math/Constants.h"

#include <vector>
#include <iostream>

void BruteForceCollisionCheck(std::vector<CircleCollider>* circleColliders, std::vector<CircleCollision>* circleCollisions)
{
	float collisionDistance = 0;
	float distance = 0;
	float penDepth = 0;

	for (int i = 0; i < circleColliders->size(); i++)
	{
		for (int k = i + 1; k < circleColliders->size(); k++)
		{
			collisionDistance = (circleColliders->at(i).radius + circleColliders->at(k).radius) / 2.0f;
			distance = Vector2::Distance(circleColliders->at(i).position, circleColliders->at(k).position);
			penDepth = collisionDistance - distance;

			if (penDepth >= MINIMUM_PENETRATION_DEPTH)
			{
				CircleCollision collision;
				collision.penetrationDepth = penDepth;
				collision.collider = &circleColliders->at(i);
				collision.otherCollider = &circleColliders->at(k);
				circleCollisions->push_back(collision);
			}
		}
	}
}