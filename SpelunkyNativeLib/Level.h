#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include <Texture.hpp>
#include <VisualServer.hpp>
#include <PackedScene.hpp>
struct LevelBlock {
	bool present;
};

class Level : public Node2D
{
	GODOT_CLASS(Level, Node2D);

	Ref<Texture> groundTexture;
	float blockSize=0;

public:
	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
	void Draw();
	RID GetRid(VisualServer * server);
	LevelBlock* GetBlock(int x, int y);
	Level();
	~Level();
};

