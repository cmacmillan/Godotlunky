#include "Rock.h"
#include "Level.h"
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>
#include <AudioStreamPlayer2D.hpp>
using namespace godot::Math;

void Rock::_register_methods()
{
	register_method("_ready", &Rock::_ready);
	register_method("_process", &Rock::_process);
}

void Rock::_init(){}

void Rock::_ready()
{
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	body.Init(Vector2(.5,.5),Vector2(0,0),.3,10000,this,level,Vector2(0,0),true,1,HitboxMask::Everything,nullptr,nullptr,true);
	level->RegisterHurtbox(&body);
}

void Rock::_process(float delta)
{
	body.process(delta,true,true);
}
