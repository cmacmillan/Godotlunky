#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include <Texture.hpp>
#include <VisualServer.hpp>
#include <PackedScene.hpp>
#include <QuadMesh.hpp>
#include <MultiMesh.hpp>
struct LevelBlock {
	bool present;
};

class Level : public Node2D
{
	GODOT_CLASS(Level, Node2D);

	Ref<MultiMesh> groundMultimesh;
	Ref<MultiMesh> topMultimesh;
	Ref<MultiMesh> bottomMultimesh;
	float blockSize=0;

public:
	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
	LevelBlock* GetBlock(int x, int y);
	void CopyLayoutIntoBlocks(string layout, int x, int y);
	void UpdateMeshes();
	Level();
	~Level();
};

