#include "Spelunker.h"
#include <Input.hpp>
#include "Level.h"
#include <AnimatedSprite.hpp>
#include "Bomb.h"
#include "Rope.h"
#include <ResourceLoader.hpp>
using namespace godot::Math;

void Spelunker::_register_methods()
{
	register_method("_ready", &Spelunker::_ready);
	register_method("_process", &Spelunker::_process);

	register_property("jumpHeight", &Spelunker::jumpHeight, 0.0f);
	register_property("walkSpeed", &Spelunker::walkSpeed, 0.0f);
	//register_property("bombScene", &Spelunker::bombScene, Ref<PackedScene>());
}

void Spelunker::_init()
{
	printf("init");
}

void Spelunker::_ready()
{
}

void Spelunker::_process(float delta)
{
	auto animator = get_node<AnimatedSprite>(".");
	if (!inited) {
		inited = true;
		level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
		startPos = level->WorldToGrid(get_position());
	}
	SpelAABB aabb = SpelAABB();
	Vector2 offset = Vector2(0,.11f);
	if (!holdingLedge && !holdingRope) {
		vel.y += level->g * delta;
	}
	aabb.size = Vector2(.72f, .9f);
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
	if (input->is_action_just_pressed("bomb")) {
		Bomb* bomb = Object::cast_to<Bomb>(((Ref<PackedScene>)ResourceLoader::get_singleton()->load("res://Bomb.tscn"))->instance());
		if (!facingRight) {
			bomb->vel.x *= -1; 
		}
		if (isCrouching) {
			bomb->vel = Vector2(400, 0);
		}
		//bomb->vel += vel;
		bomb->set_position(get_position());
		level->add_child(bomb);
	}
	if (input->is_action_just_pressed("rope")) {
		Rope* rope = Object::cast_to<Rope>(((Ref<PackedScene>)ResourceLoader::get_singleton()->load("res://Rope.tscn"))->instance());
		rope->set_position(get_position());
		level->add_child(rope);
	}
	grabRopeDisableTime -= delta;
	if (input->is_action_pressed("lookup") && !holdingRope && grabRopeDisableTime<=0) {
		auto ogPos = get_position();
		auto coord = level->WorldToGrid(ogPos);
		if (level->GetBlock(coord.x,coord.y)->hasRope){
			holdingRope = true;
			auto gridCoord = level->GridToWorld(Vector2(((int)coord.x)+.5f,coord.y));
			gridCoord.y = ogPos.y-(isGrounded?.2f:0.0f);
			isGrounded = false;
			set_position(gridCoord);
		}
	}
	if (holdingRope) {
		auto ogPos = get_position();
		auto coord = level->WorldToGrid(ogPos);
		if (!level->GetBlock(coord.x,coord.y)->hasRope || isGrounded){
			holdingRope = false;
		}
	}
	if (input->is_action_just_pressed("jump")&&(isGrounded||holdingLedge||holdingRope)) {
		if (holdingRope) {
			grabRopeDisableTime = .1f;
		}
		vel.y = -jumpHeight;
		holdingLedge = false;
		holdingRope = false;
	}
	if (holdingRope) {
		vel.x = 0;
		isIdle = false;
		auto coord = level->WorldToGrid(get_position());
		if (input->is_action_pressed("lookup") && (level->GetBlock(coord.x,coord.y-1)->hasRope || coord.y-(int)coord.y>.5f)) {
			animator->set_speed_scale(2.5);
			animator->set_animation("Climb");
			vel.y = -500;
		}
		else if (isCrouching) {
			animator->set_speed_scale(2.5);
			animator->set_animation("Climb");
			vel.y = 500;
		}
		else {
			animator->set_animation("ClimbStill");
			vel.y = 0;
		}
	}
	else if (holdingLedge) {
		animator->set_animation("Jump");
		isIdle = false;
	}
	else if (!isGrounded) {
		animator->set_animation("Jump");
		isIdle = false;
	}
	if (input->is_action_pressed("left") && !holdingLedge){
		isIdle = false;
		animator->set_flip_h(true);
		facingRight = false;
		if (!holdingRope) {
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
	}
	else if (input->is_action_pressed("right")&&!holdingLedge) {
		animator->set_flip_h(false);
		facingRight = true;
		if (!holdingRope) {
			isIdle = false;
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
	} 
	else if (!holdingRope)
	{
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
