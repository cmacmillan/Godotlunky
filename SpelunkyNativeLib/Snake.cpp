#include "Snake.h"

void Snake::_register_methods()
{
	register_method("_ready", &Snake::_ready);
	register_method("_process", &Snake::_process);

	//register_property("jumpHeight", &Spelunker::jumpHeight, 0.0f);
}
/*
common data:
Vector2 vel;
Vector2 startPos;
float bounciness
Vector2 size
Vector2 offset


*/

void Snake::_init()
{
	printf("init");
}

void Snake::_ready()
{
}

void Snake::_process(float delta)
{
}
