#pragma once

#include "CircleCollision.h"
#include "CircleCollider.h"
#include "../Math/Vector2.h"
#include "../Math/Constants.h"

#include <vector>
#include <iostream>


struct GridCell
{
	std::vector<CircleCollider*> colliders;
	bool isChecked = false;
	Vector2 gridPosition;
};

struct Grid
{
	int stride;
	int height;
	Vector2 start;
	Vector2 end;
	std::vector<GridCell> gridCells;

	Grid(Vector2 _start, Vector2 _end)
	{
		start.x = std::floor(_start.x);
		start.y = std::floor(_start.y);
		end.x = std::floor(_end.x);
		end.y = std::floor(_end.y);

		//Create grid with empty cells
		for (int y = start.y; y <= end.y; y++)
		{
			for (int x = start.x; x <= end.x; x++)
			{
				GridCell gridCell;
				gridCell.isChecked = false;
				gridCell.gridPosition = Vector2(x, y);
				gridCells.push_back(gridCell);
			}
		}

		stride = end.x - start.x + 1;
		height = end.y - start.y + 1;
	}

	GridCell* GetCell(int x, int y)
	{
		x -= start.x;
		y -= start.y;
		return &gridCells.at((y * stride) + x);
	}
};

void BroadPhase(Grid* grid, std::vector<CircleCollider>* circleColliders)
{
	//Loop through all colliders and put them on the grid
	for (int i = 0; i < circleColliders->size(); i++)
	{
		int x = std::floor(circleColliders->at(i).position.x);
		int y = std::floor(circleColliders->at(i).position.y);
		grid->GetCell(x, y)->colliders.push_back(&circleColliders->at(i));
	}
}

void NarrowPhase(Grid* grid, std::vector<CircleCollision>* circleCollisions)
{
	float collisionDistance = 0;
	float distance = 0;
	float penDepth = 0;

	//Loop over each cell
	for (int cellIndex = 0; cellIndex < grid->gridCells.size(); cellIndex++)
	{
		GridCell* cell = &grid->gridCells.at(cellIndex);
		cell->isChecked = true;

		//Check if cell is empty
		if (cell->colliders.size() > 0)
		{
			//Loop over adjacent cells
			for (int outerCellY = cell->gridPosition.y - 1; outerCellY <= cell->gridPosition.y + 1; outerCellY++)
			{
				for (int outerCellX = cell->gridPosition.x - 1; outerCellX <= cell->gridPosition.x + 1; outerCellX++)
				{
					//Cell can't be outside grid
					if (outerCellX >= grid->start.x && outerCellY >= grid->start.y && outerCellX <= grid->end.x && outerCellY <= grid->end.y)
					{
						GridCell* otherCell = grid->GetCell(outerCellX, outerCellY);

						//If other cell is center cell
						if (otherCell->gridPosition == cell->gridPosition)
						{
							for (int colliderIndex = 0; colliderIndex < cell->colliders.size() - 1; colliderIndex++)
							{
								CircleCollider* colliderOne = cell->colliders.at(colliderIndex);

								for (int colliderTwoIndex = colliderIndex + 1; colliderTwoIndex < cell->colliders.size(); colliderTwoIndex++)
								{
									CircleCollider* colliderTwo = otherCell->colliders.at(colliderTwoIndex);

									//Create collision
									collisionDistance = (colliderOne->radius + colliderTwo->radius) / 2.0f;
									distance = Vector2::Distance(colliderOne->position, colliderTwo->position);
									penDepth = collisionDistance - distance;

									if (penDepth >= MINIMUM_PENETRATION_DEPTH)
									{
										CircleCollision collision;
										collision.penetrationDepth = penDepth;
										collision.collider = colliderOne;
										collision.otherCollider = colliderTwo;
										circleCollisions->push_back(collision);
									}
								}
							}
						}
						//Check if cell is empty
						else if (otherCell->colliders.size() > 0)
						{
							//If the cell hasn't already done it's own collision checks, then do checks on it
							if (!otherCell->isChecked)
							{
								//Loop over center cell colliders
								for (int centerCellColliderIndex = 0; centerCellColliderIndex < cell->colliders.size(); centerCellColliderIndex++)
								{
									CircleCollider* colliderOne = cell->colliders.at(centerCellColliderIndex);
									//Loop over outer cell colliders
									for (int outerCellColliderIndex = 0; outerCellColliderIndex < otherCell->colliders.size(); outerCellColliderIndex++)
									{
										CircleCollider* colliderTwo = otherCell->colliders.at(outerCellColliderIndex);

										//Create collision
										collisionDistance = (colliderOne->radius + colliderTwo->radius) / 2.0f;
										distance = Vector2::Distance(colliderOne->position, colliderTwo->position);
										penDepth = collisionDistance - distance;

										if (penDepth >= MINIMUM_PENETRATION_DEPTH)
										{
											CircleCollision collision;
											collision.penetrationDepth = penDepth;
											collision.collider = colliderOne;
											collision.otherCollider = colliderTwo;
											circleCollisions->push_back(collision);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void SpacePartitionCollisionCheck(std::vector<CircleCollider>* circleColliders, std::vector<CircleCollision>* circleCollisions)
{
	//The grids size will be set by the smallest and largest positions of the colliders
	Vector2 start = circleColliders->at(0).position;
	Vector2 end = circleColliders->at(0).position;

	//Get grid size
	for (int i = 0; i < circleColliders->size(); i++)
	{
		if (circleColliders->at(i).position.x < start.x)
			start.x = circleColliders->at(i).position.x;
		if (circleColliders->at(i).position.y < start.y)
			start.y = circleColliders->at(i).position.y;
		if (circleColliders->at(i).position.x > end.x)
			end.x = circleColliders->at(i).position.x;
		if (circleColliders->at(i).position.y > end.y)
			end.y = circleColliders->at(i).position.y;
	}

	Grid grid(start, end);

	BroadPhase(&grid, circleColliders);
	NarrowPhase(&grid, circleCollisions);
}