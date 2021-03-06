#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include <Camera2D.hpp>
#include <Sprite.hpp>
#include "Body.h"
#include "DamageSource.h"
#include "Globals.h"
#include "Music.h"

class Spelunker : public Node2D, IDamageReciever, IPicker, ISmushReciever
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
	float deadTime = 0;
	bool isEnteringDoor = false;
	float enterDoorOpacity = 1;
	float grabRopeDisableTime = 0;
	float timeLookingUp = 0;
	float timeLookingDown = 0;
	bool wasGrounded;
	bool frozenInCutscene = true;
	Camera2D* camera;
	Vector2 grabbedLedgeBlock;
	Vector2 baseLedgeCoords = Vector2(0, 0);
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
	HitboxData whipHitbox;
	HitboxData footHitbox;

	Globals* globals;
	Music* music;

	int levelIndex;
	float spaceTextLerp;

	int ropeCount = 4;
	int bombCount= 4;
	int goldCollected = 0;

	Body* GetBody();
	Vector2 GetPickPosition();
	void PickedBodyDestroyed();

	void TakeSmush();

	void Die(DamageSource source,bool playSound=true);
	bool TakeDamage(int damageAmount,bool stun,vector<HitboxData*>* hitboxesToRemove,DamageSource source);

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
	Spelunker();
	~Spelunker();
};

