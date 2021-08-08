#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"

class Spelunker : public Node2D
{
	GODOT_CLASS(Spelunker, Node2D);

public:
	float jumpHeight = 0;
	float walkSpeed;
	Vector2 vel;
	Vector2 startPos;
	bool inited = false;
	Level* level=nullptr;
	bool holdingLedge = false;
	bool holdingRope= false;
	float grabRopeDisableTime = 0;
	bool facingRight = true;
	Vector2 ledgeCoords=Vector2(0,0);
	static void _register_methods();
	void _init();

	void _ready();
	void _process(float delta);
	Spelunker();
	~Spelunker();
};

