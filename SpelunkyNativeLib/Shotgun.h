#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"
#include <Sprite.hpp>

class Shotgun : public Node2D, IThrowAction
{
	GODOT_CLASS(Shotgun, Node2D);

public:
	Body body;
	Level* level;
	Sprite* sprite;
	float cooldownTime;

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);

	virtual void DoThrowAction() override;
};

