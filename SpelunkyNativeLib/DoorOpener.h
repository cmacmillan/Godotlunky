#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"
#include "Godolmec.h"

class DoorOpener : public Node2D, IDamageReciever
{
	GODOT_CLASS(DoorOpener, Node2D);

public:
	Body body;
	Level* level;
	float flickerTime = 0;
	bool isFlickering;
	Sprite* sprite;

	Color normalColor;
	Color flashColor;

	bool isGodolmec;

	bool TakeDamage(int damageAmount, bool stun, vector<HitboxData*>* hitboxesToRemove,DamageSource source);

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};

