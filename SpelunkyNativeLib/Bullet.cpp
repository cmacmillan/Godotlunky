#pragma once
#include "Bullet.h"
#include "Level.h"
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>
#include <AudioStreamPlayer2D.hpp>
using namespace godot::Math;

void Bullet::_register_methods()
{
	register_method("_ready", &Bullet::_ready);
	register_method("_process", &Bullet::_process);
}

void Bullet::_init(){}

void Bullet::_ready()
{
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	body.Init(Vector2(.2,.2),Vector2(0,0),0,0,this,level,Vector2(0,0),false,1,HitboxMask::Everything,nullptr,nullptr,true,true,level->bulletImpactSFX);
}

void Bullet::_process(float delta)
{
	if (body.process(delta, false, false)) {
		body.OnDestroy(nullptr);
		queue_free();
	}
}
