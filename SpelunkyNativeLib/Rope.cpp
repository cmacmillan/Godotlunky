#include "Rope.h"
#include <ResourceLoader.hpp>
using namespace godot::Math;

void Rope::_register_methods()
{
	register_method("_ready", &Rope::_ready);
	register_method("_process", &Rope::_process);

	register_property("length", &Rope::length, 0);
	register_property("vel", &Rope::vel, Vector2(0,0));
}

void Rope::_init()
{
}

void Rope::_ready()
{
}

void Rope::_process(float delta)
{
	if (!isInited) {
		isInited = true;
		level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
		startPos = level->WorldToGrid(get_position());
		startPos.x = godot::Math::floor(startPos.x) + .5f;
		set_position(level->GridToWorld(startPos));
	}
	if (!hasUnfurled) {
		SpelAABB aabb = SpelAABB();
		vel.y += level->g * delta;
		aabb.size = Vector2(.5f, .5f);
		aabb.center = level->WorldToGrid(get_position() + vel * delta);
		Vector2 normal;
		Vector2 finalPos = aabb.center;
		bool isGrounded;
		float bounciness = 0.0f;
		if (level->CheckCollisionWithTerrain(aabb, startPos, finalPos, normal, isGrounded))
		{
			if (normal.x != 0 && sign(normal.x) != sign(vel.x)) {
				vel.x = -vel.x * bounciness;
			}
			if (normal.y != 0 && sign(normal.y) != sign(vel.y)) {
				vel.y = -vel.y * bounciness;
			}
		}
		if (vel.y>=-100 && (int)(startPos.y+.5f)!=(int)(finalPos.y+.5f)) {
			hasUnfurled = true;
			baseX = finalPos.x;
			baseY = finalPos.y;
			level->GetBlock(baseX, baseY)->hasRope = true;
			finalPos.y = baseY + .5f;
			subIndex = 2;
		}
		set_position(level->GridToWorld(finalPos));
		startPos = finalPos;
	}
	else
	{
		timeSinceLastSegment += delta;
		while (timeSinceLastSegment > .03f && (currSegmentIndex<length)) {
			timeSinceLastSegment -= .03f;
			SpawnSegment(subIndex/3.0f);
			subIndex++;
			if (subIndex >= 3) {
				subIndex = 0;
				currSegmentIndex++;
				if (currSegmentIndex < length) {
					level->GetBlock(baseX, baseY+currSegmentIndex)->hasRope = true;
				}
			}
		}
	}
}
void Rope::SpawnSegment(float yOffset) {
	Node2D* rope = Object::cast_to<Node2D>(((Ref<PackedScene>)ResourceLoader::get_singleton()->load("res://RopePiece.tscn"))->instance());
	rope->set_position(level->GridToWorld(Vector2(baseX + .5, baseY+.2 +yOffset + currSegmentIndex)));
	level->add_child(rope);
}
