#include "PrizeBox.h"
#include "Level.h"
#include <AnimatedSprite.hpp>
#include <AudioStream.hpp>
#include <AudioStreamPlayer2D.hpp>
using namespace godot::Math;

void PrizeBox::_register_methods()
{
	register_method("_ready", &PrizeBox::_ready);
	register_method("_process", &PrizeBox::_process);
}


#define prizeBoxProbabilitiesLength 3
constexpr float prizeBoxProbabilities[prizeBoxProbabilitiesLength] = {
	10.0f,//rope
	10.0f,//small bomb
	2.0f,//big bomb
};

constexpr float PrizeBoxProbSum() {
	float sum = 0.0f;
	for (int i = 0; i < prizeBoxProbabilitiesLength; i++) {
		sum += prizeBoxProbabilities[i];
	}
	return sum;
}

constexpr float prizeBoxProbSum = PrizeBoxProbSum();

void PrizeBox::_init(){}

void PrizeBox::_ready()
{
	level = Object::cast_to<Level>(this->get_node("/root/GameScene/Level"));
	body.Init(Vector2(.8f,.5f),Vector2(0,0),.1,5000,this,level,Vector2(0,0),true,1,HitboxMask::Item,this,this,true,true,nullptr);
	level->RegisterHurtbox(&body);
}

void PrizeBox::OpenBox(vector<HitboxData*>* hitboxesToRemove) 
{
	float rand = level->Random() * prizeBoxProbSum;
	int SpawnIndex=0;
	for (;rand>prizeBoxProbabilities[SpawnIndex]; SpawnIndex++) {
		rand -= prizeBoxProbabilities[SpawnIndex];
	}
	Vector2 pos=body.aabb.center;
	switch (SpawnIndex) {
	case 0:
		SpawnSmallRopePile(level,pos);
		break;
	case 1:
		SpawnSmallBombPile(level, pos);
		break;
	case 2:
		SpawnLargeBombBox(level, pos);
		break;
	}
	///////////////
	level->PlayAudio(level->boxOpenSFX,body.aabb.center);
	body.OnDestroy(hitboxesToRemove);
	queue_free();
}

void PrizeBox::_process(float delta)
{
	body.process(delta,true,true);
}

void PrizeBox::DoThrowAction()
{
	OpenBox(nullptr);
	level->UnregisterHurtbox(&body);
}

bool PrizeBox::TakeDamage(int damageAmount, bool stun, vector<HitboxData*>* hitboxesToRemove) 
{
	OpenBox(hitboxesToRemove);
	return true;
}
