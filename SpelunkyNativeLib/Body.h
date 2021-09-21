#pragma once
#include "Common.h"
#include "IDamageReciever.h"
#include "HitboxData.h"
#include <Node2D.hpp>
#include <AudioStreamPlayer2D.hpp>
#include <AudioStream.hpp>
#include "Globals.h"

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
	bool isSmushed;
	ISmushReciever* smushReciever;
	bool ignoreCustomCollision;
	HeldItem heldItemType;

	bool pickable;
	bool dealDamageWhenMovingFast;
	bool isFacingRight;
	bool playHitSFX;
	Ref<AudioStream> hitSFX;
	float weight;
	Vector2 endPos;//basically just out
	Vector2 normal;//basically just out
	void OnDestroy(vector<HitboxData*>* hitboxesToRemove);
	void Init(Vector2 size, Vector2 offset, float bounciness, float friction, Node2D* node, Level* level, Vector2 initialVelocity, bool pickable, float weight, HitboxMask takeDamageMask, IDamageReciever* damageReceiver, IThrowAction* throwAction, bool dealDamageWhenMovingFast, bool playHitSFX, Ref<AudioStream> hitSFX,ISmushReciever* smushReciever,HeldItem heldType);
	bool process(float delta, bool applyGravity, bool applyFriction);
};
