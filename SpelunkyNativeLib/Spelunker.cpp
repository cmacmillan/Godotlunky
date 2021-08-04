#include "Spelunker.h"
#include <Input.hpp>
#include "Level.h"
using namespace godot::Math;

void Spelunker::_register_methods()
{
	register_method("_ready", &Spelunker::_ready);
	register_method("_process", &Spelunker::_process);

	register_property("jumpHeight", &Spelunker::jumpHeight, 0.0f);
}

void Spelunker::_init()
{
	printf("init");
}

Vector2 vel;
Vector2 startPos;
bool inited = false;
void Spelunker::_ready()
{
}

void Spelunker::_process(float delta)
{
	//set_position(get_position()+vel);
	printf("%f    ", delta);
	if (!inited) {
		inited = true;
		vel = Vector2(-1000,-1000);
		Level* level = (Level*)this->get_node("/root/GameScene/Level");
		startPos = level->WorldToGrid(get_position());
	}
	Level* level = (Level*)this->get_node("/root/GameScene/Level");
	SpelAABB aabb = SpelAABB();
	Vector2 offset = Vector2(0,.08f);
	//vel.y += 1000.0f * delta;
	aabb.size = Vector2(.72f, .96f);
	aabb.center = level->WorldToGrid(get_position()+vel*delta)+offset;
	//aabb.size = Vector2(.25f, .25f);
	//aabb.size = Vector2(.99f, .99f);
	//aabb.size = Vector2(.01f, .01f);
	Vector2 normal;
	Vector2 finalPos=aabb.center;
	//printf("(%f,%f)", startPos.x,startPos.y);
	float bounciness = 1;
	if (level->CheckCollisionWithTerrain(aabb, startPos, finalPos, normal)) 
	{
		if (normal.x!=0 && sign(normal.x) != sign(vel.x)) {
			vel.x = -vel.x * bounciness;
		}
		if (normal.y!=0 && sign(normal.y) != sign(vel.y)) {
			vel.y = -vel.y * bounciness;
		}
		//could do dot product reflection like this, but that produces bad corner behaviour
		//float proj = vel.dot(-normal);
		//vel = vel + normal * proj + normal * proj * bounciness;
	}

	//printf("(%f,%f)", finalPosRef.x, finalPosRef.y);
	set_position(level->GridToWorld(finalPos-offset));
	startPos = finalPos;
	//
	//i += delta;
	//set_position(Level::singleton->GridToWorld(Vector2((int)i, 0)));
	/*
	auto input = Input::get_singleton();
	if (input->is_key_pressed(0x1C))
		return;
	}
	*/
}
Spelunker::Spelunker() {
	printf("const");
}
Spelunker::~Spelunker() {
	printf("dest");
}
