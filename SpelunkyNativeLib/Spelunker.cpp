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
#include <string>
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


bool Spelunker::TakeDamage(int damageAmount,bool stun,vector<HitboxData*>* hitboxesToRemove,DamageSource source) {
	if (invulTime <= 0 && !isDead && !isEnteringDoor) {
		if (stun) {
			isStunned = true;
			stunTime = 0;
		}
		health -= damageAmount;
		if (health <= 0) {
			health = 0;
			Die(source);
			return false;//keep the hitbox active even after death
		}
		else {
			level->PlayAudio(level->hitSFX, body.aabb.center);
			for (int i = 0; i < 3; i++) {
				SpawnBloodSpurt(level, body.aabb.center);
			}
			invulTime = 2;
		}
	}
	return false;
}
void Spelunker::Die(DamageSource source,bool playSound) {
	if (!isDead && !isEnteringDoor) {
		globals->shouldRead = false;
		if (playSound)
			level->PlayAudio(level->hitSFX, body.aabb.center);
		for (int i = 0; i < 10; i++) {
			SpawnBloodSpurt(level, body.aabb.center);
		}
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

void Spelunker::TakeSmush() {
	level->PlayAudio(level->smushSFX, body.aabb.center);
	get_node<AnimatedSprite>(".")->set_visible(false);
	Die(DamageSource::MushedDamage,false);
}
void Spelunker::_ready()
{
	spaceTextLerp = 0;
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	set_position(level->GridToWorld(level->spawnPos));
	body.Init(Vector2(.72f, .9f), Vector2(0, .11f), 0, 5000, this, level, Vector2(0, 0), false, 1, HitboxMask::Player, this, nullptr, false, false, nullptr, this, HeldItem::Unknown);
	camera = Object::cast_to<Camera2D>(get_node("/root/GameScene/CameraTarget/Camera2D"));
	whipForward = get_node<Sprite>("WhipForward");
	whipBack = get_node<Sprite>("WhipBack");
	frozenInCutscene = false;
	isWhipping = false;
	isDead = false;
	deadTime = 0;
	isStunned = false;
	body.isFacingRight = true;
	pickedBody = nullptr;
	level->RegisterHurtbox(&body);
	level->spelunker = this;
	level->cameraTarget->set_position(get_position());

	globals = get_node<Globals>("/root/Globals");
	music = get_node<Music>("/root/Music");
	if (globals->shouldRead) {
		health = globals->healthCount;
		ropeCount = globals->ropeCount;
		bombCount = globals->bombCount;
		goldCollected = globals->cashCount;
		levelIndex = globals->levelIndex;
		Body* spawned = nullptr;
		switch (globals->heldItem) {
		case HeldItem::Shotgun:
			spawned = &SpawnShotgun(level, body.aabb.center, this)->body;
			break;
		case HeldItem::Rock:
			spawned = &SpawnRock(level, body.aabb.center, this)->body;
			break;
		case HeldItem::PrizeBox:
			spawned = &SpawnPrizeBox(level, body.aabb.center, this)->body;
			break;
		case HeldItem::Unknown:
			break;
		}
		if (spawned != nullptr) {
			pickedBody = spawned;
		}
		if (levelIndex == 3) {
			music->nextAudio = nullptr;
		}
	}
	else
	{
		health = 4;
		ropeCount = 4;
		bombCount = 4;
		goldCollected = 0;
		globals->levelIndex = 0;
		levelIndex = 0;
		music->nextAudio = music->caveThemeMusic;
		music->currentVolume = 1.0f;
		music->audioSource->set_stream(music->caveThemeMusic);
	}
}

Vector2 ledgeFlipOffsets[7] = {
	Vector2(.7f,-.7f),
	Vector2(.6f,-.7f),
	Vector2(.5f,-.4f),
	Vector2(.3f,-.3f),
	Vector2(.15f,-.15f),
	Vector2(0.0f,0),
	Vector2(0,0),
};

void Spelunker::_process(float delta)
{
	if (level->isFadingOut) {
		return;
	}
	if (isDead) {
		deadTime += delta;
		if (deadTime > 1.5f) {
			level->isFadingOut = true;
			level->fadeOutLerp = 1;
			level->PlayAudio(level->fadeOutSFX,body.aabb.center);
		}
	}
	if (body.isSmushed)
		return;
	auto animator = get_node<AnimatedSprite>(".");
	if (isEnteringDoor) {
		enterDoorOpacity = godot::Math::move_toward(enterDoorOpacity,0,delta);
		animator->set_modulate(Color(1, 1, 1, enterDoorOpacity));
		if (enterDoorOpacity == 0 && !level->isFadingOut) {
			level->isFadingOut = true;
			level->fadeOutLerp = 1;
			level->PlayAudio(level->fadeOutSFX,body.aabb.center);
		}
		return;
	}
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
			if (!frozenInCutscene&&!isStunned&&!isWhipping&&body.normal.x == (body.isFacingRight? -1 : 1) && body.vel.y > 0 && level->GetBlock(coordsAhead.x,coordsAhead.y)->present && !level->GetBlock(coordsUp.x,coordsUp.y)->present && godot::Math::fmod(body.endPos.y,1)<.3f)
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

	auto footAABB = SpelAABB();
	float ogHeight = body.aabb.size.y;
	footAABB.center = body.aabb.center + Vector2(0, ogHeight / 4);
	footAABB.size = body.aabb.size + Vector2(0, -ogHeight / 2);
	footHitbox.SetValues(footAABB, 1, HitboxMask::Enemy, Vector2(0, 0), 0, true, &body,DamageSource::SpelunkerDamage);

	float accelSpeed=20000;

	auto input = Input::get_singleton();

	if (body.isGrounded && !frozenInCutscene && !isStunned && !isDead && body.aabb.overlaps(level->exitPosition) && level->isDoorOpen) {
		if (input->is_action_pressed("EnterDoor")) {
			body.aabb.center = level->exitPosition.center;
			set_position(level->GridToWorld(body.aabb.center));
			isEnteringDoor = true;
			globals->shouldRead = true;
			if (pickedBody != nullptr) {
				globals->heldItem = pickedBody->heldItemType;
				if (pickedBody->heldItemType == HeldItem::Unknown) {
					pickedBody->pickedBy = nullptr;
					pickedBody = nullptr;
				}
			}
			else 
			{
				globals->heldItem = HeldItem::Unknown;
			}
			globals->levelIndex++;
			globals->healthCount = health;
			globals->ropeCount = ropeCount;
			globals->bombCount = bombCount;
			globals->cashCount = goldCollected;
			animator->set_animation("EnterDoor");
			animator->set_speed_scale(2);
			level->PlayAudio(level->walkThroughDoorSFX, body.aabb.center);
			return;
		}
		else
		{
			spaceTextLerp = godot::Math::move_toward(spaceTextLerp, 1, delta * 8);
		}
	}
	else
	{
		spaceTextLerp = godot::Math::move_toward(spaceTextLerp, 0, delta * 8);
	}
	level->exitDoor->get_node<Label>("LabelParent/Label")->set_modulate(Color(1,1,1,spaceTextLerp));
	level->exitDoor->get_node<Node2D>("LabelParent")->set_position(Vector2(0,godot::Math::lerp(-80,-100,spaceTextLerp)));

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


	if (isCrouching && input->is_action_just_pressed("whip")&&!isStunned&&!frozenInCutscene&&!isWhipping&&!holdingLedge&&!holdingRope) {
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
	else if (input->is_action_just_pressed("whip") && !holdingLedge && !isWhipping && !isStunned && !frozenInCutscene) {
		if (pickedBody != nullptr) {
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
			whipHitbox.SetValues(body.aabb, 1, HitboxMask::ItemAndEnemy,Vector2(1000*(body.isFacingRight?1:-1),-500) , 0, true,nullptr,DamageSource::SpelunkerDamage);
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
	if (!isWhipping && !isStunned && !frozenInCutscene) {
		if (input->is_action_just_pressed("bomb")) {
			if (bombCount > 0) {
				bombCount--;
				auto startVelocity = Vector2(1500, -1300);
				if (!body.isFacingRight) {
					startVelocity.x *= -1;
				}
				if (isCrouching) {
					startVelocity = Vector2(400 * (body.isFacingRight ? 1 : -1), 0);
				}
				Bomb* bomb = SpawnBomb(level, body.aabb.center, startVelocity);
				level->PlayAudio(level->throwSFX, body.aabb.center);
				bomb->body.moveFastHitbox.creatorToEscape = &body;
			}
			else 
			{
				level->PlayAudio(level->noneLeftSFX, body.aabb.center);
			}
		}
		if (input->is_action_just_pressed("rope")) {
			if (ropeCount > 0) {
				ropeCount--;
				if (isCrouching && !holdingLedge) {
					Vector2 spawnPos = body.aabb.center+Vector2(body.isFacingRight ? 1 : -1, 0);
					spawnPos.y = godot::Math::floor(spawnPos.y) - .5f;
					Rope* rope = SpawnRope(level, spawnPos, Vector2(0, 0));
				}
				else if (isCrouching && animator->get_animation() == "FlippingOntoLedge")
				{
					Vector2 spawnPos = baseLedgeCoords;
					Rope* rope = SpawnRope(level, spawnPos, Vector2(0, 0));
				}
				else if (isCrouching && holdingLedge) {
					Vector2 spawnPos = body.aabb.center;
					Rope* rope = SpawnRope(level, spawnPos, Vector2(0, 0));
				}
				else {
					Rope* rope = SpawnRope(level, body.aabb.center, Vector2(0, -2100));
					rope->body.moveFastHitbox.creatorToEscape = &body;
				}
			} 
			else
			{
				level->PlayAudio(level->noneLeftSFX, body.aabb.center);
			}
		}
		grabRopeDisableTime -= delta;
		if (input->is_action_pressed("lookup") && !holdingLedge && !holdingRope && grabRopeDisableTime <= 0) {
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
	if (input->is_action_just_pressed("jump")&&(body.isGrounded||holdingLedge||holdingRope)&&!isStunned&&!frozenInCutscene) {
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
		if (holdingLedge) {
			body.isFacingRight = !body.isFacingRight;
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
		if (!isWhipping && !isStunned&&!frozenInCutscene) {
			if (animator->get_animation() != "FlippingOntoLedge" || animator->get_frame()==6) {
				animator->set_animation("HoldingLedge");
			}
			else 
			{
				float sign = -1;
				if (body.isFacingRight)
					sign = 1;
				auto offset = ledgeFlipOffsets[animator->get_frame()];//inb4 crash esketit :P
				offset.x *= sign;
				ledgeCoords = baseLedgeCoords + offset;
			}
			body.aabb.center = ledgeCoords;
			animator->set_flip_h(body.isFacingRight);
		}
		isIdle = false;
	}
	else if (!body.isGrounded) {
		if (!isWhipping && !isStunned&&!frozenInCutscene) {
			animator->set_animation("Jump");
		}
		isIdle = false;
	}
	bool didSetLook = false;
	if (input->is_action_pressed("left") && !holdingLedge && !isStunned&&!frozenInCutscene){
		isIdle = false;
		if (!isWhipping) {
			animator->set_flip_h(true);
			body.isFacingRight= false;
		}
		if (!holdingRope) {
			if (isCrouching && body.isGrounded)
			{
				Vector2 pos = body.aabb.center;
				float xOffset = pos.x + .5f;
				if (!level->GetBlock(xOffset - 1, pos.y + 1)->present && level->GetBlock(xOffset, pos.y + 1)->present) {
					holdingLedge = true;
					baseLedgeCoords = Vector2(godot::Math::floor(xOffset)-.4f,godot::Math::floor(pos.y)+1.15f);
					ledgeCoords = baseLedgeCoords+ledgeFlipOffsets[0];
					body.isFacingRight = true;
					animator->set_speed_scale(2.5f);
					animator->set_animation("FlippingOntoLedge");
					animator->play();
				}
				else {
					body.vel.x = move_toward(body.vel.x, -walkSpeed / 2, delta * accelSpeed);
					if (!isWhipping) {
						animator->set_speed_scale(2);
						animator->set_animation("Crawl");
					}
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
	else if (input->is_action_pressed("right")&&!holdingLedge && !isStunned&&!frozenInCutscene) {
		if (!isWhipping) {
			animator->set_flip_h(false);
			body.isFacingRight = true;
		}
		if (!holdingRope) {
			isIdle = false;
			if (isCrouching && body.isGrounded)
			{
				Vector2 pos = body.aabb.center;
				float xOffset = pos.x - .5f;
				if (!level->GetBlock(xOffset + 1, pos.y + 1)->present && level->GetBlock(xOffset, pos.y + 1)->present) {
					holdingLedge = true;
					baseLedgeCoords = Vector2(godot::Math::floor(xOffset)+1.4f,godot::Math::floor(pos.y)+1.15f);
					auto offset = ledgeFlipOffsets[0];
					offset.x *= -1;
					ledgeCoords = baseLedgeCoords + offset;
					body.isFacingRight = false;
					animator->set_speed_scale(2.5f);
					animator->set_animation("FlippingOntoLedge");
					animator->play();
				}
				else {
					body.vel.x = move_toward(body.vel.x, walkSpeed / 2, delta * accelSpeed);
					if (!isWhipping) {
						animator->set_speed_scale(4);
						animator->set_animation("Crawl");
					}
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
	else if (!holdingRope && !isStunned && !frozenInCutscene)
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
	if (isCrouching && body.isGrounded &&!isStunned&&!frozenInCutscene&&!holdingLedge &&!input->is_action_pressed("right") && !input->is_action_pressed("left")) {
		timeLookingDown += delta;
		didSetLook = true;
	}
	if (!didSetLook) 
	{
		timeLookingDown = 0;
		timeLookingUp = 0;
	}
	if (timeLookingDown > .5f) 
	{
		camera->set_position(Vector2(0, 700));
	}
	else if (timeLookingUp >.5f)
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
			TakeDamage(1,true,nullptr,DamageSource::FallDamage);
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
			Die(DamageSource::SpikesDamage);
		}
	}
	wasGrounded = body.isGrounded;

	level->healthCountLabel->set_text(String(std::to_string(this->health).c_str()));
	level->bombCountLabel->set_text(String(std::to_string(this->bombCount).c_str()));
	level->ropeCountLabel->set_text(String(std::to_string(this->ropeCount).c_str()));
	level->moneyCountLabel->set_text(String(std::to_string(this->goldCollected).c_str()));
	level->levelIndexLabel->set_text(String((std::to_string(this->levelIndex+1)+"/4").c_str()));
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
