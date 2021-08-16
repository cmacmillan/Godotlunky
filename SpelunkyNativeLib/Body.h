#pragma once
#include "Common.h"
#include "Level.h"
class Level;

class IPicker {
public:
	virtual Vector2 GetPickPosition()=0;
	virtual void PickedBodyDestroyed()=0;
};
class Body
{
public:
	IPicker* pickedBy;
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
