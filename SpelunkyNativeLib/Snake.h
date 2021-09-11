#pragma once
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"
#include <AnimatedSprite.hpp>
class Snake : public Node2D, IDamageReciever, ISmushReciever
{
	GODOT_CLASS(Snake, Node2D);
public:
	AnimatedSprite* sprite;
	Level* level;
	Body body;
	HitboxData hitbox;
	bool isDead;

	void TakeSmush();

	bool TakeDamage(int damageAmount,bool stun,vector<HitboxData*>* hitboxesToRemove);

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};

