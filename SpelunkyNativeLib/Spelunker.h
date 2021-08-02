#pragma once
#include "Common.h"
#include <Node2D.hpp>

class Spelunker : public Node2D
{
	GODOT_CLASS(Spelunker, Node2D);

	float jumpHeight = 0;
	float gravity = 0;

public:
	static void _register_methods();
	void _init();

	void _ready();
	void _process(float delta);
	Spelunker();
	~Spelunker();
};

