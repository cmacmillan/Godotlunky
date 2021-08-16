#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"
class Rope : public Node2D, IBody
{
	GODOT_CLASS(Rope, Node2D);
public:
	float throwStrength;
	int length;
	int currSegmentIndex=0;
	int subIndex=0;
	float timeSinceLastSegment=0;
	int baseY;
	int baseX;
	bool hasUnfurled;
	Level* level;
	Body body;

	Body* GetBody();

	void SpawnSegment(float yOffset);
	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};
