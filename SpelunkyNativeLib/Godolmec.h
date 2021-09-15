#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"
#include <AnimationPlayer.hpp>

enum GodolmecState : int { 
	BreakingFree=0,
	JumpingAtPlayer=1,
	WaitingToSwitchStates=2,
	WaitingToBreakFree=3,
	FiringBombs=4,
};
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

	GodolmecState state;
	float stateTime = 0;
	bool wasGrounded;

	MovingPlatform jawHitbox1;
	MovingPlatform faceHitbox1;
	MovingPlatform faceHitbox2;

	void SwitchState(GodolmecState targetState);

	void SetColliderPositions();
	Vector2 GetBodyBounds(float& center);

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};

