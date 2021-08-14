#include "Body.h"

Body::Body(Vector2 size, Vector2 offset, float bounciness, float friction,Node2D* node,Level* level)
{
	this->size = size;
	this->offset = offset;
	this->bounciness = bounciness;
	this->friction = friction;
	this->node = node;
	this->level = level;
	this->startPos = level->WorldToGrid(node->get_position());
}

bool Body::process(float delta)
{
	Vector2 endPos;
	Vector2 normal;
	SpelAABB aabb = SpelAABB();
	if (level->CheckCollisionWithTerrain(aabb, this->startPos, endPos, normal, this->isGrounded))
	{
		return true;
	}
	return false;
}
