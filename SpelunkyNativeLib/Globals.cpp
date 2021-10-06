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
	register_property("isFirstLoad", &Globals::isFirstLoad, true);
	register_property("damageSource", &Globals::damageSource, 0);

	register_property("useAutoRun", &Globals::useAutoRun, true);
	register_property("useClassicControls", &Globals::useClassicControls, true);
	register_property("useMusic", &Globals::useMusic, true);
}

void Globals::_init(){}

void Globals::_ready()
{
	healthCount = 0;
	cashCount = 0;
	bombCount = 0;
	ropeCount = 0;
	shouldRead = false;
	isFirstLoad = true;
}

void Globals::_process(float delta)
{
}
