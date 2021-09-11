#pragma once
#include "AutoPickup.h"
#include "Level.h"
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>
#include <AudioStreamPlayer2D.hpp>
using namespace godot::Math;

void AutoPickup::_register_methods()
{
	register_method("_ready", &AutoPickup::_ready);
	register_method("_process", &AutoPickup::_process);

	register_property("hitboxSize", &AutoPickup::hitboxSize, Vector2(0,0));
	register_property("hitboxOffset", &AutoPickup::hitboxOffset, Vector2(0,0));
	register_property("amountOfStuff", &AutoPickup::amountOfStuff, 0);
	register_property("typeInt", &AutoPickup::typeInt, 0);
}

void AutoPickup::_init(){}

void AutoPickup::PickedUp(Spelunker* spelunker) {
	switch (type) {
	case AutoPickupType::BombPile:
		spelunker->bombCount += amountOfStuff;
		break;
	case AutoPickupType::RopePile:
		spelunker->ropeCount += amountOfStuff;
		break;
	case AutoPickupType::Gold:
		spelunker->goldCollected += amountOfStuff;
		break;
	default:
		printf("NOT IMPLEMENTED!");
		break;
	}
	level->PlayAudio(GetPickupSFX(), body.aabb.center);
	level->UnregisterHurtbox(&body);
	body.OnDestroy(nullptr);
	queue_free();
}

Ref<AudioStream> AutoPickup::GetPickupSFX() {
	switch (type) {
	case AutoPickupType::Gold:
		return level->goldPickupSFX;
	default:
		return level->itemPickupSFX;
	}
}

void AutoPickup::TakeSmush() {
	level->PlayAudio(level->itemSmushSFX,body.aabb.center);
	level->UnregisterHurtbox(&body);
	body.OnDestroy(nullptr);
	queue_free();
}

void AutoPickup::_ready()
{
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	type = (AutoPickupType)typeInt;
	body.Init(hitboxSize, hitboxOffset, .1, 5000, this, level, Vector2(0, 0), false, 1, HitboxMask::Item,nullptr,nullptr,true,true,nullptr,this);
	level->RegisterHurtbox(&body);
	if (timeUntilActive <= 0) {
		level->autoPickups->insert(this);
		isRegistered = true;
	}
}

void AutoPickup::_process(float delta)
{
	body.process(delta,true,true);
	if (timeUntilActive > 0) {
		timeUntilActive -= delta;
	}
	if (timeUntilActive <= 0 && !isRegistered) {
		level->autoPickups->insert(this);
		isRegistered = true;
	}
}
