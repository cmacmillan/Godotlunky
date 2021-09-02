#pragma once
#include "Shotgun.h"
#include "Level.h"
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>
#include <AudioStreamPlayer2D.hpp>
#include "Bullet.h"
#include <PackedScene.hpp>
#include "ObjectMaker.h"
using namespace godot::Math;

void Shotgun::_register_methods()
{
	register_method("_ready", &Shotgun::_ready);
	register_method("_process", &Shotgun::_process);
}

void Shotgun::_init(){}

void Shotgun::_ready()
{
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	body.Init(Vector2(.7,.3),Vector2(0,0),.1,5000,this,level,Vector2(0,0),true,1,HitboxMask::Item,nullptr,this,true,true,nullptr);
	sprite = get_node<Sprite>("Sprite");
	level->RegisterHurtbox(&body);
}

void Shotgun::_process(float delta)
{
	if (cooldownTime > 0) {
		cooldownTime -= delta;
	}
	body.process(delta,true,true);
	sprite->set_flip_h(!body.isFacingRight);
}

void Shotgun::DoThrowAction()
{
	if (cooldownTime <= 0) {
		for (int i = 0; i < 10; i++) {
			auto bullet = SpawnBullet(level, Vector2(body.endPos.x, body.endPos.y));
			bullet->body.moveFastHitbox.creatorToEscape = this->body.pickedBy->GetBody();
			bullet->body.vel.x = (2000+(Random()*1000)) * (body.isFacingRight ? 1 : -1);
			float rand = Random()*2 - 1;
			bullet->body.vel.y = rand * 100;
		}
		body.pickedBy->GetBody()->vel += Vector2(3000 * (body.pickedBy->GetBody()->isFacingRight ? -1 : 1), -300);
		cooldownTime = 1;
		auto audio = get_node<AudioStreamPlayer2D>("AudioSource");
		audio->set_volume_db(0.0f);
		audio->set_stream(level->shotgunSFX);
		audio->play();
	}
}
