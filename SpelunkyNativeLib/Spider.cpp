#include "Spider.h"
#include "Level.h"
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>
#include <AudioStreamPlayer2D.hpp>
using namespace godot::Math;

void Spider::_register_methods()
{
	register_method("_ready", &Spider::_ready);
	register_method("_process", &Spider::_process);
}

void Spider::_init(){}

void Spider::TakeSmush() {
	level->PlayAudio(level->smushSFX, body.aabb.center);
	TakeDamage(1,false,nullptr,DamageSource::MushedDamage);
	level->UnregisterHurtbox(&body);
}

void Spider::_ready()
{
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	body.Init(Vector2(.8,.4),Vector2(0,0),.2f,9999999,this,level,Vector2(0,0),false,1,HitboxMask::Enemy,this,nullptr,false,true,level->spiderJumpSFX,this,HeldItem::Unknown);
	hitbox.creatorToEscape = nullptr;
	hitbox.SetValues(body.aabb, 1, HitboxMask::Player, Vector2(0, 0), 0, false,&body,DamageSource::SpiderDamage);
	animatedSprite = get_node<AnimatedSprite>("AnimatedSprite");
	level->RegisterHurtbox(&body);
	level->RegisterHitbox(&hitbox);
	isDead = false;
	hasSpottedPlayer = false;
	timeBeforeJump = 0;
	hasLanded = false;
}

bool Spider::TakeDamage(int damageAmount, bool stun, vector<HitboxData*>* hitboxesToRemove,DamageSource source) {
	//no health so just die
	if (!isDead) {
		isDead = true;
		if (hitboxesToRemove != nullptr) 
		{
			hitboxesToRemove->push_back(&hitbox);
		}
		else 
		{
			level->UnregisterHitbox(&hitbox);
		}
		level->SpawnBlood(body.aabb.center);
		body.OnDestroy(hitboxesToRemove);
		queue_free();
	}
	return true;
}

#define maxBlocksToSeePlayer 7

void Spider::_process(float delta)
{
	hitbox.aabb = body.aabb;
	if (hasSpottedPlayer) 
	{
		body.process(delta,true,true);
		if (!hasLanded) {
			if (animatedSprite->get_frame() == 8) {
				body.offset = Vector2(0, .2f);
				hasLanded = true;
			}
		}
		else 
		{
			if (body.isGrounded) 
			{
				if (animatedSprite->get_animation() == "Jump") {
					animatedSprite->set_speed_scale(6);
					animatedSprite->set_animation("UnJump");
				}
				timeBeforeJump += delta;
				if (timeBeforeJump > 1.0f) {
					level->PlayAudio(level->spiderJumpSFX, body.aabb.center);
					animatedSprite->set_speed_scale(3);
					animatedSprite->set_animation("Jump");
					body.vel = Vector2(700*godot::Math::sign(level->spelunker->body.aabb.center.x-body.aabb.center.x),-1400);
					timeBeforeJump = 0.0f;
				}
			}
		}
	}
	else 
	{
		Vector2 block = body.aabb.center;
		if (!level->GetBlock(block.x, block.y-1)->present) {
			animatedSprite->set_animation("FlipOver");
			hasSpottedPlayer = true;
		}
		auto playerCenter = level->spelunker->body.aabb.center;
		auto playerBlock = level->GetBlock(playerCenter.x, playerCenter.y);
		for (int i = block.y; i < block.y + maxBlocksToSeePlayer && i < level->blocksYRes; i++) {
			if (level->GetBlock(block.x, i)->present)
				break;
			if (level->GetBlock(block.x, i) == playerBlock) {
				hasSpottedPlayer = true;
				animatedSprite->set_speed_scale(3);
				animatedSprite->set_animation("FlipOver");
				break;
			}
		}
	}
}
