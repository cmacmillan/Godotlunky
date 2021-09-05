#pragma once
#include "Level.h"
#include <Node2D.hpp>
#include <stdlib.h>
#include <vector>
#include <MultiMeshInstance2D.hpp>
#include <MultiMesh.hpp>
#include <QuadMesh.hpp>
#include <string>
#include <sstream>
#include <ResourceLoader.hpp>
#include <AudioStream.hpp>
#include "Snake.h"
#include "Rock.h"
#include "Shotgun.h"
#include "ObjectMaker.h"

void HitboxData::InitOrClearBodiesAlreadyDamagedList() {
	if (bodiesAlreadyDamaged == nullptr) {
		bodiesAlreadyDamaged = new std::vector<Body*>();
	}
	else {
		bodiesAlreadyDamaged->clear();
	}
}
void HitboxData::SetValues(SpelAABB box, int damageAmount, HitboxMask mask, Vector2 knockInDirectionAmount, float knockAwayAmount, bool stun,Body* self)
{
	this->aabb = box;
	this->damageAmount = damageAmount;
	this->mask = mask;
	this->knockInDirectionAmount = knockInDirectionAmount;
	this->knockAwayAmount = knockAwayAmount;
	this->autoUnregister = false;
	this->stun = stun;
	this->assignCreatorToEscapeToMoveFastHitbox = nullptr;
	this->self = self;
}

void Level::_register_methods()
{
	register_method("_ready", &Level::_ready);
	register_method("_process", &Level::_process);

	register_property("groundMultimesh", &Level::groundMultimesh, Ref<MultiMesh>());
	register_property("topMultimesh", &Level::topMultimesh, Ref<MultiMesh>());
	register_property("bottomMultimesh", &Level::bottomMultimesh, Ref<MultiMesh>());
	register_property("spikeMultimesh", &Level::spikeMultimesh, Ref<MultiMesh>());
	register_property("bloodySpikeMultimesh", &Level::bloodySpikesMultimesh, Ref<MultiMesh>());
	register_property("edgeWallMultimesh", &Level::edgeWallMultimesh, Ref<MultiMesh>());

	register_property("g", &Level::g, 0.0f);

	register_property("bombExplosionSFX", &Level::bombExplosionSFX, Ref<AudioStream>());
	register_property("bombTimerSFX", &Level::bombTimerSFX, Ref<AudioStream>());
	register_property("jumpSFX", &Level::jumpSFX, Ref<AudioStream>());
	register_property("ropeCatchSFX", &Level::ropeCatchSFX, Ref<AudioStream>());
	register_property("ropeThrowSFX", &Level::ropeThrowSFX, Ref<AudioStream>());
	register_property("landSFX", &Level::landSFX, Ref<AudioStream>());
	register_property("whipSFX", &Level::whipSFX, Ref<AudioStream>());
	register_property("hitSFX", &Level::hitSFX, Ref<AudioStream>());
	register_property("skewerSFX", &Level::skewerSFX, Ref<AudioStream>());
	register_property("shotgunSFX", &Level::shotgunSFX, Ref<AudioStream>());
	register_property("defaultImpactSFX", &Level::defaultImpactSFX, Ref<AudioStream>());
	register_property("bulletImpactSFX", &Level::bulletImpactSFX, Ref<AudioStream>());
	register_property("metalClankSFX", &Level::metalClankSFX, Ref<AudioStream>());
	register_property("pickUpSFX", &Level::pickUpSFX, Ref<AudioStream>());
	register_property("throwSFX", &Level::throwSFX, Ref<AudioStream>());
	register_property("splatSFX", &Level::splatSFX, Ref<AudioStream>());
	register_property("boxOpenSFX", &Level::boxOpenSFX, Ref<AudioStream>());
	register_property("goldPickupSFX", &Level::goldPickupSFX, Ref<AudioStream>());
	register_property("itemPickupSFX", &Level::itemPickupSFX, Ref<AudioStream>());
	register_property("noneLeftSFX", &Level::noneLeftSFX, Ref<AudioStream>());

	register_property("audioSourceScene", &Level::audioSourceScene, Ref<PackedScene>());

	register_property("snakeScene", &Level::snakeScene, Ref<PackedScene>());
	register_property("rockScene", &Level::rockScene, Ref<PackedScene>());
	register_property("shotgunScene", &Level::shotgunScene, Ref<PackedScene>());
	register_property("bulletScene", &Level::bulletScene, Ref<PackedScene>());
	register_property("ropeScene", &Level::ropeScene, Ref<PackedScene>());
	register_property("bombScene", &Level::bombScene, Ref<PackedScene>());
	register_property("batScene", &Level::batScene, Ref<PackedScene>());
	register_property("bloodSpurtScene", &Level::bloodSpurtScene, Ref<PackedScene>());
	register_property("prizeBoxScene", &Level::prizeBoxScene, Ref<PackedScene>());

	//auto pickups
	register_property("largeGoldScene", &Level::largeGoldScene, Ref<PackedScene>());
	register_property("ropePileScene", &Level::ropePileScene, Ref<PackedScene>());
	register_property("smallBombPileScene", &Level::smallBombPileScene, Ref<PackedScene>());
	register_property("largeBombBoxScene", &Level::largeBombBoxScene, Ref<PackedScene>());
}

