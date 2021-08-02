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

	register_property("blockSize", &Level::blockSize, 0.0f);
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

LevelBlock* blocks;
DrawType* drawTypes;
int blocksXRes;
int blocksYRes;

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

void Level::_ready()
{
	blocksXRes = 30;
	blocksYRes = 30;
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
				bottomMultimesh->set_instance_transform_2d(bottomIndex++,Transform2D().scaled(Vector2(1,-1)).translated(Vector2(i*100,-j*100)));
				break;
			case DrawType::Top:
				topMultimesh->set_instance_transform_2d(topIndex++, Transform2D().scaled(Vector2(1,-1)).translated(Vector2(i*100,-j*100)));
				break;
			case DrawType::Normal:
				groundMultimesh->set_instance_transform_2d(normalIndex++, Transform2D().scaled(Vector2(1,-1)).translated(Vector2(i*100,-j*100)));
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
}

Level::~Level()
{
	_CSTDLIB_::free(blocks);
	_CSTDLIB_::free(drawTypes);
}
