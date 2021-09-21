#include "Globals.h"
#include "Level.h"
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>
#include <AudioStreamPlayer2D.hpp>
using namespace godot::Math;

void Globals::_register_methods()
{
	register_method("_ready", &Globals::_ready);
	register_method("_process", &Globals::_process);
}

void Globals::_init(){}

void Globals::_ready()
{
	healthCount = 0;
	cashCount = 0;
	bombCount = 0;
	ropeCount = 0;
	shouldRead = false;
}

void Globals::_process(float delta)
{
}