const string layout1 = 
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 X0000000000000000000B0000000000000000000X\n\
 X00XXX00X0000000000000000000000000000000X\n\
 X00XXX000000000000G000000000000000000000X\n\
 X00XXX0000000000000000000000000P$kKr000SX\n\
 X00XXX0000000000000000000000000000000000X\n\
 X00000S0X0000000000000000000000000000000X\n\
 XX0000X0X0000000000000000000000000000000X\n\
 X0X00S00000000R0WW0000000000000000000000X\n\
 XXXXX0XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

//this should probably not be global
////////////////////////////////

void Level::CopyLayoutIntoBlocks(string layout, int x, int y,bool flipX)
{
	std::istringstream iss(layout);
	int yCurr = 0;
	for (std::string line; std::getline(iss, line); )
	{
		int xCurr = 0;
		int len = line.length();
		for (int i = 0; i < len; i++) {
			LevelBlock* block;
			bool valid = true;
			bool setPresent = false;
			Vector2 gridCoord;
			if (flipX) {
				gridCoord = Vector2(((metaBlockWidth-1)-xCurr)+x+.5f, yCurr+ y + .5f);
				block = GetBlock(((metaBlockWidth-1)-xCurr)+x+.5f, yCurr + y);
	 		}
			else 
			{
				gridCoord = Vector2(xCurr+x+.5f, yCurr+ y + .5f);
				block = GetBlock(xCurr + x, yCurr + y);
			}
			if (line[i]=='k') {
				SpawnSmallBombPile(this, gridCoord);
			} else if(line[i] == 'K') {
				SpawnLargeBombBox(this, gridCoord);
			} else if(line[i] == '$') {
				SpawnLargeGoldPile(this, gridCoord);
			} else if(line[i] == 'r') {
				SpawnSmallRopePile(this, gridCoord);
			} else if (line[i] == 'P') {
				SpawnPrizeBox(this,gridCoord);
			} else if (line[i] == 'B') {
				SpawnBat(this,gridCoord);
			} else if (line[i] == 'G') {
				SpawnShotgun(this,gridCoord);
			} else if (line[i] == 'R') {
				SpawnRock(this,gridCoord);
			} else if (line[i] == 'S') {
				SpawnSnake(this,gridCoord);
			} else if (line[i] == 'W') {
				block->hasSpikes=true;
			} else if (line[i]=='0') {
			} else if (line[i]=='X') {
				setPresent = true;
			}
			else {
				valid = false;
			}
			if (valid) {
				block->present = setPresent;
				xCurr++;
			}
		}
		yCurr++;
	}
}

void Level::RegisterHurtbox(Body* hurtbox) {
	hurtboxes->insert(hurtbox);
}
void Level::UnregisterHurtbox(Body* hurtbox) {
	hurtboxes->erase(hurtbox);
}

void Level::SpawnBlood(Vector2 gridCoord,int count) {
	PlayAudio(splatSFX, gridCoord);
	for (int i = 0; i < count; i++) {
		SpawnBloodSpurt(this,gridCoord);
	}
}

//gotta remember to unregister before freeing
void Level::RegisterHitbox(HitboxData* hitbox) 
{
	hitboxes->insert(hitbox);
}
void Level::UnregisterHitbox(HitboxData* hitbox) 
{
	hitboxes->erase(hitbox);
}

