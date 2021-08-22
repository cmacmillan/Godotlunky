#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"

class Bullet : public Node2D
{
	GODOT_CLASS(Bullet, Node2D);

public:
	Body body;
	Level* level;

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};

