#include "PrizeBox.h"
#include "Level.h"
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>
#include <AudioStreamPlayer2D.hpp>
using namespace godot::Math;

void PrizeBox::_register_methods()
{
	register_method("_ready", &PrizeBox::_ready);
	register_method("_process", &PrizeBox::_process);
}

void PrizeBox::_init(){}

void PrizeBox::_ready()
{
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	body.Init(Vector2(.8f,.5f),Vector2(0,0),.1,5000,this,level,Vector2(0,0),true,1,HitboxMask::Enemy,this,this,true,true,nullptr);
	level->RegisterHurtbox(&body);
}

void PrizeBox::OpenBox(vector<HitboxData*>* hitboxesToRemove) 
{
	level->PlayAudio(level->boxOpenSFX,body.aabb.center);
	body.OnDestroy(hitboxesToRemove);
	queue_free();
}

void PrizeBox::_process(float delta)
{
	body.process(delta,true,true);
}

void PrizeBox::DoThrowAction()
{
	OpenBox(nullptr);
	level->UnregisterHurtbox(&body);
}

bool PrizeBox::TakeDamage(int damageAmount, bool stun, vector<HitboxData*>* hitboxesToRemove) 
{
	OpenBox(hitboxesToRemove);
	return true;
}
