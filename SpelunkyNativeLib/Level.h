class Globals;
#pragma once
#include "DamageSource.h"
#include "IDamageReciever.h"
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
#include <set>
#include "HitboxData.h"
#include "Spelunker.h"
#include "AutoPickup.h"
#include <Control.hpp>
#include <Label.hpp>
#include <RandomNumberGenerator.hpp>
#include <SceneTree.hpp>
#include <Math.hpp>
#include <Camera2D.hpp>
#include "Globals.h"

enum class DrawType {
	Normal = 0,
	Top = 1,
	Bottom = 2,
	None = 3,
	Spikes = 4,
	BloodySpikes = 5,
	Bi = 6,
};
struct LevelBlock {
	bool present;
	bool indestructible;
	bool hasRope;
	bool hasSpikes;
	bool bloody;
};

struct MovingPlatform {
	Body* root;
	SpelAABB aabb;
};

class Body;
class Spelunker;
class AutoPickup;
class Godolmec;


//#define showDebugHitboxes

class Level : public Node2D
{
	GODOT_CLASS(Level, Node2D);

	Ref<MultiMesh> groundMultimesh;
	Ref<MultiMesh> topMultimesh;
	Ref<MultiMesh> bottomMultimesh;
	Ref<MultiMesh> spikeMultimesh;
	Ref<MultiMesh> bloodySpikesMultimesh;
	Ref<MultiMesh> edgeWallMultimesh;
	Ref<MultiMesh> biMultimesh;

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
	Ref<AudioStream> shotgunSFX;
	Ref<AudioStream> defaultImpactSFX;
	Ref<AudioStream> bulletImpactSFX;
	Ref<AudioStream> metalClankSFX;
	Ref<AudioStream> pickUpSFX;
	Ref<AudioStream> throwSFX;
	Ref<AudioStream> splatSFX;
	Ref<AudioStream> boxOpenSFX;
	Ref<AudioStream> goldPickupSFX;
	Ref<AudioStream> itemPickupSFX;
	Ref<AudioStream> noneLeftSFX;
	Ref<AudioStream> spiderJumpSFX;
	Ref<AudioStream> fadeInSFX;
	Ref<AudioStream> fadeOutSFX;
	Ref<AudioStream> walkThroughDoorSFX;
	Ref<AudioStream> batStartFlapSFX;
	Ref<AudioStream> switchHitSFX;
	Ref<AudioStream> switchShatterSFX;
	Ref<AudioStream> doorOpenSFX;
	Ref<AudioStream> smushSFX;
	Ref<AudioStream> itemSmushSFX;
	Ref<AudioStream> godolmecHitSFX;
	Ref<AudioStream> godolmecTakeDamageSFX;
	Ref<AudioStream> godolmecChargeExplosionSFX;
	Ref<AudioStream> godolmecExplosionSFX;

	Ref<PackedScene> snakeScene;
	Ref<PackedScene> rockScene;
	Ref<PackedScene> shotgunScene;
	Ref<PackedScene> bulletScene;
	Ref<PackedScene> ropeScene;
	Ref<PackedScene> bombScene;
	Ref<PackedScene> batScene;
	Ref<PackedScene> audioSourceScene;
	Ref<PackedScene> bloodSpurtScene;
	Ref<PackedScene> prizeBoxScene;
	Ref<PackedScene> spiderScene;
	Ref<PackedScene> doorScene;
	Ref<PackedScene> doorSwitchScene;
	Ref<PackedScene> godolmecScene;
	Ref<PackedScene> gemShatterScene;
	Ref<PackedScene> shatteredGodolmecScene;

	//auto pickups
	Ref<PackedScene> largeGoldScene;
	Ref<PackedScene> smallGoldScene;
	Ref<PackedScene> ropePileScene;
	Ref<PackedScene> smallBombPileScene;
	Ref<PackedScene> largeBombBoxScene;

#ifdef showDebugHitboxes
	RID GetRid(VisualServer* vs);
	std::vector<RID>* allRids;
	std::vector<RID>* freeRids;
#endif

	Node* frontSpawnRoot;
	Control* uiRoot;
	Label* bombCountLabel;
	Label* ropeCountLabel;
	Label* healthCountLabel;
	Label* moneyCountLabel;
	Label* timerLabel;
	Label* levelIndexLabel;

	bool lookAtGodolmec;
	bool shakeFromGodolmec;
	Camera2D* camera;
	Node2D* cameraTarget;
	float camShakeAmountToDampPerSecond;
	float camShakeForceAmount;
	float camShakesPerSecond;
	float currentShakeForce;
	float shakeForceDampRate;

	float lookAtGodolmecTime;

	Vector2 spawnPos;

	SpelAABB exitPosition;

	bool hasPlayedFadeInSound;
	float fullscreenWipePercent = 0;
	Ref<Material> fullscreenWipeMaterial;
	Godolmec* godolmec;
	Spelunker* spelunker;
	LevelBlock* blocks;
	DrawType* drawTypes;
	int blocksXRes;
	int blocksYRes;
	float worldBlockSize;
	std::set<Body*>* hurtboxes;
	std::set<HitboxData*>* hitboxes;
	std::set<AutoPickup*>* autoPickups;

	std::set<MovingPlatform*>* customCollision;
	//MovingPlatform testCustomCollision;

	bool isDoorOpen;
	bool isFadingOut;
	float fadeOutLerp;

	std::vector<AudioStreamPlayer2D*>* freeAudioSources;
	std::vector<AudioStreamPlayer2D*>* outstandingAudioSources;

	Node2D* exitDoor;

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);

	RandomNumberGenerator* rng;//free me?

	void PlayAudio(Ref<AudioStream> sound, Vector2 gridCoordPos);
	LevelBlock* GetBlock(int x, int y);
	Vector2 WorldToGrid(Vector2 v);
	Vector2 GridToWorld(Vector2 v);
	Vector2 WorldToGridSize(Vector2 v);
	Vector2 GridToWorldSize(Vector2 v);
	void DoorSwitchHit(bool isGodolmec);
	void SpawnBlood(Vector2 gridCoord,int count=10);
	void RegisterHurtbox(Body* hurtbox);
	void UnregisterHurtbox(Body* hurtbox);
	void RegisterHitbox(HitboxData* hitbox);
	void UnregisterHitbox(HitboxData* hitbox);
	bool CheckCollisionWithTerrain(SpelAABB aabb,Vector2 previousPos,Vector2& endPos,Vector2& normal,bool& isGrounded,bool& isSmushed,Body* body);
	float MarchVertical(float startY, float endY, float x1, float x2,bool& hit);
	float MarchHorizontal(float startX, float endX, float y1, float y2,bool& hit);
	bool IsOverlappingTerrain(Vector2 pos);
	Vector2 CopyLayoutIntoBlocks(string layout, int x, int y, bool flipX);
	float Random();
	void UpdateMeshes();
	void WriteHighScoreToGlobals();
	Level();
	~Level();
};