void Level::_init()
{
}

Vector2 Level::WorldToGrid(Vector2 v) 
{
	return (v - this->get_position())/(worldBlockSize);
}

Vector2 Level::WorldToGridSize(Vector2 v) 
{
	return v / worldBlockSize;
}
Vector2 Level::GridToWorldSize(Vector2 v) {
	return (v * worldBlockSize);
}
Vector2 Level::GridToWorld(Vector2 v) 
{
	return (v*worldBlockSize)+this->get_position();
}

bool Level::IsOverlappingTerrain(Vector2 pos) {
	return GetBlock(pos.x, pos.y)->present;
}

float Level::MarchVertical(float startY, float endY, float x1, float x2,bool& hit) {
	int dir = endY > startY ? 1 : -1;
	for (int i = startY; i != (int)endY+dir; i+=dir) {
		if (GetBlock(x1, i)->present || GetBlock(x2, i)->present) {
			hit = true;
			if (dir > 0)
			{
				return i - .0001f;
			}
			else
			{
				return i+1.0 + .0001f;
			}
		}
	}
	return endY;
}

float Level::MarchHorizontal(float startX, float endX, float y1, float y2, bool& hit) {
	float dir = endX > startX ? 1.0 : -1.0;
	for (int i = startX; i != (int)endX+dir;i+=dir) {
		if (GetBlock(i, y1)->present || GetBlock(i, y2)->present) {
			hit = true;
			if (dir > 0)
			{
				return i - .0001f;
			}
			else
			{
				return i + 1.0 + .0001f;
			}
		}
	}
	return endX;
}

float Level::Random() {
	float retr = rng->randf();
	if (retr == 1.0f) 
	{
		return .9999f;
	}
	return retr;
}

bool Level::CheckCollisionWithTerrain(SpelAABB aabb, Vector2 previousPos, Vector2& endPos, Vector2& normal,bool& isGrounded)
{
	Vector2 size = aabb.size;
	Vector2 half = aabb.size/2;
	Vector2 center = aabb.center;
	Vector2 upperLeft = center - half;
	Vector2 upperRight = center + Vector2(half.x, -half.y);
	Vector2 lowerLeft = center + Vector2(-half.x, half.y);
	Vector2 lowerRight = center + half;

	//this only really needs to be in the correct cell, not even in exactly the previous pos
	Vector2 prevUpperLeft = previousPos- half;
	Vector2 prevUpperRight = previousPos + Vector2(half.x, -half.y);
	Vector2 prevLowerLeft = previousPos+ Vector2(-half.x, half.y);
	Vector2 prevLowerRight = previousPos+ half;

	isGrounded = false;
	normal = Vector2();
	bool horizHit= false;
	if (center.x > previousPos.x) {
		endPos.x = MarchHorizontal(previousPos.x+half.x,upperRight.x,prevUpperRight.y,prevLowerRight.y,horizHit)-half.x;
		if (horizHit) {
			normal += Vector2(-1,0);
		}
	}
	else if (center.x < previousPos.x) {
		endPos.x = MarchHorizontal(previousPos.x - half.x, upperLeft.x, prevUpperLeft.y, prevLowerLeft.y,horizHit)+half.x;
		if (horizHit) {
			normal += Vector2(1,0);
		}
	}

	bool vertHit= false;
	if (center.y > previousPos.y) {
		endPos.y = MarchVertical(previousPos.y + half.y, lowerLeft.y, prevLowerLeft.x, prevLowerRight.x,vertHit)-half.y;
		if (vertHit) {
			normal += Vector2(0,-1);
			isGrounded = true;
		}
	}
	else if (center.y < previousPos.y)
	{
		endPos.y = MarchVertical(previousPos.y - half.y, upperLeft.y, prevUpperLeft.x, prevUpperRight.x,vertHit)+half.y;
		if (vertHit) {
			normal += Vector2(0,1);
		}
	}
	bool retr;
	retr = vertHit || horizHit;
	//if (retr)
		//normal.normalize();
	return retr;
}

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

