class Body;
class Spelunker;
class AutoPickup;
class Godolmec;
class Level;
#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include <Input.hpp>
#include "Level.h"
#include <File.hpp>

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
	bool isInMainMenu = true;
	bool isPaused = false;
	Level* level;

	int highScore;
	int maxDepthBeaten;

	static void _register_methods();
	void Save();
	void UnPause();
	void _init();
	void _ready();
	void _process(float delta);
};

