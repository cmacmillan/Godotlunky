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
	Vector2 size;
	Vector2 offset;
	float bounciness;
	float friction;
	bool isGrounded;
	Body(Vector2 size, Vector2 offset, float bounciness, float friction, Node2D* node, Level* level);
	bool process(float delta,Level* level);
};
