#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include <Camera2D.hpp>
#include <Sprite.hpp>


class Spelunker : public Node2D
{
	GODOT_CLASS(Spelunker, Node2D);

public:
	float jumpHeight = 0;
	float walkSpeed;
	Vector2 vel;
	Vector2 startPos;
	Level* level=nullptr;
	bool holdingLedge = false;
	bool holdingRope= false;
	float grabRopeDisableTime = 0;
	bool facingRight = true;
	float timeLookingUp = 0;
	float timeLookingDown = 0;
	bool wasGrounded;
	Camera2D* camera;
	Vector2 grabbedLedgeBlock;
	Vector2 ledgeCoords=Vector2(0,0);
	Sprite* whipBack;
	Sprite* whipForward;
	bool isWhipping;
	bool isStunned;
	bool isDead = false;
	float stunTime = 0;
	bool playedWhipSound;
	float whipTime;
	float friction;
	static void _register_methods();
	void _init();

	void _ready();
	void _process(float delta);
	Spelunker();
	~Spelunker();
};

