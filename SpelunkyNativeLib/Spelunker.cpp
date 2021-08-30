#pragma once
#include "Spelunker.h"
#include <Input.hpp>
#include "Level.h"
#include <AnimatedSprite.hpp>
#include "Bomb.h"
#include "Rope.h"
#include <ResourceLoader.hpp>
#include <AudioStreamPlayer2D.hpp>
#include <AudioStream.hpp>
#include "ObjectMaker.h"
using namespace godot::Math;

void Spelunker::_register_methods()
{
	register_method("_ready", &Spelunker::_ready);
	register_method("_process", &Spelunker::_process);

	register_property("jumpHeight", &Spelunker::jumpHeight, 0.0f);
	register_property("walkSpeed", &Spelunker::walkSpeed, 0.0f);
}

void Spelunker::_init()
{
}


bool Spelunker::TakeDamage(int damageAmount,bool stun,vector<HitboxData*>* hitboxesToRemove) {
	if (invulTime <= 0) {
		if (stun) {
			isStunned = true;
			stunTime = 0;
		}
		health -= damageAmount;
		if (health <= 0) {
			health = 0;
			Die();
			return true;
		}
		else {
			invulTime = 2;
		}
	}
	return false;
}
void Spelunker::Die() {
	if (!isDead) {
		level->SpawnBlood(body.aabb.center);
		isDead = true;
		isStunned = true;
		stunTime = -10000;
		if (pickedBody != nullptr) {
			pickedBody->pickedBy = nullptr;
		}
		pickedBody = nullptr;
	}
}

void Spelunker::PickedBodyDestroyed() {
	pickedBody = nullptr;
}
Body* Spelunker::GetBody() {
	return &body;
}
Vector2 Spelunker::GetPickPosition() {
	return get_position()+Vector2(0,20);
}

void Spelunker::_ready()
{
	health = 4;
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	body.Init(Vector2(.72f, .9f), Vector2(0, .11f), 0, 5000, this, level, Vector2(0, 0), false, 1, HitboxMask::Player,this,nullptr,false,false,nullptr);
	camera = Object::cast_to<Camera2D>(get_node("Camera2D"));
	whipForward = get_node<Sprite>("WhipForward");
	whipBack = get_node<Sprite>("WhipBack");
	isWhipping = false;
	isDead = false;
	isStunned = false;
	body.isFacingRight = true;
	pickedBody = nullptr;
	level->RegisterHurtbox(&body);
	level->spelunker = this;
}

