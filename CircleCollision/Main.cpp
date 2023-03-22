#include "Collision/CircleCollision.h"

#include "Collision/BruteForce.h"
#include "Collision/SpacePartition.h"

#include "Math/Constants.h"

#include <vector>
#include <iostream>
#include <thread>

#include <chrono>

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

uint64_t GetTime()
{
	return (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
}

CollisionResolutionInfo DoCollisions(std::vector<CircleCollider>* circleColliders, CollisionType collisionType ,bool fullResolve)
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

int main()
{
	unsigned int numberOfSupportedThreads = std::thread::hardware_concurrency();
	std::cout << numberOfSupportedThreads << " concurrent threads are supported.\n\n";

	std::vector<CircleCollider> circleColliders;

	for (int y = 0; y < 100; y++)
	{
		for (int x = 0; x < 100; x++)
		{
			CircleCollider c;
			c.position = Vector2(x / 2.0f, y / 2.0f);
			c.radius = 1;
			circleColliders.push_back(c);
		}
	}

	std::cout << "Colliders: " << circleColliders.size() << std::endl;
	
	uint64_t startTime = GetTime();
	
	bool fully_resolve_collisions_this_frame = false;
	CollisionType collisionType = CollisionType::SpacePartition;
	CollisionResolutionInfo resolutionInfo = DoCollisions(&circleColliders, collisionType ,fully_resolve_collisions_this_frame);

	uint64_t endTime = GetTime();
	uint64_t totalTime = endTime - startTime;

	std::cout << "Collision Type: " << collisionType << std::endl;
	std::cout << "Collisions Resolved: " << resolutionInfo.collisionsResolved << std::endl;
	std::cout << "Iterations: " << std::to_string(resolutionInfo.iterations) << std::endl;
	std::cout << "Time in nanosecond:" << totalTime << std::endl;
	std::cout << "Time in miliseconds:" << totalTime / 1000000.0f << std::endl;
	std::cout << "Time in seconds:" << totalTime / 1000000000.0f << std::endl;

	return 0;
}