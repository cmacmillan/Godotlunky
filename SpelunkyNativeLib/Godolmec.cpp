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

	register_method("FireBomb", &Godolmec::FireBomb);
	register_method("FinishAnimation", &Godolmec::FinishAnimation);
	register_method("StopShaking", &Godolmec::StopShaking);
}

void Godolmec::_init(){}

void Godolmec::_ready()
{
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	level->godolmec = this;//must nullify on free
	animPlayer = get_node<AnimationPlayer>("AnimationPlayer");
	topPoint = get_node<Node2D>("Jaw/Face/TopPoint");
	bottomPoint = get_node<Node2D>("Jaw/BottomPoint");
	faceRoot = get_node<Node2D>("Jaw/Face");
	health = 4;
	float center;
	Vector2 bounds = GetBodyBounds(center);
	body.Init(Vector2(4.3f,(bounds.x-bounds.y)),Vector2(0,center),0,100000,this,level,Vector2(0,0),false,100,HitboxMask::Nothing,nullptr,nullptr,false,false,nullptr,nullptr);
	level->RegisterHurtbox(&body);
	level->customCollision->insert(&jawHitbox1);
	level->customCollision->insert(&faceHitbox1);
	level->customCollision->insert(&faceHitbox2);
	bombSpot1 = get_node<Node2D>("Jaw/Face/FireFlash1");
	bombSpot2 = get_node<Node2D>("Jaw/Face/FireFlash2");
	bombSpot3 = get_node<Node2D>("Jaw/Face/FireFlash3");
	bombSpot4 = get_node<Node2D>("Jaw/Face/FireFlash4");
	jawRedFlash = get_node<Sprite>("Jaw/JawOutline");
	faceRedFlash = get_node<Sprite>("Jaw/Face/FaceOutline");
	jawCrack1 = get_node<Sprite>("Jaw/JawCrack1");
	jawCrack2 = get_node<Sprite>("Jaw/JawCrack2");
	jawCrack3 = get_node<Sprite>("Jaw/JawCrack3");
	faceCrack1 = get_node<Sprite>("Jaw/Face/FaceCrack1");
	faceCrack2 = get_node<Sprite>("Jaw/Face/FaceCrack2");
	faceCrack3 = get_node<Sprite>("Jaw/Face/FaceCrack3");
	isTakingDamage = false;
	jawHitbox1.root = &body;
	faceHitbox1.root = &body;
	faceHitbox2.root = &body;
	jawHitbox1.aabb.size = Vector2(4.3f, .85f);
	faceHitbox1.aabb.size = Vector2(4.3f,2.3f);
	faceHitbox2.aabb.size = Vector2(2.6f,1.f);
	wasGrounded = true;
	doorOpenerSpot = get_node<Node2D>("Jaw/DoorOpenerSpawnPoint");
	auto doorOpener = SpawnDoorOpener(level, level->WorldToGrid(doorOpenerSpot->get_global_position()));
	flashTime = 0;
	doorOpener->isGodolmec = true;
	SwitchState(GodolmecState::WaitingToBreakFree);
	SetColliderPositions();
}

void Godolmec::TakeDamage() {
	level->PlayAudio(level->godolmecTakeDamageSFX, body.aabb.center);
	health--;
	switch (health)
	{
	case 3:
		jawCrack1->set_visible(true);
		faceCrack1->set_visible(true);
		break;
	case 2:
		jawCrack1->set_visible(false);
		faceCrack1->set_visible(false);
		jawCrack2->set_visible(true);
		faceCrack2->set_visible(true);
		break;
	case 1:
		jawCrack2->set_visible(false);
		faceCrack2->set_visible(false);
		jawCrack3->set_visible(true);
		faceCrack3->set_visible(true);
		break;
	case 0:
		break;
	default:
		break;
	}
	isTakingDamage = true;
	flashTime = 0;
	flashDirection = 1;
	flashOpacity = 0;
}

void Godolmec::StopShaking() {
	level->shakeFromGodolmec = false;
}

