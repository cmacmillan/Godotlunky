#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>
#include <AudioStreamPlayer2D.hpp>
#include "HitboxData.h"

class Bat : public Node2D, IDamageReciever
{
	GODOT_CLASS(Bat, Node2D);

public:
	Body body;
	Level* level;
	HitboxData hitbox;
	AnimatedSprite* animatedSprite;
	bool hasSpottedPlayer;
	bool isDead;

	bool TakeDamage(int damageAmount, bool stun, vector<HitboxData*>* hitboxesToRemove);

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};

