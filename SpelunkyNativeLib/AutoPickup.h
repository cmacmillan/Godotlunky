#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"

enum AutoPickupType : unsigned int{
	RopePile = 0,
	BombPile = 1,
	Gold = 2,
};

class Spelunker;

class AutoPickup : public Node2D
{
	GODOT_CLASS(AutoPickup, Node2D);

public:
	Body body;
	Level* level;
	AutoPickupType type;

	//exposed
	Vector2 hitboxSize;
	Vector2 hitboxOffset;
	int amountOfStuff;
	int typeInt;
	//////////////

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
	void PickedUp(Spelunker* spelunker);
};

