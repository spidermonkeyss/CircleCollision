/*
	The order of collision resolution is different for the two types
	So given the same starting parameters, they will have different outcomes
*/
#include "Collision/Collision.h"

#include <iostream>

#include <chrono>

uint64_t GetTime()
{
	return (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
}

int main()
{
	unsigned int numberOfSupportedThreads = std::thread::hardware_concurrency();
	std::cout << numberOfSupportedThreads << " concurrent threads are supported.\n\n";

	std::vector<CircleCollider> circleColliders;

	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
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

	CollisionResolutionInfo resolutionInfo = DoCollisions(&circleColliders, collisionType, fully_resolve_collisions_this_frame);

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
