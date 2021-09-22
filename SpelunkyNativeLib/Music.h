#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include <AudioStreamPlayer.hpp>

class Music : public Node2D
{
	GODOT_CLASS(Music, Node2D);

public:
	AudioStreamPlayer* audioSource;

	Ref<AudioStream> caveThemeMusic;
	Ref<AudioStream> bossThemeMusic;
	Ref<AudioStream> creditsTheme;
	Ref<AudioStream> mainMenuTheme;
		
	Ref<AudioStream> nextAudio;
	float currentVolume;

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};

