#pragma once
#include "Common.h"
#include "IDamageReciever.h"
#include "HitboxData.h"
#include <Node2D.hpp>

class Level;

class Body
{
public:
	IPicker* pickedBy;
	IThrowAction* throwAction;
	IDamageReciever* damageReciever;
	HitboxMask takeDamageMask;
	Level* level;
	Node2D* node;
	bool moveFastHitboxActive;
	HitboxData moveFastHitbox;
	Vector2 startPos;
	Vector2 vel;
	SpelAABB aabb;
	Vector2 offset;
	float bounciness;
	float friction;
	bool isGrounded;
	bool pickable;
	bool dealDamageWhenMovingFast;
	bool isFacingRight;
	float weight;
	Vector2 endPos;//basically just out
	Vector2 normal;//basically just out
	void OnDestroy(vector<HitboxData*>* hitboxesToRemove);
	void Init(Vector2 size, Vector2 offset, float bounciness, float friction, Node2D* node, Level* level,Vector2 initialVelocity,bool pickable, float weight,HitboxMask mask,IDamageReciever* damageReciever,IThrowAction* throwAction,bool dealDamageWhenMovingFast);
	bool process(float delta, bool applyGravity, bool applyFriction);
};
