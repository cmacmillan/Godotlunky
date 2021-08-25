#include "Snake.h"

void Snake::_register_methods()
{
	register_method("_ready", &Snake::_ready);
	register_method("_process", &Snake::_process);
}

void Snake::_init()
{
	printf("init");
}

void Snake::_ready()
{
	sprite = get_node<AnimatedSprite>("AnimatedSprite");
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	body.Init(Vector2(.8, .6), Vector2(0, .2), 0, 0, this, level, Vector2(0, 0),false,1,HitboxMask::Enemy,this,nullptr,false,false,nullptr);
	level->RegisterHurtbox(&body);
	hitbox.SetValues(body.aabb, 1, HitboxMask::Player, Vector2(0, 0), 0,false);
	hitbox.creatorToEscape = nullptr;
	hitbox.autoUnregister = false;
	level->RegisterHitbox(&hitbox);
}

bool Snake::TakeDamage(int damageAmount,bool stun,vector<HitboxData*>* hitboxesToRemove) {
	//no health so just die
	if (hitboxesToRemove != nullptr) {
		hitboxesToRemove->push_back(&hitbox);
	} else {
		level->UnregisterHitbox(&hitbox);
	}
	body.OnDestroy(hitboxesToRemove);
	queue_free();
	return true;
}

void Snake::_process(float delta)
{
	sprite->set_flip_h(body.isFacingRight);
	auto currGridCoord = level->WorldToGrid(get_position());
	if (!body.isGrounded || (((level->GetBlock(currGridCoord.x - 1, currGridCoord.y)->present && level->GetBlock(currGridCoord.x + 1, currGridCoord.y)->present)) ||
		(!level->GetBlock(currGridCoord.x-1,currGridCoord.y+1)->present &&!level->GetBlock(currGridCoord.x+1,currGridCoord.y+1)->present)||
		(level->GetBlock(currGridCoord.x-1,currGridCoord.y)->present && !level->GetBlock(currGridCoord.x+1,currGridCoord.y+1)->present) ||
		(!level->GetBlock(currGridCoord.x-1,currGridCoord.y+1)->present && level->GetBlock(currGridCoord.x+1,currGridCoord.y)->present)
		)) {
		sprite->set_animation("Idle");
		body.vel.x = 0;
	}
	else {
		sprite->set_animation("Walk");
		body.vel.x = 120*(body.isFacingRight?1:-1);
	}
	if (body.process(delta, true, false)) {
		if (body.normal.x != 0) {
			body.isFacingRight = body.normal.x > 0;
		}
	}
	auto gridCoord = level->WorldToGrid(get_position()+Vector2(50*(body.isFacingRight?1:-1),0));
	if (body.isGrounded) {
		if (!level->GetBlock((int)gridCoord.x, 1 + (int)gridCoord.y)->present) {
			body.isFacingRight = !body.isFacingRight;
		}
	}
	hitbox.aabb = body.aabb;
}
