#include "Level.h"
#include <Node2D.hpp>
#include <stdlib.h>
#include <vector>
#include <MultiMeshInstance2D.hpp>
#include <MultiMesh.hpp>
#include <QuadMesh.hpp>
#include <string>
#include <sstream>


void Level::_register_methods()
{
	register_method("_ready", &Level::_ready);
	register_method("_process", &Level::_process);

	register_property("groundMultimesh", &Level::groundMultimesh, Ref<MultiMesh>());
	register_property("topMultimesh", &Level::topMultimesh, Ref<MultiMesh>());
	register_property("bottomMultimesh", &Level::bottomMultimesh, Ref<MultiMesh>());
}

enum DrawType {
	Normal = 0,
	Top = 1,
	Bottom = 2,
	None = 3,
};

Level* Level::singleton = nullptr;
LevelBlock* blocks;
DrawType* drawTypes;
int blocksXRes;
int blocksYRes;
float worldBlockSize;

const string layout1 = 
"X00000000X\n\
 X00000000X\n\
 X00XXX000X\n\
 X00XXX000X\n\
 X00XXX000X\n\
 X00XXX000X\n\
 X00000000X\n\
 X00000000X\n\
 X00000000X\n\
 XXXXXXXXXX";

void Level::CopyLayoutIntoBlocks(string layout,int x, int y) 
{
	std::istringstream iss(layout);
	for (std::string line; std::getline(iss, line); )
	{
		int xCurr = x;
		int len = line.length();
		for (int i = 0; i < len; i++) {
			if (line[i]=='X') {
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


float Level::MarchVertical(float startY, float endY, float x1, float x2) {
	float dir = endY > startY ? 1.0 : -1.0;
	if (GetBlock(x1, endY)->present || GetBlock(x2, endY)->present) {
		if (dir > 0)
		{
			return floor(endY)-.0001f;
		}
		else
		{
			return ceil(endY)+.0001f;
		}
	}
	/*
	for (float f = startY; abs(f - endY) < 1; f += dir) {
		if (GetBlock(x1, f)->present || GetBlock(x2, f)->present) {
			if (dir > 0) 
			{
				return floor(f);
			}
			else 
			{
				return ceil(f);
			}
		}
	}
	*/
	return endY;
}

float Level::MarchHorizontal(float startX, float endX, float y1, float y2) {
	float dir = endX > startX ? 1.0 : -1.0;
	if (GetBlock(endX, y1)->present || GetBlock(endX,y2)->present) {
		if (dir > 0)
		{
			return floor(endX)-.0001f;
		}
		else
		{
			return ceil(endX)+.0001f;
		}
	}
	/*
	for (float f = startX; abs(f - endX) < 1; f += dir) {
		if (GetBlock(f,y1)->present || GetBlock(f,y2)->present) {
			if (dir > 0) 
			{
				return floor(f);
			}
			else 
			{
				return ceil(f);
			}
		}
	}
	*/
	return endX;
}
void Level::CheckCollisionWithTerrain(SpelAABB aabb, Vector2 previousPos, Vector2& endPos, Vector2& normal) 
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

	if (center.x > previousPos.x) {
		endPos.x = MarchHorizontal(previousPos.x+half.x,upperRight.x,prevUpperRight.y,prevLowerRight.y)-half.x;
	}
	else if (center.x < previousPos.x) {
		endPos.x = MarchHorizontal(previousPos.x - half.x, upperLeft.x, prevUpperLeft.y, prevLowerLeft.y)+half.x;
	}

	if (center.y > previousPos.y) {
		endPos.y = MarchVertical(previousPos.y + half.y, lowerLeft.y, prevLowerLeft.x, prevLowerRight.x)-half.y;
	}
	else if (center.y < previousPos.y)
	{
		endPos.y = MarchVertical(previousPos.y - half.y, upperLeft.y, prevUpperLeft.x, prevUpperRight.x)+half.y;
	}
}

void Level::_ready()
{
	blocksXRes = 30;
	blocksYRes = 30;
	worldBlockSize = 100;
	blocks = (LevelBlock*)malloc(sizeof(LevelBlock) * blocksXRes*blocksYRes);
	drawTypes = (DrawType*)malloc(sizeof(DrawType) * blocksXRes*blocksYRes);
	CopyLayoutIntoBlocks(layout1, 0, 0);
	UpdateMeshes();
}
void Level::UpdateMeshes() {
	int topCount = 0;
	int bottomCount = 0;
	int normalCount = 0;
	for (int i = 0; i < blocksXRes; i++) {
		for (int j = 0; j < blocksYRes; j++) {
			if (!GetBlock(i, j)->present) {
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
	int normalIndex = 0;
	int topIndex = 0;
	int bottomIndex= 0;
	for (int i = 0; i < blocksXRes; i++) {
		for (int j = 0; j < blocksYRes; j++) {
			switch (drawTypes[j*blocksXRes+i])
			{
			case DrawType::Bottom:
				bottomMultimesh->set_instance_transform_2d(bottomIndex++,Transform2D().scaled(Vector2(1,-1)).translated(Vector2(i*worldBlockSize+worldBlockSize/2.0f,-j*worldBlockSize-worldBlockSize/2.0f)));
				break;
			case DrawType::Top:
				topMultimesh->set_instance_transform_2d(topIndex++, Transform2D().scaled(Vector2(1,-1)).translated(Vector2(i*worldBlockSize+worldBlockSize/2.0f,-j*worldBlockSize-worldBlockSize/2.0f)));
				break;
			case DrawType::Normal:
				groundMultimesh->set_instance_transform_2d(normalIndex++, Transform2D().scaled(Vector2(1,-1)).translated(Vector2(i*worldBlockSize+worldBlockSize/2.0f,-j*worldBlockSize-worldBlockSize/2.0f)));
				break;
			default:
				break;
			}
		}
	}
}

void Level::_process(float delta)
{
}

LevelBlock* Level::GetBlock(int x, int y) {
	if (x < 0 || x >= blocksXRes || y >= blocksYRes || y < 0) {
		printf("ERROR!!!! OUT OF BOUNDS!!!!!");
		return blocks;
	}
	int index = x+y*blocksXRes;
	return blocks + index;
}

Level::Level()
{
	singleton = this;
}

Level::~Level()
{
	_CSTDLIB_::free(blocks);
	_CSTDLIB_::free(drawTypes);
}
