#include "Body.h"
using namespace godot::Math;

void Body::Init(Vector2 size, Vector2 offset, float bounciness, float friction,Node2D* node,Level* level,Vector2 initialVelocity)
{
	this->aabb = SpelAABB();
	aabb.size = size;
	this->offset = offset;
	this->bounciness = bounciness;
	this->friction = friction;
	this->node = node;
	this->level = level;
	this->startPos = level->WorldToGrid(node->get_position());
	this->vel = initialVelocity;
}

bool Body::process(float delta, bool applyGravity, bool applyFriction)
{
	if (applyGravity) {
		vel.y += level->g * delta;
	}
	aabb.center = level->WorldToGrid(node->get_position()+vel*delta) + this->offset;
	endPos = aabb.center;
	bool hitTerrain = level->CheckCollisionWithTerrain(aabb, startPos, endPos, normal, isGrounded);
	if (hitTerrain)
	{
		if (normal.x != 0 && sign(normal.x) != sign(vel.x)) {
			vel.x = -vel.x * bounciness;
		}
		if (normal.y != 0 && sign(normal.y) != sign(vel.y)) {
			vel.y = -vel.y * bounciness;
		}
	}
	if (isGrounded && applyFriction) {
		vel.x = godot::Math::move_toward(vel.x, 0, delta * friction);
	}
	node->set_position(level->GridToWorld(endPos - offset));
	aabb.center = endPos;
	startPos = endPos;
	return hitTerrain;
}