void Godolmec::FinishAnimation() {
	level->spelunker->frozenInCutscene = false;
	level->lookAtGodolmec=false;
	level->lookAtGodolmecTime = 0.0f;
}

void Godolmec::FireBomb(int index) 
{
	Node2D* spot = nullptr;
	switch (index)
	{
	case 0:
		spot = bombSpot1;
		break;
	case 1:
		spot = bombSpot2;
		break;
	case 2:
		spot = bombSpot3;
		break;
	case 3:
		spot = bombSpot4;
		break;
	default:
		break;
	}
	SpawnBomb(level,level->WorldToGrid(spot->get_global_position()),2000*level->WorldToGridSize(spot->get_transform().xform(Vector2(0,1))));
}

void Godolmec::SwitchState(GodolmecState targetState) 
{
	switch (targetState) {
	case GodolmecState::BreakingFree:
		animPlayer->set_current_animation("BreakFree");
		break;
	case GodolmecState::FiringBombs:
		animPlayer->set_current_animation("FireBombs");
		break;
	case GodolmecState::JumpingAtPlayer: 
	{
		body.vel = Vector2(0, -2000);
	}
		break;
		break;
	case GodolmecState::WaitingToSwitchStatesForceJump:
	case GodolmecState::WaitingToSwitchStates:
	case GodolmecState::WaitingToBreakFree:
		break;
	}
	stateTime = 0;
	state = targetState;
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
	jawHitbox1.aabb.center = level->WorldToGrid(get_global_position())+Vector2(0,1.8);
	faceHitbox1.aabb.center = level->WorldToGrid(faceRoot->get_global_position())+Vector2(0,0.f);
	faceHitbox2.aabb.center = level->WorldToGrid(faceRoot->get_global_position())+Vector2(0,-1.2f);
}

void Godolmec::_process(float delta)
{
	SetColliderPositions();
	body.process(delta, true, true);
	stateTime += delta;
	switch (state)
	{
	case GodolmecState::WaitingToSwitchStatesForceJump:
		if (stateTime > 1.0f) {
			SwitchState(GodolmecState::JumpingAtPlayer);
		}
		break;
	case GodolmecState::WaitingToSwitchStates:
		if (stateTime > 1.0f) {
			if (level->Random() < .85f) {
				SwitchState(GodolmecState::JumpingAtPlayer);
			}
			else {
				SwitchState(GodolmecState::FiringBombs);
			}
		}
		break;
	case GodolmecState::FiringBombs:
		if (stateTime > 5.0f) {
			SwitchState(GodolmecState::WaitingToSwitchStatesForceJump);
		}
		break;
	case GodolmecState::BreakingFree:
		if (stateTime > 10.0f) {
			SwitchState(GodolmecState::JumpingAtPlayer);
		}
		break;
	case GodolmecState::JumpingAtPlayer:
		if (body.isGrounded) {
			SwitchState(GodolmecState::WaitingToSwitchStates);
		}
		else 
		{
			auto target = level->spelunker->body.aabb.center;
			float x = godot::Math::sign(target.x - body.aabb.center.x);
			body.vel.x = x*600;
		}
		break;
	default:
		break;
	}
	if (isTakingDamage) {
		flashTime += delta;
		flashOpacity = godot::Math::move_toward(flashOpacity, flashDirection, delta * 10);
		if (flashOpacity == flashDirection) {
			flashDirection = 1 - flashDirection;
		}
		faceRedFlash->set_modulate(Color(1, 0, 0, flashOpacity));
		jawRedFlash->set_modulate(Color(1, 0, 0, flashOpacity));
		if (flashOpacity == 0 && flashTime > 1.5f) {
			isTakingDamage = false;
		}
	}
	if (!wasGrounded && body.isGrounded && state!=GodolmecState::WaitingToBreakFree) {
		level->PlayAudio(level->godolmecHitSFX, body.aabb.center);
	}
	wasGrounded = body.isGrounded;
}
