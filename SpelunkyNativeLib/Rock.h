#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"

class Rock : public Node2D
{
	GODOT_CLASS(Rock, Node2D);

public:
	Body body;
	Level* level;

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};