void Level::_ready()
{
	rng = RandomNumberGenerator::_new();
	rng->randomize();
	blocksXRes = metaBlockWidth*numMetaBlocksWidth+2;
	blocksYRes = metaBlockHeight*numMetaBlocksHeight+2;
	worldBlockSize = 100;
#ifdef showDebugHitboxes
	allRids = new	std::vector<RID>();
	freeRids = new std::vector<RID>();
#endif // showDebugHitboxes

	frontSpawnRoot = get_node("/root/GameScene/SpawnRoot");
	uiRoot = get_node<Control>("/root/GameScene/Spelunker/Camera2D/CanvasLayer/Control");
	bombCountLabel = uiRoot->get_node<Label>("TopLeftContainer/BombCountContainer/Label");
	ropeCountLabel= uiRoot->get_node<Label>("TopLeftContainer/RopeCountContainer/Label");
	healthCountLabel= uiRoot->get_node<Label>("TopLeftContainer/HealthCountContainer/Label");
	moneyCountLabel = uiRoot->get_node<Label>("TopLeftContainer/MoneyCountContainer/Label");

	freeAudioSources = new std::vector<AudioStreamPlayer2D*>();
	outstandingAudioSources = new std::vector<AudioStreamPlayer2D*>();
	hitboxes = new std::set<HitboxData*>();
	hurtboxes = new std::set<Body*>();
	autoPickups = new std::set<AutoPickup*>();
	blocks = (LevelBlock*)malloc(sizeof(LevelBlock) * blocksXRes*blocksYRes);
	drawTypes = (DrawType*)malloc(sizeof(DrawType) * blocksXRes*blocksYRes);
	for (int i = 0; i < blocksXRes; i++) {
		for (int j = 0; j < blocksYRes; j++) {
			GetBlock(i, j)->hasRope = false;
			GetBlock(i, j)->present = true;
			GetBlock(i, j)->indestructible= i==0||j==0||i==blocksXRes||j==blocksYRes;
			GetBlock(i, j)->hasSpikes= false;
			GetBlock(i, j)->bloody= false;
		}
	}
	bool metaBlockMask[numMetaBlocks];
	for (int i = 0; i < numMetaBlocks; i++) {
		metaBlockMask[i] = false;
	}
	printf("start generating");
	int startIndex = 0;//(Random() * numMetaBlocksWidth);
	for (int j = 0; j < 4; j++) {
	//for (int j = 0; j < metaBlockHeight; j++) {
		int endIndex = startIndex;
		while (endIndex == startIndex) {
			endIndex = (Random() * numMetaBlocksWidth);
		}
		int direction = sign(endIndex-startIndex);
		for (int i = startIndex; i != endIndex+direction; i+=direction) {
			string metaBlock;
			bool flip = false;
			if (i == startIndex && j==0) //starting platform
			{
				metaBlock = startingPlatforms[(int)(startingPlatformsLength * Random())];
				flip = Random() > .5f;//randomly flip
			}
			else if (i == startIndex) //recieve drop
			{
				metaBlock = dropRecievers[(int)(dropRecieverLength * Random())];
				flip = startIndex>endIndex;
			}
			else if (i == endIndex) //do drop
			{
				int count = hallwayDropLength + rightRecieverDropLength;
				int rand = count * Random();
				if (rand < hallwayDropLength) 
				{
					metaBlock = hallwayDrop[rand];
					flip = Random() > .5f;//randomly flip
				}
				else 
				{
					metaBlock = rightRecieverDrop[rand-hallwayDropLength];
					flip = endIndex > startIndex;
				}
			}
			else //hallway
			{
				int count = hallwayLength + hallwayDropLength;
				int rand = count * Random();
				flip = Random() > .5f;//randomly flip
				if (rand < hallwayLength) 
				{
					metaBlock = hallway[rand];
				}
				else 
				{
					metaBlock = hallwayDrop[rand-hallwayLength];
				}
			}
			CopyLayoutIntoBlocks(metaBlock, i * metaBlockWidth+1, j * metaBlockHeight+1, flip);
		}
		startIndex = endIndex;
	}
	printf("done generating");
	//////////////
	int edgeThickness = 15;
	int expectedCount = 2 * edgeThickness * (blocksXRes-1) + 2 * edgeThickness * (blocksYRes-1) + edgeThickness * edgeThickness * 4;
	edgeWallMultimesh->set_instance_count(expectedCount);
	int edgeWallIndex = 0;
	//edgeWallMultimesh->set_instance_transform_2d(edgeWallIndex++, Transform2D().translated(Vector2(i * worldBlockSize + worldBlockSize / 2.0f, (j + 1) * worldBlockSize - worldBlockSize / 2.0f)));
	for (int j = -edgeThickness+1; j < 1; j++) {
		for (int i = -edgeThickness+1; i < blocksXRes + edgeThickness; i++) {
			edgeWallMultimesh->set_instance_transform_2d(edgeWallIndex++, Transform2D().translated(Vector2(i * worldBlockSize + worldBlockSize / 2.0f, (j + 1) * worldBlockSize - worldBlockSize / 2.0f)));
		}
	}
	for (int j = blocksYRes; j < blocksYRes+edgeThickness; j++) {
		for (int i = -edgeThickness+1; i < blocksXRes + edgeThickness; i++) {
			edgeWallMultimesh->set_instance_transform_2d(edgeWallIndex++, Transform2D().translated(Vector2(i * worldBlockSize + worldBlockSize / 2.0f, (j + 1) * worldBlockSize - worldBlockSize / 2.0f)));
		}
	}
	for (int j = 1; j<blocksYRes; j++) {
		for (int i = -edgeThickness+1; i < 1; i++) {
			edgeWallMultimesh->set_instance_transform_2d(edgeWallIndex++, Transform2D().translated(Vector2(i * worldBlockSize + worldBlockSize / 2.0f, (j + 1) * worldBlockSize - worldBlockSize / 2.0f)));
		}
	}
	for (int j = 1; j<blocksYRes; j++) {
		for (int i = blocksXRes; i < blocksXRes+edgeThickness; i++) {
			edgeWallMultimesh->set_instance_transform_2d(edgeWallIndex++, Transform2D().translated(Vector2(i * worldBlockSize + worldBlockSize / 2.0f, (j + 1) * worldBlockSize - worldBlockSize / 2.0f)));
		}
	}
	printf("expected %d, actual %d ", expectedCount, edgeWallIndex);
	//CopyLayoutIntoBlocks(layout1, 0, 0);
	UpdateMeshes();
}
void Level::UpdateMeshes() {
	int topCount = 0;
	int bottomCount = 0;
	int normalCount = 0;
	int spikeCount = 0;
	int bloodySpikeCount= 0;
	for (int i = 0; i < blocksXRes; i++) {
		for (int j = 0; j < blocksYRes; j++) {
			if (GetBlock(i, j)->bloody) {
				drawTypes[j * blocksXRes + i] = DrawType::BloodySpikes;
				bloodySpikeCount++;
			}
			else if (GetBlock(i, j)->hasSpikes) {
				drawTypes[j * blocksXRes + i] = DrawType::Spikes;
				spikeCount++;
			}
			else if (!GetBlock(i, j)->present || GetBlock(i, j)->indestructible) {
				drawTypes[j * blocksXRes + i] = DrawType::None;
			}
			else if (j > 0 && !GetBlock(i, j - 1)->present) {
				drawTypes[j * blocksXRes + i] = DrawType::Top;
				topCount++;
			} 
			else if (j < blocksYRes-1 && !GetBlock(i, j + 1)->present)
			{
				drawTypes[j * blocksXRes + i] = DrawType::Bottom;
				bottomCount++;
			}
			else
			{
				drawTypes[j * blocksXRes + i] = DrawType::Normal;
				normalCount++;
			}
		}
	}
	groundMultimesh->set_instance_count(normalCount);
	topMultimesh->set_instance_count(topCount);
	bottomMultimesh->set_instance_count(bottomCount);
	spikeMultimesh->set_instance_count(spikeCount);
	bloodySpikesMultimesh->set_instance_count(bloodySpikeCount);
	int normalIndex = 0;
	int topIndex = 0;
	int bottomIndex= 0;
	int spikeIndex= 0;
	int bloodySpikeIndex= 0;
	for (int i = 0; i < blocksXRes; i++) {
		for (int j = 0; j < blocksYRes; j++) {
			switch (drawTypes[j*blocksXRes+i])
			{
			case DrawType::BloodySpikes:
				bloodySpikesMultimesh->set_instance_transform_2d(bloodySpikeIndex++, Transform2D().translated(Vector2(i * worldBlockSize + worldBlockSize / 2.0f, (j + 1) * worldBlockSize - worldBlockSize / 2.0f)));
				break;
			case DrawType::Spikes:
				spikeMultimesh->set_instance_transform_2d(spikeIndex++, Transform2D().translated(Vector2(i * worldBlockSize + worldBlockSize / 2.0f, (j + 1) * worldBlockSize - worldBlockSize / 2.0f)));
				break;
			case DrawType::Bottom:
				bottomMultimesh->set_instance_transform_2d(bottomIndex++, Transform2D().translated(Vector2(i * worldBlockSize + worldBlockSize / 2.0f, (j + 1) * worldBlockSize - worldBlockSize / 2.0f)));
				break;
			case DrawType::Top:
				topMultimesh->set_instance_transform_2d(topIndex++, Transform2D().translated(Vector2(i * worldBlockSize + worldBlockSize / 2.0f, (j + 1) * worldBlockSize - worldBlockSize / 2.0f)));
				break;
			case DrawType::Normal:
				groundMultimesh->set_instance_transform_2d(normalIndex++, Transform2D().translated(Vector2(i * worldBlockSize + worldBlockSize / 2.0f, (j + 1) * worldBlockSize - worldBlockSize / 2.0f)));
				break;
			default:
				break;
			}
		}
	}
}


