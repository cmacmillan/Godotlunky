#include "Common.h"
#include "Spelunker.h"
#include "Level.h"
#include "Bomb.h"
#include "Rope.h"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
	godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
	godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
	godot::Godot::nativescript_init(handle);

	godot::register_class<Level>();
	godot::register_class<Spelunker>();
	godot::register_class<Bomb>();
	godot::register_class<Rope>();
}