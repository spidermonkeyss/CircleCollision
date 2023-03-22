#pragma once

#include "CircleCollider.h"

struct CircleCollision 
{
	float penetrationDepth;
	CircleCollider* collider;
	CircleCollider* otherCollider;
};