#ifdef showDebugHitboxes
RID Level::GetRid(VisualServer* vs) {
	if (freeRids->size() == 0) {
		auto newRid = vs->canvas_item_create();
		allRids->push_back(newRid);
		return newRid;
	}
	else 
	{
		auto rid = freeRids->back();
		freeRids->pop_back();
		return rid;
	}
}
#endif

std::vector<Body*>* hurtboxesToRemove = nullptr;
std::vector<HitboxData*>* hitboxesToRemove= nullptr;
std::vector<AutoPickup*>* autopickupsToRemove= nullptr;
void Level::_process(float delta)
{
	for (int i = 0; i < outstandingAudioSources->size(); i++) 
	{
		AudioStreamPlayer2D* curr = (*outstandingAudioSources)[i];
		if (curr->get_playback_position()>=curr->get_stream()->get_length()) {
			outstandingAudioSources->erase(outstandingAudioSources->begin() + i);//kill me
			freeAudioSources->push_back(curr);
			i--;
		}
	}
	if (hurtboxesToRemove == nullptr) {
		hurtboxesToRemove = new std::vector<Body*>();
	}
	if (hitboxesToRemove == nullptr) {
		hitboxesToRemove = new std::vector<HitboxData*>();
	}
	if (autopickupsToRemove == nullptr) {
		autopickupsToRemove= new std::vector<AutoPickup*>();
	}
#ifdef showDebugHitboxes
	auto vs= VisualServer::get_singleton();
	for (auto i : *allRids) {
		vs->canvas_item_clear(i);
	}
	for (auto hitbox : *hitboxes) {
		auto rid = GetRid(vs);
		Rect2 r;
		vs->canvas_item_set_parent(rid, get_canvas_item());
		auto size = GridToWorldSize(hitbox->aabb.size);
		r.set_position(GridToWorld(hitbox->aabb.center)-size/2);
		r.set_size(size);
		vs->canvas_item_add_rect(rid,r,Color(1,0,0.4));
	}
	for (auto hurtbox: *hurtboxes) {
		auto rid = GetRid(vs);
		Rect2 r;
		vs->canvas_item_set_parent(rid, get_canvas_item());
		auto size = GridToWorldSize(hurtbox->aabb.size);
		r.set_position(GridToWorld(hurtbox->aabb.center)-size/2);
		r.set_size(size);
		vs->canvas_item_add_rect(rid,r,Color(0,0,1,.4));
	}
	freeRids->clear();
	for (auto i : *allRids) {
		freeRids->push_back(i);
	}
#endif // showDebugHitboxes
	//process foot hitbox
	if (!spelunker->body.isGrounded && spelunker->body.vel.y>0) {
		auto hitbox = spelunker->footHitbox;
		for (auto body : *hurtboxes) {
			if (((hitbox.mask & body->takeDamageMask) != 0) && hitbox.aabb.overlaps(body->aabb)) {
				if (body->damageReciever != nullptr) {
					if (body->damageReciever->TakeDamage(hitbox.damageAmount, hitbox.stun, hitboxesToRemove)) {
						hurtboxesToRemove->push_back(body);
					}
					spelunker->body.vel.y = -1500;
				}
			}
		}
	}
	for (auto remove : *hurtboxesToRemove) {
		UnregisterHurtbox(remove);
	}
	for (auto remove : *hitboxesToRemove) {
		UnregisterHitbox(remove);
	}
	hurtboxesToRemove->clear();
	hitboxesToRemove->clear();
	for (auto hitbox : *hitboxes) {
		if (hitbox->autoUnregister) {
			hitboxesToRemove->push_back(hitbox);
		}
		if (hitbox->creatorToEscape != nullptr) {
			if (!hitbox->creatorToEscape->aabb.overlaps(hitbox->aabb)) {
				hitbox->creatorToEscape = nullptr;
			}
		}
	}
	//process hit/hurt
	for (auto hitbox : *hitboxes){
		for (auto body : *hurtboxes) {
			if (hitbox->creatorToEscape!=body && ((hitbox->mask & body->takeDamageMask) != 0) && hitbox->aabb.overlaps(body->aabb)) {
				bool isValid = true;
				if (hitbox->bodiesAlreadyDamaged != nullptr) {
					for (auto i : *hitbox->bodiesAlreadyDamaged) {
						if (i == body) {
							isValid = false;
							break;
						}
					}
					if (isValid) {
						hitbox->bodiesAlreadyDamaged->push_back(body);
					}
				}
				if (hitbox->self == body) {
					isValid = false;
				}
				if (isValid) {
					bool shouldApplyForce = true;
					if (body->damageReciever != nullptr) {
						if (body->damageReciever->TakeDamage(hitbox->damageAmount, hitbox->stun, hitboxesToRemove)) {
							hurtboxesToRemove->push_back(body);
							shouldApplyForce = false;
						}
					}
					if (hitbox->assignCreatorToEscapeToMoveFastHitbox != nullptr) {
						body->moveFastHitbox.creatorToEscape = hitbox->assignCreatorToEscapeToMoveFastHitbox;
					}
					if (shouldApplyForce) {
						body->vel += hitbox->knockInDirectionAmount;
						Vector2 away = (body->aabb.center - hitbox->aabb.center).normalized();
						body->vel += away * hitbox->knockAwayAmount;
					}
				}
			}
		}
	}
	//process auto pickups
	for (auto autoPickup : *autoPickups) {
		if (autoPickup->body.aabb.overlaps(spelunker->body.aabb)) {
			autoPickup->PickedUp(spelunker);
			autopickupsToRemove->push_back(autoPickup);
		}
	}
	for (auto remove : *hurtboxesToRemove) {
		UnregisterHurtbox(remove);
	}
	for (auto remove : *hitboxesToRemove) {
		UnregisterHitbox(remove);
	}
	for (auto remove : *autopickupsToRemove) {
		autoPickups->erase(remove);
	}
	hurtboxesToRemove->clear();
	hitboxesToRemove->clear();
	autopickupsToRemove->clear();
}

