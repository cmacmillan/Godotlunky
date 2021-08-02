#include "Spelunker.h"
#include <Input.hpp>

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

void Spelunker::_ready()
{
	printf("Ready");
}

void Spelunker::_process(float delta)
{
	//printf("asdf");
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
