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
}

void Bomb::_init(){}

void Bomb::_ready()
{
	AnimatedSprite* animator = get_node<AnimatedSprite>("AnimatedSprite");
	animator->_set_playing(true);
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	body = Body();
	body.Init(Vector2(.5f, .5f), Vector2(0, 0), .3, 3000, this, level, startVelocity,true,1,HitboxMask::Everything,nullptr,nullptr,true);
	level->RegisterHurtbox(&body);
	auto audio = get_node<AudioStreamPlayer2D>("Audio");
	audio->set_stream(level->bombTimerSFX);
	audio->play();
}

void Bomb::_process(float delta)
{
	lifetime += delta;
	if (!hasExploded) {
		body.process(delta, true, true);
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
		SpelAABB damageBox = SpelAABB();
		body.OnDestroy();
		damageBox.center = coord;
		damageBox.size = Vector2(4,4);
		level->UnregisterHurtbox(&body);
		explosionHitbox.SetValues(damageBox, 10, HitboxMask::Everything, Vector2(0, -800), 2000);
		explosionHitbox.creatorToEscape = nullptr;
		explosionHitbox.autoUnregister = true;
		level->RegisterHitbox(&explosionHitbox);
		for (int i = coord.x - 2; i <= coord.x + 2; i++) {
			for (int j = coord.y - 2; j <= coord.y + 2; j++) {
				auto block = level->GetBlock(i, j);
				if (!block->indestructible) {
					block->present = false;
					block->hasSpikes = false;
					block->bloody = false;
				}
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