LevelBlock* Level::GetBlock(int x, int y) {
	if (x < 0 || x >= blocksXRes || y >= blocksYRes || y < 0) {
		printf("ERROR!!!! OUT OF BOUNDS!!!!! (%d,%d)",x,y);
		return blocks;
	}
	int index = x+y*blocksXRes;
	return blocks + index;
}

void Level::PlayAudio(Ref<AudioStream> sound, Vector2 gridCoordPos) 
{
	AudioStreamPlayer2D* audioPlayer;
	if (freeAudioSources->size() == 0) 
	{
		audioPlayer = cast_to<AudioStreamPlayer2D>(audioSourceScene->instance());
		get_node("/root/GameScene/SpawnRoot")->add_child(audioPlayer);
	}
	else
	{
		audioPlayer = freeAudioSources->back();
		freeAudioSources->pop_back();
	}
	outstandingAudioSources->push_back(audioPlayer);
	audioPlayer->set_stream(sound);
	audioPlayer->set_position(GridToWorld(gridCoordPos));
	audioPlayer->play();
}

Level::Level()
{
}

Level::~Level()
{
#ifdef showDebugHitboxes
	delete allRids;
	delete freeRids;
#endif // showDebugHitboxes
	std::free(blocks);
	std::free(drawTypes);
	this->rng->free();
	delete hitboxes;
	delete hurtboxes;
	delete autoPickups;
}
