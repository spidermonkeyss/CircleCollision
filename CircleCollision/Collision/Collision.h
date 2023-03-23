#pragma once
#include "CircleCollision.h"

#include "BruteForce.h"
#include "SpacePartition.h"

#include "../Math/Constants.h"

#include <vector>

enum CollisionType { BruteForce, SpacePartition };

struct CollisionResolutionInfo
{
	unsigned int iterations;
	unsigned int collisionsResolved;
};

void ResolveCircleCollisions(std::vector<CircleCollision>* circleCollisions)
{
	float pushLength = 0;
	Vector2 collisionDir;
	for (int i = 0; i < circleCollisions->size(); i++)
	{
		pushLength = circleCollisions->at(i).penetrationDepth / 2.0f;
		if (pushLength < MINIMUM_PENETRATION_DEPTH)
			pushLength = MINIMUM_PENETRATION_DEPTH;
		collisionDir = circleCollisions->at(i).collider->position - circleCollisions->at(i).otherCollider->position;
		circleCollisions->at(i).collider->position += collisionDir.Normal() * pushLength;
		circleCollisions->at(i).otherCollider->position -= collisionDir.Normal() * pushLength;
	}
}

CollisionResolutionInfo DoCollisions(std::vector<CircleCollider>* circleColliders, CollisionType collisionType, bool fullResolve)
{
	CollisionResolutionInfo resolutionInfo;
	resolutionInfo.collisionsResolved = 0;

	std::vector<CircleCollision> circleCollisions;
	circleCollisions.reserve(circleColliders->size());

	unsigned int iterations = 0;
	bool collisionsResolved = false;
	while (!collisionsResolved)
	{
		iterations++;
		circleCollisions.clear();

		switch (collisionType)
		{
		case BruteForce:
			BruteForceCollisionCheck(circleColliders, &circleCollisions);
			break;
		case SpacePartition:
			SpacePartitionCollisionCheck(circleColliders, &circleCollisions);
			break;
		default:
			break;
		}

		ResolveCircleCollisions(&circleCollisions);

		resolutionInfo.collisionsResolved += circleCollisions.size();
		if (fullResolve == false || circleCollisions.size() == 0)
			collisionsResolved = true;
	}

	resolutionInfo.iterations = iterations;
	return resolutionInfo;
}