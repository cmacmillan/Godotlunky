#pragma once
#include "BloodSpurt.h"
#include "Level.h"
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>
#include <AudioStreamPlayer2D.hpp>
#include "ObjectMaker.h"
using namespace godot::Math;

void BloodSpurt::_register_methods()
{
	register_method("_ready", &BloodSpurt::_ready);
	register_method("_process", &BloodSpurt::_process);
}

void BloodSpurt::_init(){}

void BloodSpurt::_ready()
{
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	body.Init(Vector2(.15f, .15f), Vector2(0, 0), .1f, 10000, this, level, Vector2((Random()-.5f)*500.0f, -1000*Random()-500), false, 1, HitboxMask::Nothing, nullptr, nullptr, false, false, nullptr);
	particles = get_node<Particles2D>("Particles2D");
	particles->set_visibility_rect(Rect2(Vector2(0, 0), Vector2(100000,190000)));
	//level->RegisterHurtbox(&body);
	isDying = false;
	dieTime = 0;
}

void BloodSpurt::_process(float delta)
{
	if (isDying) {
		dieTime += delta;
		if (dieTime > 1.0f) {
			queue_free();
		}
		else if (dieTime > .5f) {
			particles->set_emitting(false);
		}
	}
	if (body.process(delta, true, true)&&body.vel.length()<100) {
		if (!isDying) {
			isDying = true;
		}
	}
}

