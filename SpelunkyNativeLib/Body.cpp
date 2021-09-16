#pragma once
#include "Body.h"
#include "Level.h"
using namespace godot::Math;

void Body::Init(Vector2 size, Vector2 offset, float bounciness, float friction, Node2D* node, Level* level,Vector2 initialVelocity,bool pickable, float weight,HitboxMask takeDamageMask, IDamageReciever* damageReceiver, IThrowAction* throwAction,bool dealDamageWhenMovingFast,bool playHitSFX, Ref<AudioStream> hitSFX,ISmushReciever* smushReciever)
{
	this->aabb = SpelAABB();
	aabb.size = size;
	this->offset = offset;
	this->bounciness = bounciness;
	this->friction = friction;
	this->node = node;
	this->level = level;
	this->startPos = level->WorldToGrid(node->get_position());
	this->endPos= this->startPos;
	this->vel = initialVelocity;
	this->pickable = pickable;
	this->weight = weight;
	this->takeDamageMask = takeDamageMask;
	this->damageReciever = damageReceiver;
	this->throwAction = throwAction;
	this->dealDamageWhenMovingFast = dealDamageWhenMovingFast;
	this->pickedBy = nullptr;
	this->moveFastHitboxActive = false;
	this->isFacingRight = false;
	this->playHitSFX = playHitSFX;
	this->hitSFX = hitSFX;
	this->isGrounded = false;
	this->isSmushed= false;
	this->smushReciever = smushReciever;
	process(0, false, false);
}

void Body::OnDestroy(vector<HitboxData*>* hitboxesToRemove) {
	if (pickedBy!=nullptr) {
		pickedBy->PickedBodyDestroyed();
	}
	if (moveFastHitboxActive) {
		if (hitboxesToRemove == nullptr) {
			level->UnregisterHitbox(&moveFastHitbox);
		}
		else {
			hitboxesToRemove->push_back(&moveFastHitbox);
		}
	}
}

bool Body::process(float delta, bool applyGravity, bool applyFriction)
{
	if (isSmushed)
		return false;
	if (applyGravity) {
		vel.y += level->g * delta;
	}
	if (pickedBy != nullptr) {
		vel = (pickedBy->GetPickPosition() - node->get_position())/delta;
	}
	aabb.center = level->WorldToGrid(node->get_position()+vel*delta) + this->offset;
	endPos = aabb.center;
	bool hitTerrain = level->CheckCollisionWithTerrain(aabb, startPos, endPos, normal, isGrounded,isSmushed,this);
	isSmushed &= smushReciever != nullptr;
	if (isSmushed) {
		smushReciever->TakeSmush();
		return false;
	}
	if (hitTerrain)
	{
		if (playHitSFX && pickedBy==nullptr) {
			if ((normal.x != 0 || normal.y != 0) && (vel.dot(-normal) > 500))
			{
				level->PlayAudio(hitSFX==nullptr?level->defaultImpactSFX:hitSFX, aabb.center);
			}
		}
		if (normal.x != 0 && godot::Math::sign(normal.x) != godot::Math::sign(vel.x)) {
			vel.x = -vel.x * bounciness;
		}
		if (normal.y != 0 && godot::Math::sign(normal.y) != godot::Math::sign(vel.y)) {
			vel.y = -vel.y * bounciness;
		}
	}
	if (isGrounded && applyFriction) {
		vel.x = godot::Math::move_toward(vel.x, 0, delta * friction);
	}
	if (pickedBy != nullptr) {
		vel = Vector2(0,0);
	}
	bool shouldHitboxBeActive = dealDamageWhenMovingFast && vel.length() > 1200;
	moveFastHitbox.aabb = aabb;
	if (shouldHitboxBeActive && !moveFastHitboxActive) {
		moveFastHitbox.SetValues(aabb, 1, HitboxMask::Everything, Vector2(0, 0), 0,true,this,DamageSource::FlyingObjectDamage);
		level->RegisterHitbox(&moveFastHitbox);
		moveFastHitboxActive = true;
	}
	else if (!shouldHitboxBeActive && moveFastHitboxActive)
	{
		level->UnregisterHitbox(&moveFastHitbox);
		moveFastHitboxActive = false;
	}
	node->set_position(level->GridToWorld(endPos - offset));
	aabb.center = endPos;
	startPos = endPos;
	return hitTerrain;
}
