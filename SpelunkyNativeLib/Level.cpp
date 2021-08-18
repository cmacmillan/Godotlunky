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

void HitboxData::SetValues(SpelAABB box, int damageAmount, HitboxMask mask, Vector2 knockInDirectionAmount, float knockAwayAmount,bool stun)
{
	this->aabb = box;
	this->damageAmount = damageAmount;
	this->mask = mask;
	this->knockInDirectionAmount = knockInDirectionAmount;
	this->knockAwayAmount = knockAwayAmount;
	this->autoUnregister = false;
	this->stun = stun;
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

	register_property("snakeScene", &Level::snakeScene, Ref<PackedScene>());
	register_property("rockScene", &Level::rockScene, Ref<PackedScene>());
}

const string layout1 = 
"XXXXXXXXXXXXXXXXXXXXXX\n\
 X00000000000000000000X\n\
 X00XXX00X000000000000X\n\
 X00XXX000000000S00000X\n\
 X00XXX000000000000000X\n\
 X00XXX000000000000000X\n\
 X00000S0X000000000000X\n\
 XX0000X0X000000000000X\n\
 X0X00S00000000R0WW000X\n\
 XXXXX0XXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXX";

//this should probably not be global
////////////////////////////////

void Level::CopyLayoutIntoBlocks(string layout,int x, int y) 
{
	std::istringstream iss(layout);
	for (std::string line; std::getline(iss, line); )
	{
		int xCurr = x;
		int len = line.length();
		for (int i = 0; i < len; i++) {
			if (line[i] == 'R') {
				auto rock =  cast_to<Rock>(rockScene->instance());
				rock->set_position(GridToWorld(Vector2(xCurr+.5f,y+.5f)));
				get_node("/root/GameScene/SpawnRoot")->add_child(rock);
				GetBlock(xCurr, y)->present = false;
				xCurr++;
			}
			else if (line[i] == 'S') {
				auto snake= cast_to<Snake>(snakeScene->instance());
				snake->set_position(GridToWorld(Vector2(xCurr+.5f,y+.5f)));
				this->add_child(snake);
				GetBlock(xCurr, y)->present = false;
				xCurr++;
			}
			else if (line[i] == 'W') {
				GetBlock(xCurr,y)->hasSpikes=true;
				GetBlock(xCurr,y)->present=false;
				xCurr++;
			}
			else if (line[i]=='X') {
				GetBlock(xCurr,y)->present=true;
				xCurr++;
			}
			else if (line[i]=='0') {
				GetBlock(xCurr,y)->present=false;
				xCurr++;
			}
		}
		y++;
	}
}

void Level::RegisterHurtbox(Body* hurtbox) {
	hurtboxes->insert(hurtbox);
}
void Level::UnregisterHurtbox(Body* hurtbox) {
	hurtboxes->erase(hurtbox);
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

void Level::_ready()
{
	blocksXRes = 30;
	blocksYRes = 30;
	worldBlockSize = 100;
	hitboxes = new std::set<HitboxData*>();
	hurtboxes = new std::set<Body*>();
	blocks = (LevelBlock*)malloc(sizeof(LevelBlock) * blocksXRes*blocksYRes);
	drawTypes = (DrawType*)malloc(sizeof(DrawType) * blocksXRes*blocksYRes);
	for (int i = 0; i < blocksXRes; i++) {
		for (int j = 0; j < blocksYRes; j++) {
			GetBlock(i, j)->hasRope = false;
			GetBlock(i, j)->indestructible= false;
			GetBlock(i, j)->hasSpikes= false;
			GetBlock(i, j)->bloody= false;
		}
	}
	printf("start copying layout");
	CopyLayoutIntoBlocks(layout1, 0, 0);
	printf("done copying layout");
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
			else if (!GetBlock(i, j)->present) {
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

std::vector<Body*>* hurtboxesToRemove = nullptr;
std::vector<HitboxData*>* hitboxesToRemove= nullptr;
void Level::_process(float delta)
{
	if (hurtboxesToRemove == nullptr) {
		hurtboxesToRemove = new std::vector<Body*>();
	}
	if (hitboxesToRemove == nullptr) {
		hitboxesToRemove = new std::vector<HitboxData*>();
	}
	//process hit/hurt
	for (auto hitbox : *hitboxes){
		for (auto body : *hurtboxes) {
			if (hitbox->creatorToEscape!=body && ((hitbox->mask & body->takeDamageMask) != 0) && hitbox->aabb.overlaps(body->aabb)) {
				if (body->damageReciever != nullptr) {
					if (body->damageReciever->TakeDamage(hitbox->damageAmount,hitbox->stun,hitboxesToRemove)) {
						hurtboxesToRemove->push_back(body);
					}
				}
				body->vel += hitbox->knockInDirectionAmount;
				Vector2 away = (body->aabb.center - hitbox->aabb.center).normalized();
				body->vel += away * hitbox->knockAwayAmount;
			}
		}
	}
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
	for (auto remove : *hurtboxesToRemove) {
		UnregisterHurtbox(remove);
	}
	for (auto remove : *hitboxesToRemove) {
		UnregisterHitbox(remove);
	}
	hurtboxesToRemove->clear();
	hitboxesToRemove->clear();
}

LevelBlock* Level::GetBlock(int x, int y) {
	if (x < 0 || x >= blocksXRes || y >= blocksYRes || y < 0) {
		printf("ERROR!!!! OUT OF BOUNDS!!!!! (%d,%d)",x,y);
		return blocks;
	}
	int index = x+y*blocksXRes;
	return blocks + index;
}

Level::Level()
{
}

Level::~Level()
{
	_CSTDLIB_::free(blocks);
	_CSTDLIB_::free(drawTypes);
	delete hitboxes;
	delete hurtboxes;
}
