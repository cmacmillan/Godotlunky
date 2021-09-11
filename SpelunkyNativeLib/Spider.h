#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"
#include <AnimatedSprite.hpp>

class Spider : public Node2D, IDamageReciever, ISmushReciever
{
	GODOT_CLASS(Spider, Node2D);

public:
	Body body;
	Level* level;
	HitboxData hitbox;
	bool isDead;
	bool hasLanded;
	bool hasSpottedPlayer;
	float timeBeforeJump;
	AnimatedSprite* animatedSprite;

	void TakeSmush();

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);


	bool TakeDamage(int damageAmount, bool stun, vector<HitboxData*>* hitboxesToRemove);
};

