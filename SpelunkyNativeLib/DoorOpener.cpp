#include "DoorOpener.h"
#include "Level.h"
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>
#include <AudioStreamPlayer2D.hpp>
using namespace godot::Math;

void DoorOpener::_register_methods()
{
	register_method("_ready", &DoorOpener::_ready);
	register_method("_process", &DoorOpener::_process);
	
	register_property("normalColor", &DoorOpener::normalColor, Color(1, 1, 1, 1));
	register_property("flashColor", &DoorOpener::flashColor, Color(1, 1, 1, 1));
}

void DoorOpener::_init(){}

void DoorOpener::_ready()
{
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	body.Init(Vector2(1,1),Vector2(0,0),0,0,this,level,Vector2(0,0),false,1,HitboxMask::Item,this,nullptr,false,false,nullptr,nullptr);
	level->RegisterHurtbox(&body);
	sprite = get_node<Sprite>("Sprite");
	sprite->set_modulate(normalColor);
	isFlickering = false;
	isGodolmec = false;
}

bool DoorOpener::TakeDamage(int damageAmount, bool stun, vector<HitboxData*>* hitboxesToRemove,DamageSource source) {
	if (!isFlickering) {
		if (!isGodolmec || (level->godolmec->state == GodolmecState::FiringBombs && !level->godolmec->isTakingDamage)) {
			level->DoorSwitchHit(isGodolmec);
			level->PlayAudio(level->switchHitSFX, body.aabb.center);
			isFlickering = true;
			flickerTime = 0;
		}
	}
	return false;
}

void DoorOpener::_process(float delta)
{
	if (isGodolmec) {
		auto pos = level->godolmec->doorOpenerSpot->get_global_position();
		body.aabb.center = level->WorldToGrid(pos);
		set_global_position(pos);
	}
	if (isFlickering) {
		flickerTime += delta;
		if (flickerTime > 2.0f) {
			isFlickering = false;
			sprite->set_modulate(normalColor);
		}
		else 
		{
			if (godot::Math::fmod(flickerTime, .4f) < .2) {
				sprite->set_modulate(flashColor);
			}
			else
			{
				sprite->set_modulate(normalColor);
			}
		}
	}
}