void Spelunker::_process(float delta)
{
	auto animator = get_node<AnimatedSprite>(".");
	if (invulTime > 0) {
		invulTime -= delta;
		invulFlicker = !invulFlicker;
	}
	else {
		invulTime = 0;
		invulFlicker = false;
	}
	animator->set_visible(!invulFlicker);
	Vector2 ogVel = body.vel;
	if (holdingLedge && !level->GetBlock(grabbedLedgeBlock.x, grabbedLedgeBlock.y)->present) {
		holdingLedge = false;
	}
	if (!holdingLedge) {
		if (body.process(delta, !holdingLedge && !holdingRope, true)) {
			Vector2 coordsAhead = body.endPos + Vector2((body.isFacingRight? 1 : -1), 0);
			Vector2 coordsUp = coordsAhead - Vector2(0, 1);
			if (!isStunned&&!isWhipping&&body.normal.x == (body.isFacingRight? -1 : 1) && body.vel.y > 0 && level->GetBlock(coordsAhead.x,coordsAhead.y)->present && !level->GetBlock(coordsUp.x,coordsUp.y)->present && godot::Math::fmod(body.endPos.y,1)<.3f)
			{
				grabbedLedgeBlock = coordsAhead;
				holdingLedge = true;
				ledgeCoords = body.endPos;
				ledgeCoords.y = godot::Math::floor(body.endPos.y)+.15f;
				body.vel.x = 0;
				body.vel.y = 0;
			}
			else
			{
				holdingLedge = false;
			}
		}
	}
	else 
	{
		body.startPos = ledgeCoords;
		set_position(level->GridToWorld(ledgeCoords));
	}

	float accelSpeed=20000;

	auto input = Input::get_singleton();
	bool isRunning = false;
	bool isIdle = true;
	bool isCrouching = false;
	if (input->is_action_pressed("run")) {
		isRunning = true;
	}
	if (input->is_action_pressed("crouch")) {
		isCrouching = true;
	}
	if (input->is_action_just_pressed("debugstun")) {
		isStunned = true;
		stunTime = 0;
	}
	if (isDead) {
		holdingLedge = false;
		holdingRope = false;
	}
	if (isStunned) {
		stunTime += delta;
		holdingLedge = false;
		isWhipping = false;
		holdingRope = false;
		animator->set_animation("Stunned");
		get_node<AnimatedSprite>("StunBirds")->set_visible(!isDead);
		if (stunTime > 1.5 &&!isDead) {
			isStunned = false;
		}
	}
	else 
	{
		get_node<AnimatedSprite>("StunBirds")->set_visible(false);
	}
	if (body.isFacingRight) {
		whipForward->set_position(Vector2(100,30));
		whipBack->set_position(Vector2(-100,-50));
		whipForward->set_flip_h(false);
		whipBack->set_flip_h(false);
	}
	else {
		whipForward->set_position(Vector2(-100,30));
		whipBack->set_position(Vector2(100,-50));
		whipForward->set_flip_h(true);
		whipBack->set_flip_h(true);
	}
	if (isCrouching && input->is_action_just_pressed("whip")&&!isStunned&&!isWhipping&&!holdingLedge&&!holdingRope) {
		if (pickedBody == nullptr) {
			for (auto i : *level->hurtboxes) {
				if (body.aabb.overlaps(i->aabb) && i->pickable) {
					level->PlayAudio(level->pickUpSFX, body.aabb.center);
					pickedBody = i;
					i->pickedBy = this;
					break;
				}
			}
		}
		else {
			pickedBody->moveFastHitbox.creatorToEscape = &body;
			pickedBody->pickedBy = nullptr;
			pickedBody = nullptr;
		}
	}  
	else if (input->is_action_just_pressed("whip") && !holdingLedge && !isWhipping && !isStunned) {
		if (pickedBody!=nullptr) {
			if (pickedBody->throwAction==nullptr) {
				pickedBody->moveFastHitbox.creatorToEscape = &body;
				pickedBody->vel = Vector2(2000, -1300);
				level->PlayAudio(level->throwSFX, body.aabb.center);
				if (!body.isFacingRight) {
					pickedBody->vel.x *= -1;
				}
				if (isCrouching) {
					pickedBody->vel = Vector2(400 * (body.isFacingRight? 1 : -1), 0);
				}
				pickedBody->pickedBy = nullptr;
				pickedBody = nullptr;
			}
			else 
			{
				pickedBody->throwAction->DoThrowAction();
			}
		}
		else 
		{
			whipHitbox.SetValues(body.aabb, 1, HitboxMask::Enemy,Vector2(1000*(body.isFacingRight?1:-1),-500) , 0, true);
			whipHitbox.InitOrClearBodiesAlreadyDamagedList();
			whipHitbox.assignCreatorToEscapeToMoveFastHitbox = &body;
			whipHitbox.creatorToEscape = nullptr;
			whipHitbox.autoUnregister = false;
			level->RegisterHitbox(&whipHitbox);
			isWhipping = true;
			playedWhipSound = false;
			animator->set_animation("Whip");
			animator->set_speed_scale(3);
			animator->set_frame(0);
			animator->play();
			whipTime = 0;
		}
	}
	if (pickedBody != nullptr) {
		pickedBody->isFacingRight = body.isFacingRight;
	}
	if (isStunned || isDead) {
		level->UnregisterHitbox(&whipHitbox);
		whipTime = 10;
		isWhipping = false;
	}
	whipTime += delta;
	if (isWhipping) {
		if (whipTime < .3f) 
		{
			whipHitbox.aabb.center = body.aabb.center+Vector2(.9*(body.isFacingRight?-1:1),-.7);
			whipHitbox.aabb.size = Vector2(1, 1);
		}
		else 
		{
			whipHitbox.aabb.center = body.aabb.center+Vector2(.9*(body.isFacingRight?1:-1),.1);
			whipHitbox.aabb.size = Vector2(1.3, .5);
		}
		if (whipTime < .1) {
			whipBack->set_visible(false);
			whipForward->set_visible(false);
		}
		else if (whipTime < .3) {
			if (!playedWhipSound && whipTime>.2) {
				playedWhipSound = true;
				auto audio = get_node<AudioStreamPlayer2D>("WhipAudio");
				audio->set_volume_db(0.0f);
				audio->set_stream(level->whipSFX);
				audio->play();
			}
			whipBack->set_visible(true);
			whipForward->set_visible(false);
		}
		else if (whipTime<.5) {
			whipBack->set_visible(false);
			whipForward->set_visible(true);
		}
		else 
		{
			level->UnregisterHitbox(&whipHitbox);
			isWhipping = false;
		}
	}
	else {
		whipBack->set_visible(false);
		whipForward->set_visible(false);
	}
	if (!isWhipping && !isStunned) {
		if (input->is_action_just_pressed("bomb")) {
			Bomb* bomb = SpawnBomb(level);
			bomb->body.moveFastHitbox.creatorToEscape = &body;
			bomb->startVelocity = Vector2(1300, -1300);
			if (!body.isFacingRight) {
				bomb->startVelocity.x *= -1;
			}
			if (isCrouching) {
				bomb->startVelocity = Vector2(400 * (body.isFacingRight? 1 : -1), 0);
			}
			//bomb->vel += vel;
			bomb->set_position(get_position());
			get_node("/root/GameScene/SpawnRoot")->add_child(bomb);
		}
		if (input->is_action_just_pressed("rope")) {
			Rope* rope = SpawnRope(level);
			rope->body.moveFastHitbox.creatorToEscape = &body;
			rope->set_position(get_position());
			level->add_child(rope);
		}
		grabRopeDisableTime -= delta;
		if (input->is_action_pressed("lookup") && !holdingRope && grabRopeDisableTime <= 0) {
			auto ogPos = get_position();
			auto coord = level->WorldToGrid(ogPos);
			if (level->GetBlock(coord.x, coord.y)->hasRope) {
				holdingRope = true;
				auto gridCoord = level->GridToWorld(Vector2(((int)coord.x) + .5f, coord.y));
				gridCoord.y = ogPos.y - (body.isGrounded ? .2f : 0.0f);
				body.isGrounded = false;
				set_position(gridCoord);
			}
		}
		if (holdingRope) {
			auto ogPos = get_position();
			auto coord = level->WorldToGrid(ogPos);
			if (!level->GetBlock(coord.x, coord.y)->hasRope || body.isGrounded) {
				holdingRope = false;
			}
		}
	}
	if (input->is_action_just_pressed("jump")&&(body.isGrounded||holdingLedge||holdingRope)&&!isStunned) {
		if (holdingRope) {
			grabRopeDisableTime = .1f;
		}
		auto audio = get_node<AudioStreamPlayer2D>("JumpAudio");
		audio->set_volume_db(0.0f);
		audio->set_stream(level->jumpSFX);
		audio->play();
		if (!isCrouching) {
			body.vel.y = -jumpHeight;
		}
		holdingLedge = false;
		holdingRope = false;
	}
	if (holdingRope) {
		body.vel.x = 0;
		isIdle = false;
		auto coord = level->WorldToGrid(get_position());
		if (input->is_action_pressed("lookup") && (level->GetBlock(coord.x,coord.y-1)->hasRope || coord.y-(int)coord.y>.5f)) {
			if (!isWhipping) {
				animator->set_speed_scale(2.5);
				animator->set_animation("Climb");
			}
			body.vel.y = -500;
		}
		else if (isCrouching) {
			if (!isWhipping) {
				animator->set_speed_scale(2.5);
				animator->set_animation("Climb");
			}
			body.vel.y = 500;
		}
		else {
			if (!isWhipping) {
				animator->set_animation("ClimbStill");
			}
			body.vel.y = 0;
		}
	}
	else if (holdingLedge) {
		if (!isWhipping && !isStunned) {
			animator->set_animation("Jump");
		}
		isIdle = false;
	}
	else if (!body.isGrounded) {
		if (!isWhipping && !isStunned) {
			animator->set_animation("Jump");
		}
		isIdle = false;
	}
	bool didSetLook = false;
	if (input->is_action_pressed("left") && !holdingLedge && !isStunned){
		isIdle = false;
		if (!isWhipping) {
			animator->set_flip_h(true);
			body.isFacingRight= false;
		}
		if (!holdingRope) {
			if (isCrouching && body.isGrounded)
			{
				body.vel.x = move_toward(body.vel.x,-walkSpeed / 2,delta*accelSpeed);
				if (!isWhipping) {
					animator->set_speed_scale(2);
					animator->set_animation("Crawl");
				}
			}
			else
			{
				float velTarget = isRunning ? -walkSpeed * 2 : -walkSpeed;
				body.vel.x = move_toward(body.vel.x, velTarget, delta * accelSpeed);
				if (body.isGrounded && !isWhipping) {
					animator->set_animation("Walk");
					if (!isRunning)
						animator->set_speed_scale(2.5);
					else
						animator->set_speed_scale(5);
				}
			}
		}
	}
	else if (input->is_action_pressed("right")&&!holdingLedge && !isStunned) {
		if (!isWhipping) {
			animator->set_flip_h(false);
			body.isFacingRight = true;
		}
		if (!holdingRope) {
			isIdle = false;
			if (isCrouching && body.isGrounded)
			{
				body.vel.x = move_toward(body.vel.x, walkSpeed / 2, delta*accelSpeed);
				if (!isWhipping) {
					animator->set_speed_scale(4);
					animator->set_animation("Crawl");
				}
			}
			else
			{
				float velTarget = isRunning ? walkSpeed * 2 : walkSpeed;
				body.vel.x = move_toward(body.vel.x, velTarget, delta * accelSpeed);
				if (body.isGrounded && !isWhipping) {
					animator->set_animation("Walk");
					if (!isRunning)
						animator->set_speed_scale(2.5);
					else
						animator->set_speed_scale(5);
				}
			}
		}
	} 
	else if (!holdingRope && !isStunned)
	{
		body.vel.x = move_toward(body.vel.x, 0, delta * accelSpeed);
		if (body.isGrounded && isIdle && !isWhipping) {
			if (input->is_action_pressed("lookup")) {
				timeLookingUp += delta;
				didSetLook = true;
				animator->set_animation("LookUp");
			}
			else if (isCrouching){
				animator->set_animation("CrouchStill");
			}
			else {
				animator->set_animation("StandStill");
			}
		}
	}
	if (isCrouching && body.isGrounded &&!isStunned) {
		timeLookingDown += delta;
		didSetLook = true;
	}
	if (!didSetLook) 
	{
		timeLookingDown = 0;
		timeLookingUp = 0;
	}
	if (timeLookingDown > .3f) 
	{
		camera->set_position(Vector2(0, 700));
	}
	else if (timeLookingUp >.3)
	{
		camera->set_position(Vector2(0, -700));
	}
	else 
	{
		camera->set_position(Vector2(0, 0));
	}
	if (body.isGrounded && !wasGrounded && ogVel.y>0 && !isDead)
	{
		if (ogVel.y > 2600) {
			auto audio = get_node<AudioStreamPlayer2D>("JumpAudio");
			audio->set_volume_db(0.0f);
			audio->set_stream(level->hitSFX);
			audio->play();
			TakeDamage(1,true,nullptr);
		}
		else {
			auto audio = get_node<AudioStreamPlayer2D>("JumpAudio");
			audio->set_volume_db(-5.0f);
			audio->set_stream(level->landSFX);
			audio->play();
		}
	}
	{
		auto coord = level->WorldToGrid(get_position());
		float footHeight = godot::Math::fmod((coord.y + body.aabb.center.y + body.aabb.size.y / 2),1.0f);
		auto block = level->GetBlock(coord.x, coord.y);
		if (block->hasSpikes && body.vel.y>0 && !isDead && footHeight>.5f) {
			auto audio = get_node<AudioStreamPlayer2D>("JumpAudio");
			audio->set_volume_db(0.0f);
			audio->set_stream(level->skewerSFX);
			audio->play();
			block->bloody = true;
			level->UpdateMeshes();
			body.vel.x = 0;
			body.friction = 1000000;
			Die();
		}
	}
	wasGrounded = body.isGrounded;
}
Spelunker::Spelunker() {
	printf("const");
}
Spelunker::~Spelunker() {
	if (whipHitbox.bodiesAlreadyDamaged != nullptr) {
		delete whipHitbox.bodiesAlreadyDamaged;
	}
	printf("dest");
}
