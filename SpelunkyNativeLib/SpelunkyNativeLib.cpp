#pragma once
#include "Common.h"
#include "Level.h"
#include "Spelunker.h"
#include "Bomb.h"
#include "Rope.h"
#include "Snake.h"
#include "Rock.h"
#include "Shotgun.h"
#include "Bullet.h"
#include "Bat.h"
#include "BloodSpurt.h"
#include "PrizeBox.h"
#include "AutoPickup.h"
#include "Spider.h"
#include "DoorOpener.h"
#include "Godolmec.h"
#include "Globals.h"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
	godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
	godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
	godot::Godot::nativescript_init(handle);

	godot::register_class<Level>();
	godot::register_class<Spelunker>();
	godot::register_class<Bomb>();
	godot::register_class<Rope>();
	godot::register_class<Snake>();
	godot::register_class<Rock>();
	godot::register_class<Shotgun>();
	godot::register_class<Bullet>();
	godot::register_class<Bat>();
	godot::register_class<BloodSpurt>();
	godot::register_class<PrizeBox>();
	godot::register_class<AutoPickup>();
	godot::register_class<Spider>();
	godot::register_class<DoorOpener>();
	godot::register_class<Godolmec>();
	godot::register_class<Globals>();
}