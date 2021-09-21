#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"

class Globals : public Node2D
{
	GODOT_CLASS(Globals, Node2D);
public:
	int bombCount;
	int ropeCount;
	int healthCount;
	int cashCount;
	int levelIndex;
	bool shouldRead;

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};

