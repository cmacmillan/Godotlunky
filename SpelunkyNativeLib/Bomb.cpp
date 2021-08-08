#include "Bomb.h"
#include "Level.h"
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>
#include <AudioStreamPlayer2D.hpp>
using namespace godot::Math;

void Bomb::_register_methods()
{
	register_method("_ready", &Bomb::_ready);
	register_method("_process", &Bomb::_process);

	register_property("friction", &Bomb::friction, 0.0f);
	register_property("bounciness", &Bomb::bounciness, 0.0f);
	register_property("vel", &Bomb::vel, Vector2(0,0));
}

void Bomb::_init()
{
}

void Bomb::_ready()
{
	AnimatedSprite* animator = get_node<AnimatedSprite>("AnimatedSprite");
	animator->_set_playing(true);
}

void Bomb::_process(float delta)
{
	if (!inited) {
		inited = true;
		level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
		startPos = level->WorldToGrid(get_position());
		auto audio = get_node<AudioStreamPlayer2D>("Audio");
		audio->set_stream(level->bombTimerSFX);
		audio->play();
	}
	lifetime += delta;
	if (!hasExploded) {
		SpelAABB aabb = SpelAABB();
		vel.y += level->g * delta;
		aabb.size = Vector2(.5f, .5f);
		aabb.center = level->WorldToGrid(get_position() + vel * delta);
		Vector2 normal;
		Vector2 finalPos = aabb.center;
		bool isGrounded;
		float bounciness = .3f;
		if (level->CheckCollisionWithTerrain(aabb, startPos, finalPos, normal, isGrounded))
		{
			if (normal.x != 0 && sign(normal.x) != sign(vel.x)) {
				vel.x = -vel.x * bounciness;
			}
			if (normal.y != 0 && sign(normal.y) != sign(vel.y)) {
				vel.y = -vel.y * bounciness;
			}
		}
		set_position(level->GridToWorld(finalPos));
		startPos = finalPos;
		if (isGrounded) {
			vel.x = godot::Math::move_toward(vel.x, 0, delta * friction);
		}
	}
	if (lifetime > 3.15f) {
		queue_free();
	}
	else if (lifetime > 2.5f && !hasExploded) {
		auto audio = get_node<AudioStreamPlayer2D>("Audio");
		audio->set_stream(level->bombExplosionSFX);
		audio->play();
		hasExploded = true;
		Vector2 coord = level->WorldToGrid(get_position());
		for (int i = coord.x - 2; i <= coord.x + 2; i++) {
			for (int j = coord.y - 2; j <= coord.y + 2; j++) {
				auto block = level->GetBlock(i, j);
				//printf("bbbb!");
				//if (!block->indestructible) {
					//printf("set!");
					block->present = false;
				//}
			}
		}
		level->UpdateMeshes();
		auto sprite = this->get_node<AnimatedSprite>("AnimatedSprite");
		sprite->set_visible(false);
		sprite = this->get_node<AnimatedSprite>("Explosion/AnimatedSprite");
		sprite->set_visible(true);
		sprite->_set_playing(true);
	}
}
