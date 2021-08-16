#pragma once
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"
#include <AnimatedSprite.hpp>
class Snake : public Node2D, IDamageReciever
{
	GODOT_CLASS(Snake, Node2D);
public:
	AnimatedSprite* sprite;
	Level* level;
	Body body;
	bool isFacingRight;

	Body* GetBody();

	bool TakeDamage(int damageAmount);

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};

