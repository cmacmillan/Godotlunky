#pragma once
#include "Bat.h"
using namespace godot::Math;

void Bat::_register_methods()
{
	register_method("_ready", &Bat::_ready);
	register_method("_process", &Bat::_process);
}

void Bat::_init(){}

bool Bat::TakeDamage(int damageAmount, bool stun, vector<HitboxData*>* hitboxesToRemove,DamageSource source) {
	//no health so just die
	if (!isDead) {
		isDead = true;
		if (hitboxesToRemove != nullptr) {
			hitboxesToRemove->push_back(&hitbox);
		}
		else {
			level->UnregisterHitbox(&hitbox);
		}
		level->SpawnBlood(body.aabb.center);
		body.OnDestroy(hitboxesToRemove);
		queue_free();
	}
	return true;
}

void Bat::TakeSmush() {
	level->PlayAudio(level->smushSFX, body.aabb.center);
	TakeDamage(1,false,nullptr,DamageSource::MushedDamage);
	level->UnregisterHurtbox(&body);
}

void Bat::_ready()
{
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	body.Init(Vector2(.5,.5),Vector2(0,0),0,0,this,level,Vector2(0,0),false,1,HitboxMask::Enemy,this,nullptr,false,false,nullptr,this,HeldItem::Unknown);
	hitbox.SetValues(body.aabb, 1, HitboxMask::Player, Vector2(0, 0), 0, false,&body,DamageSource::BatDamage);
	hitbox.creatorToEscape = nullptr;
	animatedSprite = get_node<AnimatedSprite>("AnimatedSprite");
	level->RegisterHitbox(&hitbox);
	level->RegisterHurtbox(&body);
	isDead = false;
	hasSpottedPlayer = false;
}

void Bat::_process(float delta)
{
	hitbox.aabb = body.aabb;
	if (hasSpottedPlayer) {
		body.vel = (level->spelunker->body.aabb.center-body.aabb.center).normalized()*300;
		animatedSprite->set_flip_h(body.vel.x > 0);
		body.process(delta,false,false);
	}
	else {
		Vector2 block = body.aabb.center;
		block.y -= 1;
		if ((level->spelunker->body.endPos.y>body.endPos.y && level->spelunker->body.endPos.distance_to(body.endPos)<8) || !level->GetBlock(block.x,block.y)->present) {
			animatedSprite->set_animation("Flap");
			level->PlayAudio(level->batStartFlapSFX, body.aabb.center);
			hasSpottedPlayer = true;
		}
	}
}
