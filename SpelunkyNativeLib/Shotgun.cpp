#include "Shotgun.h"
#include "Level.h"
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>
#include <AudioStreamPlayer2D.hpp>
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
	body.Init(Vector2(.7,.3),Vector2(0,0),.1,5000,this,level,Vector2(0,0),true,1,HitboxMask::Everything,nullptr,nullptr,true);
	level->RegisterHurtbox(&body);
}

void Shotgun::_process(float delta)
{
	body.process(delta,true,true);
}
