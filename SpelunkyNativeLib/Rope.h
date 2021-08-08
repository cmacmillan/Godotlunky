#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
class Rope : public Node2D
{
	GODOT_CLASS(Rope, Node2D);
public:
	float throwStrength;
	Vector2 vel;
	int length;
	int currSegmentIndex=0;
	int subIndex=0;
	float timeSinceLastSegment=0;
	int baseY;
	int baseX;
	bool hasUnfurled;
	Level* level;
	bool isInited;
	Vector2 startPos;

	void SpawnSegment(float yOffset);
	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};
