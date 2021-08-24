#pragma once
#include "Bat.h"
using namespace godot::Math;

void Bat::_register_methods()
{
	register_method("_ready", &Bat::_ready);
	register_method("_process", &Bat::_process);
}

void Bat::_init(){}

bool Bat::TakeDamage(int damageAmount, bool stun, vector<HitboxData*>* hitboxesToRemove) {
	//no health so just die
	if (hitboxesToRemove != nullptr) {
		hitboxesToRemove->push_back(&hitbox);
	} else {
		level->UnregisterHitbox(&hitbox);
	}
	body.OnDestroy(hitboxesToRemove);
	queue_free();
	return true;
}

void Bat::_ready()
{
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	body.Init(Vector2(.5,.5),Vector2(0,0),0,0,this,level,Vector2(0,0),false,1,HitboxMask::Enemy,this,nullptr,false);
	hitbox.SetValues(body.aabb, 1, HitboxMask::Player, Vector2(0, 0), 0, false);
	hitbox.creatorToEscape = nullptr;
	level->RegisterHitbox(&hitbox);
	level->RegisterHurtbox(&body);
	hasSpottedPlayer = false;
}

void Bat::_process(float delta)
{
	if (hasSpottedPlayer) {
		body.vel = level->spelunker->body.aabb.center-body.aabb.center;
		body.process(delta,false,false);
	}
	else {
	}
}
