#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include <Texture.hpp>
#include <VisualServer.hpp>
#include <PackedScene.hpp>
#include <QuadMesh.hpp>
#include <MultiMesh.hpp>
#include <MultiMeshInstance2D.hpp>
enum DrawType {
	Normal = 0,
	Top = 1,
	Bottom = 2,
	None = 3,
};
struct LevelBlock {
	bool present;
	bool indestructible=false;
	bool hasRope = false;
};

struct SpelAABB
{
	Vector2 center;
	Vector2 size;
};

class Level : public Node2D
{
	GODOT_CLASS(Level, Node2D);

	Ref<MultiMesh> groundMultimesh;
	Ref<MultiMesh> topMultimesh;
	Ref<MultiMesh> bottomMultimesh;

	MultiMeshInstance2D* groundMultimeshInstance;
	MultiMeshInstance2D* topMultimeshInstance;
	MultiMeshInstance2D* bottomMultimeshInstance;

public:
	float g;
	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
	LevelBlock* GetBlock(int x, int y);
	Vector2 WorldToGrid(Vector2 v);
	Vector2 GridToWorld(Vector2 v);
	bool CheckCollisionWithTerrain(SpelAABB aabb,Vector2 previousPos,Vector2& endPos,Vector2& normal,bool& isGrounded);
	float MarchVertical(float startY, float endY, float x1, float x2,bool& hit);
	float MarchHorizontal(float startX, float endX, float y1, float y2,bool& hit);
	bool IsOverlappingTerrain(Vector2 pos);
	void CopyLayoutIntoBlocks(string layout, int x, int y);
	void UpdateMeshes();
	Level();
	~Level();
};

