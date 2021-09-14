#include "Godolmec.h"
#include "Level.h"
#include <Input.hpp>
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>
#include <AudioStreamPlayer2D.hpp>
using namespace godot::Math;

void Godolmec::_register_methods()
{
	register_method("_ready", &Godolmec::_ready);
	register_method("_process", &Godolmec::_process);
}

void Godolmec::_init(){}

void Godolmec::_ready()
{
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	animPlayer = get_node<AnimationPlayer>("AnimationPlayer");
	topPoint = get_node<Node2D>("Jaw/Face/TopPoint");
	bottomPoint = get_node<Node2D>("Jaw/BottomPoint");
	faceRoot = get_node<Node2D>("Jaw/Face");
	float center;
	Vector2 bounds = GetBodyBounds(center);
	body.Init(Vector2(4.3f,(bounds.x-bounds.y)),Vector2(0,center),0,100000,this,level,Vector2(0,0),false,100,HitboxMask::Nothing,nullptr,nullptr,false,true,level->godolmecHitSFX,nullptr);
	level->RegisterHurtbox(&body);
	level->customCollision->insert(&jawHitbox1);
	level->customCollision->insert(&faceHitbox1);
	level->customCollision->insert(&faceHitbox2);
	jawHitbox1.root = &body;
	faceHitbox1.root = &body;
	faceHitbox2.root = &body;
	jawHitbox1.aabb.size = Vector2(4.3f, .65f);
	faceHitbox1.aabb.size = Vector2(4.3f,2.3f);
	faceHitbox2.aabb.size = Vector2(2.6f,1.f);
	animPlayer->set_current_animation("BreakFree");
	SetColliderPositions();
}

Vector2 Godolmec::GetBodyBounds(float& center) {
	float bottom = level->WorldToGrid(bottomPoint->get_global_position()).y;
	float top = level->WorldToGrid(topPoint->get_global_position()).y;
	center = ((bottom + top) * .5f)-level->WorldToGrid(get_global_position()).y;
	return Vector2(bottom,top);
}

void Godolmec::SetColliderPositions() {
	float center;
	Vector2 bounds = GetBodyBounds(center);
	body.startPos.y += center-body.offset.y;
	body.offset.y = center;
	body.aabb.size = Vector2(4.3f, (bounds.x - bounds.y));
	jawHitbox1.aabb.center = level->WorldToGrid(get_global_position())+Vector2(0,1.95);
	faceHitbox1.aabb.center = level->WorldToGrid(faceRoot->get_global_position())+Vector2(0,0.f);
	faceHitbox2.aabb.center = level->WorldToGrid(faceRoot->get_global_position())+Vector2(0,-1.2f);
}

void Godolmec::_process(float delta)
{
	SetColliderPositions();
	body.process(delta,true,true);
}
