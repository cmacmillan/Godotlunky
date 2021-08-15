#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include <Texture.hpp>
#include <VisualServer.hpp>
#include <PackedScene.hpp>
#include <QuadMesh.hpp>
#include <MultiMesh.hpp>
#include <MultiMeshInstance2D.hpp>
#include <AudioStreamPlayer2D.hpp>
#include <vector>
enum DrawType {
	Normal = 0,
	Top = 1,
	Bottom = 2,
	None = 3,
	Spikes = 4,
	BloodySpikes = 5,
};
enum HitboxMask : unsigned int {
	Nothing = 0,
	Player = 1,
	Enemy = 2,
	All = 4294967295,
};
struct LevelBlock {
	bool present;
	bool indestructible;
	bool hasRope;
	bool hasSpikes;
	bool bloody;
};

struct SpelAABB
{
	Vector2 center;
	Vector2 size;
	bool overlaps(SpelAABB& other) {
		Vector2 half = size/2;
		Vector2 otherHalf= other.size/2;
		return overlaps1D(other.center.x - otherHalf.x, other.center.x + otherHalf.x, center.x - half.x, center.x + half.x) && \
			overlaps1D(other.center.y - otherHalf.y, other.center.y + otherHalf.y, center.y - half.y, center.y + half.y);
	}
	//beautiful https://stackoverflow.com/questions/20925818/algorithm-to-check-if-two-boxes-overlap
	bool overlaps1D(float min1, float max1, float min2, float max2) {
		return max1 >= min2 && max2 >= min1;
	}
};

class IDamageReciever {
public:
	virtual void TakeDamage(int damageAmount) = 0;
};

struct HurtboxData {
	SpelAABB aabb;
	IDamageReciever* reciever;
	HitboxMask mask;
};
struct HitboxData {
	SpelAABB aabb;
	int damageAmount;
	HitboxMask mask;
};

class Level : public Node2D
{
	GODOT_CLASS(Level, Node2D);

	Ref<MultiMesh> groundMultimesh;
	Ref<MultiMesh> topMultimesh;
	Ref<MultiMesh> bottomMultimesh;
	Ref<MultiMesh> spikeMultimesh;
	Ref<MultiMesh> bloodySpikesMultimesh;

public:
	float g;
	Ref<AudioStream> ropeThrowSFX;
	Ref<AudioStream> ropeCatchSFX;
	Ref<AudioStream> bombExplosionSFX;
	Ref<AudioStream> bombTimerSFX;
	Ref<AudioStream> jumpSFX;
	Ref<AudioStream> landSFX;
	Ref<AudioStream> whipSFX;
	Ref<AudioStream> hitSFX;
	Ref<AudioStream> skewerSFX;

	Ref<PackedScene> snakeScene;

	LevelBlock* blocks;
	DrawType* drawTypes;
	int blocksXRes;
	int blocksYRes;
	float worldBlockSize;
	std::vector<HitboxData>* hitboxes;
	std::vector<HurtboxData>* hurtboxes;

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);
	LevelBlock* GetBlock(int x, int y);
	Vector2 WorldToGrid(Vector2 v);
	Vector2 GridToWorld(Vector2 v);
	void RegisterHurtbox(SpelAABB box,IDamageReciever* receiver,HitboxMask mask);
	void RegisterHitbox(SpelAABB box,int damageAmount,HitboxMask mask);
	bool CheckCollisionWithTerrain(SpelAABB aabb,Vector2 previousPos,Vector2& endPos,Vector2& normal,bool& isGrounded);
	float MarchVertical(float startY, float endY, float x1, float x2,bool& hit);
	float MarchHorizontal(float startX, float endX, float y1, float y2,bool& hit);
	bool IsOverlappingTerrain(Vector2 pos);
	void CopyLayoutIntoBlocks(string layout, int x, int y);
	void UpdateMeshes();
	Level();
	~Level();
};

