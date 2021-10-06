#pragma once
#include "Common.h"
#include <Node2D.hpp>

enum class HeldItem :unsigned int {
	Unknown=0,
	PrizeBox=1,
	Shotgun=2,
	Rock=3,
};

class Globals : public Node2D
{
	GODOT_CLASS(Globals, Node2D);
public:
	int bombCount;
	int ropeCount;
	int healthCount;
	int cashCount;
	int levelIndex;
	HeldItem heldItem;
	bool shouldRead;
	bool isFirstLoad = true;
	int damageSource;
	bool useAutoRun=true;
	bool useClassicControls=true;
	bool useMusic = true;

	int highScore;
	int maxDepthBeaten;

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};

