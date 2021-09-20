#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"
#include <AnimationPlayer.hpp>
#include "ObjectMaker.h"
#include "DoorOpener.h"

enum class GodolmecState : int { 
	BreakingFree=0,
	JumpingAtPlayer=1,
	WaitingToSwitchStates=2,
	WaitingToBreakFree=3,
	FiringBombs=4,
	WaitingToSwitchStatesForceJump=5,
	Dying = 6,
};
class DoorOpener;
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

	Node2D* shatter;

	GodolmecState state;
	float stateTime = 0;
	bool wasGrounded;

	float flashOpacity;
	float flashDirection;
	int health;
	float flashTime = 0;
	bool isTakingDamage;

	MovingPlatform jawHitbox1;
	MovingPlatform faceHitbox1;
	MovingPlatform faceHitbox2;

	Node2D* bombSpot1;
	Node2D* bombSpot2;
	Node2D* bombSpot3;
	Node2D* bombSpot4;
	Node2D* doorOpenerSpot;

	AudioStreamPlayer2D* explosionChargeSFX;
	DoorOpener* doorOpener;

	Sprite* faceRedFlash;
	Sprite* jawRedFlash;

	Sprite* jawCrack1;
	Sprite* jawCrack2;
	Sprite* jawCrack3;
	Sprite* faceCrack1;
	Sprite* faceCrack2;
	Sprite* faceCrack3;

	void SwitchState(GodolmecState targetState);

	void TakeDamage();

	void FireBomb(int index);

	void FinishAnimation();

	void StopShaking();

	void SetColliderPositions();
	Vector2 GetBodyBounds(float& center);

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};

