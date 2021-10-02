#include "Music.h"
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>

void Music::_register_methods()
{
	register_method("_ready", &Music::_ready);
	register_method("_process", &Music::_process);

	register_property("bossThemeMusic", &Music::bossThemeMusic, Ref<AudioStream>());
	register_property("caveThemeMusic", &Music::caveThemeMusic, Ref<AudioStream>());
	register_property("creditsTheme", &Music::creditsTheme, Ref<AudioStream>());
	register_property("mainMenuTheme", &Music::mainMenuTheme, Ref<AudioStream>());

	register_property("nextAudio", &Music::nextAudio, Ref<AudioStream>());
	register_property("currentVolume", &Music::currentVolume, 0.0f);
}

void Music::_init(){}

void Music::_ready()
{
	audioSource = get_node<AudioStreamPlayer>("audioSource");
	currentVolume = 0.0f;
}

void Music::_process(float delta)
{
	auto currentStream = audioSource->get_stream();
	if (!audioSource->is_playing() && currentStream!=nullptr && currentStream!=creditsTheme)
		audioSource->play();
	if (currentStream != nextAudio) {
		currentVolume = godot::Math::move_toward(currentVolume, 0.0f, delta);
		if (currentVolume==0.0f) {
			audioSource->set_stream(nextAudio);
		}
	}
	else
	{
		if (currentStream == nullptr) {
			audioSource->set_volume_db(-100.0f);
			return;
		}
		currentVolume = godot::Math::move_toward(currentVolume, 1.0f, delta);
	}
	audioSource->set_volume_db(godot::Math::lerp(-100,-15,currentVolume));
}
