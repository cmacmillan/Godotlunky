#pragma once
#include <Node2D.hpp>
#include "Level.h"
class Snake : public Node2D
{
	GODOT_CLASS(Snake, Node2D);
public:
	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};

