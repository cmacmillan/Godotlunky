#include "Spelunker.h"
#include <Input.hpp>
#include "Level.h"
#include <AnimatedSprite.hpp>
using namespace godot::Math;

void Spelunker::_register_methods()
{
	register_method("_ready", &Spelunker::_ready);
	register_method("_process", &Spelunker::_process);

	register_property("jumpHeight", &Spelunker::jumpHeight, 0.0f);
	register_property("walkSpeed", &Spelunker::walkSpeed, 0.0f);
	register_property("gravity", &Spelunker::gravity, 0.0f);
}

void Spelunker::_init()
{
	printf("init");
}

Vector2 vel;
Vector2 startPos;
bool inited = false;
bool holdingLedge = false;
bool facingRight = true;
Vector2 ledgeCoords;
void Spelunker::_ready()
{
}

void Spelunker::_process(float delta)
{
	auto animator = (AnimatedSprite*)get_node(".");
	if (!inited) {
		inited = true;
		Level* level = (Level*)this->get_node("/root/GameScene/Level");
		startPos = level->WorldToGrid(get_position());
	}
	Level* level = (Level*)this->get_node("/root/GameScene/Level");
	SpelAABB aabb = SpelAABB();
	Vector2 offset = Vector2(0,.08f);
	if (!holdingLedge) {
		vel.y += gravity * delta;
	}
	aabb.size = Vector2(.72f, .96f);
	aabb.center = level->WorldToGrid(get_position()+vel*delta)+offset;
	Vector2 normal;
	Vector2 finalPos=aabb.center;
	bool isGrounded;
	float bounciness = 0;
	if (!holdingLedge) {
		if (level->CheckCollisionWithTerrain(aabb, startPos, finalPos, normal, isGrounded))
		{
			if (normal.x != 0 && sign(normal.x) != sign(vel.x)) {
				vel.x = -vel.x * bounciness;
			}
			if (normal.y != 0 && sign(normal.y) != sign(vel.y)) {
				vel.y = -vel.y * bounciness;
			}
			//could do dot product reflection like this, but that produces bad corner behaviour
			//float proj = vel.dot(-normal);
			//vel = vel + normal * proj + normal * proj * bounciness;
			Vector2 coordsAhead = finalPos + Vector2((facingRight ? 1 : -1), 0);
			Vector2 coordsUp = coordsAhead - Vector2(0, 1);
			if (normal.x == (facingRight ? -1 : 1) && vel.y > 0 && level->GetBlock(coordsAhead.x,coordsAhead.y)->present && !level->GetBlock(coordsUp.x,coordsUp.y)->present && godot::Math::fmod(finalPos.y,1)<.3f)
			{
				holdingLedge = true;
				ledgeCoords = finalPos;
				ledgeCoords.y = godot::Math::floor(finalPos.y)+.15f;
				vel.x = 0;
				vel.y = 0;
			}
			else
			{
				holdingLedge = false;
			}
		}
		set_position(level->GridToWorld(finalPos-offset));
		startPos = finalPos;
	}
	else 
	{
		startPos = ledgeCoords;
		set_position(level->GridToWorld(ledgeCoords));
	}

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
	if (input->is_action_just_pressed("jump")&&(isGrounded||holdingLedge)) {
		vel.y = -jumpHeight;
		holdingLedge = false;
	}
	if (!isGrounded) {
		animator->set_animation("Jump");
		isIdle = false;
	}
	if (input->is_action_pressed("left")){
		isIdle = false;
		animator->set_flip_h(true);
		facingRight = false;
		if (isCrouching && isGrounded)
		{
			vel.x = -walkSpeed / 2;
			animator->set_animation("Crawl");
		}
		else 
		{
			vel.x = isRunning ? -walkSpeed * 2 : -walkSpeed;
			if (isGrounded) {
				animator->set_animation("Walk");
				if (!isRunning)
					animator->set_speed_scale(2.5);
				else
					animator->set_speed_scale(5);
			}
		}
	}
	else if (input->is_action_pressed("right")) {
		isIdle = false;
		animator->set_flip_h(false);
		facingRight = true;
		if (isCrouching && isGrounded)
		{
			vel.x = walkSpeed / 2;
			animator->set_speed_scale(4);
			animator->set_animation("Crawl");
		}
		else
		{
			vel.x = isRunning ? walkSpeed * 2 : walkSpeed;
			if (isGrounded) {
				animator->set_animation("Walk");
				if (!isRunning)
					animator->set_speed_scale(2.5);
				else
					animator->set_speed_scale(5);
			}
		}
	} 
	else {
		vel.x = 0;
		if (isGrounded && isIdle) {
			if (input->is_action_pressed("lookup")) {
				animator->set_animation("LookUp");
			} 
			else if (isCrouching) {
				animator->set_animation("CrouchStill");
			}
			else {
				animator->set_animation("StandStill");
			}
		}
	}
}
Spelunker::Spelunker() {
	printf("const");
}
Spelunker::~Spelunker() {
	printf("dest");
}
