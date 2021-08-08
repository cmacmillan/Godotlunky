#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"

class Bomb : public Node2D
{
	GODOT_CLASS(Bomb, Node2D);

public:
	float friction;
	float bounciness;
	float lifetime = 0;
	bool hasExploded = false;
	Vector2 vel;
	Vector2 startPos;
	bool inited = false;
	Level* level;
	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};
