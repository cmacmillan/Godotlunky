#pragma once
#include "Common.h"
#include "Level.h"
class Level;
class Body
{
public:
	IDamageReciever* damageReciever;
	HitboxMask takeDamageMask;
	Level* level;
	Node2D* node;
	Vector2 startPos;
	Vector2 vel;
	SpelAABB aabb;
	Vector2 offset;
	float bounciness;
	float friction;
	bool isGrounded;
	bool pickable;
	bool throwable;
	float weight;
	Vector2 endPos;//basically just out
	Vector2 normal;//basically just out
	void Init(Vector2 size, Vector2 offset, float bounciness, float friction, Node2D* node, Level* level,Vector2 initialVelocity,bool pickable, bool throwable,float weight,HitboxMask mask,IDamageReciever* damageReciever);
	bool process(float delta, bool applyGravity, bool applyFriction);
};

class IBody {
public:
	virtual Body* GetBody() = 0;
};
