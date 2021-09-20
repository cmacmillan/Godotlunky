#pragma once
#include "Rope.h"
#include <ResourceLoader.hpp>
#include <AudioStreamPlayer2D.hpp>
#include <AudioStream.hpp>
using namespace godot::Math;

void Rope::_register_methods()
{
	register_method("_ready", &Rope::_ready);
	register_method("_process", &Rope::_process);

	register_property("length", &Rope::length, 0);
}

void Rope::_init()
{
}

void Rope::_ready()
{
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	Vector2 start = level->WorldToGrid(get_position());
	start.x = godot::Math::floor(start.x) + .5f;
	set_position(level->GridToWorld(start));
	body.Init(Vector2(.5f, .5f), Vector2(0, 0), 0.0f, 0.0f, this, level, startVelocity, false, 1, HitboxMask::Nothing, nullptr, nullptr, true, false, nullptr, nullptr);
	auto audio = get_node<AudioStreamPlayer2D>("Audio");
	audio->set_stream(level->ropeThrowSFX);
	audio->play();
}

void Rope::_process(float delta)
{
	if (!hasUnfurled) {
		Vector2 ogStartPos = body.startPos;
		body.process(delta, true, false);
		if (body.vel.y>=-100 && (int)(ogStartPos.y+.5f)!=(int)(body.endPos.y+.5f)) {
			auto audio = get_node<AudioStreamPlayer2D>("Audio");
			audio->set_stream(level->ropeCatchSFX);
			audio->play();
			hasUnfurled = true;
			baseX = body.endPos.x;
			baseY = body.endPos.y;
			level->GetBlock(baseX, baseY)->hasRope = true;
			body.endPos.y = baseY + .5f;
			subIndex = 2;
		}
		set_position(level->GridToWorld(body.endPos));
		body.startPos= body.endPos;
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
					auto block = level->GetBlock(baseX, baseY + currSegmentIndex);
					if (block->present) {
						currSegmentIndex = length;
					}
					else {
						block->hasRope = true;
					}
				}
			}
		}
	}
}
void Rope::SpawnSegment(float yOffset) {
	Node2D* rope = Object::cast_to<Node2D>(((Ref<PackedScene>)ResourceLoader::get_singleton()->load("res://Scenes/RopePiece.tscn"))->instance());
	rope->set_position(level->GridToWorld(Vector2(baseX + .5, baseY+.2 +yOffset + currSegmentIndex)));
	level->add_child(rope);
}
