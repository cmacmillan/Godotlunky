#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"
#include <AnimationPlayer.hpp>

class Godolmec : public Node2D
{
	GODOT_CLASS(Godolmec, Node2D);

public:
	Body body;
	Level* level;
	AnimationPlayer* animPlayer;
	Node2D* topPoint;
	Node2D* bottomPoint;
	Node2D* faceRoot;

	MovingPlatform jawHitbox1;
	MovingPlatform faceHitbox1;
	MovingPlatform faceHitbox2;

	void SetColliderPositions();
	Vector2 GetBodyBounds(float& center);

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};

