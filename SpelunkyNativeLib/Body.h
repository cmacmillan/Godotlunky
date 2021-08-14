#pragma once
#include "Common.h"
#include "Level.h"
class Body
{
public:
	Level* level;
	Node2D* node;
	Vector2 startPos;
	Vector2 vel;
	SpelAABB aabb;
	Vector2 offset;
	float bounciness;
	float friction;
	bool isGrounded;
	Vector2 endPos;//basically just out
	Vector2 normal;//basically just out
	//Body(Vector2 size, Vector2 offset, float bounciness, float friction, Node2D* node, Level* level,Vector2 initialVelocity);
	void Init(Vector2 size, Vector2 offset, float bounciness, float friction, Node2D* node, Level* level,Vector2 initialVelocity);
	bool process(float delta, bool applyGravity, bool applyFriction);
};
