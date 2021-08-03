#include "Spelunker.h"
#include <Input.hpp>
#include "Level.h"

void Spelunker::_register_methods()
{
	register_method("_ready", &Spelunker::_ready);
	register_method("_process", &Spelunker::_process);

	register_property("jumpHeight", &Spelunker::jumpHeight, 0.0f);
	//register_property("level", &Spelunker::level, Ref<Level>());
}

void Spelunker::_init()
{
	printf("init");
}

Vector2 vel;
Vector2 startPos;
void Spelunker::_ready()
{
	//vel = Vector2(0, -10);
	auto level = Level::singleton;
	startPos = level->WorldToGrid(get_position());
	vel = Vector2(.3, -1);
	printf("Ready");
}

void Spelunker::_process(float delta)
{
	vel.y += 1.0f*delta;
	set_position(get_position()+vel);
	auto level = Level::singleton;
	SpelAABB aabb = SpelAABB();
	aabb.center = level->WorldToGrid(get_position());
	//aabb.size = Vector2(.25f, .25f);
	aabb.size = Vector2(.9f, .9f);
	Vector2 normal;
	Vector2 finalPos=aabb.center;
	printf("(%f,%f)", aabb.center.x, aabb.center.y);
	level->CheckCollisionWithTerrain(aabb,startPos,finalPos,normal);
	//printf("(%f,%f)", finalPosRef.x, finalPosRef.y);
	set_position(level->GridToWorld(finalPos));
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
