#include "Globals.h"
#include "Level.h"
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>
#include <AudioStreamPlayer2D.hpp>
#include <InputMap.hpp>
using namespace godot::Math;

void Globals::_register_methods()
{
	register_method("_ready", &Globals::_ready);
	register_method("_process", &Globals::_process);
	register_method("UnPause", &Globals::UnPause);
	register_property("isFirstLoad", &Globals::isFirstLoad, true);
	register_property("damageSource", &Globals::damageSource, 0);

	register_property("useAutoRun", &Globals::useAutoRun, true);
	register_property("useClassicControls", &Globals::useClassicControls, true);
	register_property("useMusic", &Globals::useMusic, true);
	register_property("isInMainMenu", &Globals::isInMainMenu, true);
	register_property("isPaused", &Globals::isPaused, false);
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
	this->set_pause_mode(PAUSE_MODE_PROCESS);	
}

void Globals::UnPause() {
	get_tree()->set_pause(false);
	get_node<Control>("/root/GameScene/CameraTarget/Camera2D/CanvasLayer/SettingsBackground")->set_visible(false);
	get_node<Control>("/root/GameScene/CameraTarget/Camera2D/CanvasLayer/Settings")->set_visible(false);
	isPaused = false;
}	

void Globals::_process(float delta)
{
	if (isInMainMenu)
		return;
	level = this->get_node<Level>("/root/GameScene/Level");
	if (level == nullptr)
		return;
	if (level->spelunker->isDead || level->isFadingOut)
		return;
	auto input = Input::get_singleton();
	if (input->is_action_just_pressed("esc")) {
		isPaused = !isPaused;
		if (isPaused) {
			get_tree()->set_pause(true);
			get_node<Control>("/root/GameScene/CameraTarget/Camera2D/CanvasLayer/SettingsBackground")->set_visible(true);
			get_node<Control>("/root/GameScene/CameraTarget/Camera2D/CanvasLayer/Settings")->set_visible(true);
		}
		else {
			UnPause();
		}
	}
}
