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
	body = Body();
	body.Init(Vector2(.8, .6), Vector2(0, .2), 0, 0, this, level, Vector2(0, 0),false,false,1,HitboxMask::Enemy,this);
	level->RegisterHurtbox(&body);
}

Body* Snake::GetBody() {
	return &body;
}

bool Snake::TakeDamage(int damageAmount) {
	//no health so just die
	queue_free();
	return true;
}

void Snake::_process(float delta)
{
	sprite->set_flip_h(isFacingRight);
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
		body.vel.x = 120*(isFacingRight?1:-1);
	}
	if (body.process(delta, true, false)) {
		if (body.normal.x != 0) {
			isFacingRight = body.normal.x > 0;
		}
	}
	auto gridCoord = level->WorldToGrid(get_position()+Vector2(50*(isFacingRight?1:-1),0));
	if (body.isGrounded) {
		if (!level->GetBlock((int)gridCoord.x, 1 + (int)gridCoord.y)->present) {
			isFacingRight = !isFacingRight;
		}
	}
	level->RegisterHitbox(body.aabb, 1, HitboxMask::Player,Vector2(0,0),0);
}
