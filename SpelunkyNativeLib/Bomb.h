#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"

class Bomb : public Node2D, ISmushReciever
{
	GODOT_CLASS(Bomb, Node2D);

public:
	Vector2 startVelocity;
	Body body;
	float lifetime = 0;
	bool hasExploded = false;
	Level* level;
	HitboxData explosionHitbox;

	void TakeSmush();

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};
