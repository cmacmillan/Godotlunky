#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include <Camera2D.hpp>
#include <Sprite.hpp>
#include "Body.h"


class Spelunker : public Node2D, IDamageReciever, IPicker
{
	GODOT_CLASS(Spelunker, Node2D);

public:
	Body body;
	float jumpHeight = 0;
	float walkSpeed;
	int health=0;
	bool invulFlicker;
	float invulTime=0;
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
	Body* pickedBody = nullptr;

	Vector2 GetPickPosition();
	void PickedBodyDestroyed();

	void Die();
	bool TakeDamage(int damageAmount,bool stun,vector<HitboxData*>* hitboxesToRemove);

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
	Spelunker();
	~Spelunker();
};

