#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"
#include <Particles2D.hpp>
#include <ParticlesMaterial.hpp>

class BloodSpurt : public Node2D
{
	GODOT_CLASS(BloodSpurt, Node2D);

public:
	Body body;
	Level* level;
	Particles2D* particles;
	//ParticlesMaterial* material;
	bool isDying;
	float dieTime;

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
};

