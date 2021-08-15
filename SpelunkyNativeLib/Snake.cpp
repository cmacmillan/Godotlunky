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
	body.Init(Vector2(1, .6), Vector2(0, .2), 0, 0, this, level, Vector2(0, 0));
}

void Snake::_process(float delta)
{
	sprite->set_flip_h(isFacingRight);
	body.vel.x = 120*(isFacingRight?1:-1);
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
}
