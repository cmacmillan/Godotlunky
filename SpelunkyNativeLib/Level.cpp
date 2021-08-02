#include "Level.h"
#include <Node2D.hpp>
#include <stdlib.h>
#include <vector>
#include <MultiMeshInstance2D.hpp>
#include <MultiMesh.hpp>

void Level::_register_methods()
{
	register_method("_ready", &Level::_ready);
	register_method("_process", &Level::_process);

	register_property("groundTexture", &Level::groundTexture, Ref<Texture>());
	register_property("blockSize", &Level::blockSize, 0.0f);
}

LevelBlock* blocks;
int blocksXRes;
int blocksYRes;
vector<RID>* allCanvasIds;
vector<RID>* freeCanvasIds;

void Level::_init()
{
}

void Level::_ready()
{
	//Draw();
	Draw();
}

float k = 0.0f;
void Level::_process(float delta)
{
	k+=.0005f;
	this->translate(Vector2(k, 0));
	auto vs = VisualServer::get_singleton();
	//vs->force_draw();
	auto instance = MultiMeshInstance2D::_new();
	auto mm = new MultiMesh();
	instance->set_multimesh(mm);
}

RID Level::GetRid(VisualServer* server) {
	if (freeCanvasIds->size() > 0) {
		auto retr = freeCanvasIds->back();
		freeCanvasIds->pop_back();
		return retr;
	}
	allCanvasIds->push_back(server->canvas_item_create());
	return allCanvasIds->back();
}

void Level::Draw()
{
	auto vs = VisualServer::get_singleton();
	freeCanvasIds->clear();
	for (auto x : *allCanvasIds) {
		freeCanvasIds->push_back(x);
	}
	for (int i = 0; i < blocksXRes; i++) {
		for (int j = 0; j < blocksYRes; j++) {
			auto id = GetRid(vs);
			vs->canvas_item_clear(id);
			vs->canvas_item_set_parent(id, get_canvas_item());
			vs->canvas_item_add_texture_rect(id, Rect2(Point2(blockSize*i, blockSize*j), Point2(blockSize, blockSize)), groundTexture.ptr()->get_rid());
			/*
			auto xform = Transform2D().rotated(godot::Math::deg2rad(k));
			vs->canvas_item_set_transform(id, xform);
			*/
		}
	}
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
	blocksXRes = 300;
	blocksYRes = 300;
	blocks = (LevelBlock*)malloc(sizeof(LevelBlock) * blocksXRes*blocksYRes);
	allCanvasIds = new vector<RID>();
	freeCanvasIds = new vector<RID>();
}

Level::~Level()
{
	_CSTDLIB_::free(blocks);
	delete allCanvasIds;
	delete freeCanvasIds;
}
