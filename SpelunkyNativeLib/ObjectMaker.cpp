#pragma once
#include "ObjectMaker.h"

template <class T>
T* PutAtAndChildToLevel(T* node,Vector2 gridCoord, Level* level,bool spawnInFront) {
	node->set_position(level->GridToWorld(gridCoord));
	if (spawnInFront)
		level->frontSpawnRoot->add_child(node);
	else
		level->add_child(node);
	return node;
}

Bullet* SpawnBullet(Level* level,Vector2 gridCoord) { return PutAtAndChildToLevel(level->cast_to<Bullet>(level->bulletScene->instance()), gridCoord, level,true);}
Rock* SpawnRock(Level* level,Vector2 gridCoord) {return PutAtAndChildToLevel(level->cast_to<Rock>(level->rockScene->instance()), gridCoord, level,true);}
Shotgun* SpawnShotgun(Level* level,Vector2 gridCoord) { return PutAtAndChildToLevel(level->cast_to<Shotgun>(level->shotgunScene->instance()), gridCoord, level,true);}
Snake* SpawnSnake(Level* level,Vector2 gridCoord) { return PutAtAndChildToLevel(level->cast_to<Snake>(level->snakeScene->instance()), gridCoord, level,false);}
Bomb* SpawnBomb(Level* level,Vector2 gridCoord,Vector2 startVel) { 
	auto bomb = level->cast_to<Bomb>(level->bombScene->instance());
	bomb->startVelocity = startVel;
	return PutAtAndChildToLevel(bomb, gridCoord, level,true);
}
Rope* SpawnRope(Level* level,Vector2 gridCoord) { return PutAtAndChildToLevel(level->cast_to<Rope>(level->ropeScene->instance()), gridCoord, level,false);}
Bat* SpawnBat(Level* level,Vector2 gridCoord) { return PutAtAndChildToLevel(level->cast_to<Bat>(level->batScene->instance()), gridCoord, level,false);}
BloodSpurt* SpawnBloodSpurt(Level* level,Vector2 gridCoord) {  return PutAtAndChildToLevel(level->cast_to<BloodSpurt>(level->bloodSpurtScene->instance()), gridCoord, level,false);}
PrizeBox* SpawnPrizeBox(Level* level, Vector2 gridCoord) { return PutAtAndChildToLevel(level->cast_to<PrizeBox>(level->prizeBoxScene->instance()), gridCoord, level,true);}
Spider* SpawnSpider(Level* level, Vector2 gridCoord) { return PutAtAndChildToLevel(level->cast_to<Spider>(level->spiderScene->instance()), gridCoord, level,true);}
DoorOpener* SpawnDoorOpener(Level* level, Vector2 gridCoord) { return PutAtAndChildToLevel(level->cast_to<DoorOpener>(level->doorSwitchScene->instance()), gridCoord, level,false);}
Godolmec* SpawnGodolmec(Level* level, Vector2 gridCoord) { return PutAtAndChildToLevel(level->cast_to<Godolmec>(level->godolmecScene->instance()), gridCoord, level,false);}

//auto pickup
AutoPickup* SpawnLargeGoldPile(Level* level, Vector2 gridCoord,float timeUntilActive) { 
	auto pickup = level->cast_to<AutoPickup>(level->largeGoldScene->instance());
	pickup->timeUntilActive = timeUntilActive;
	return PutAtAndChildToLevel(pickup, gridCoord, level,true);
}
AutoPickup* SpawnLargeBombBox(Level* level, Vector2 gridCoord,float timeUntilActive) { 
	auto pickup = level->cast_to<AutoPickup>(level->largeBombBoxScene->instance());
	pickup->timeUntilActive = timeUntilActive;
	return PutAtAndChildToLevel(pickup, gridCoord, level,true);
}
AutoPickup* SpawnSmallBombPile(Level* level, Vector2 gridCoord,float timeUntilActive) { 
	auto pickup = level->cast_to<AutoPickup>(level->smallBombPileScene->instance());
	pickup->timeUntilActive = timeUntilActive;
	return PutAtAndChildToLevel(pickup, gridCoord, level,true);
}
AutoPickup* SpawnSmallRopePile(Level* level, Vector2 gridCoord,float timeUntilActive) { 
	auto pickup = level->cast_to<AutoPickup>(level->ropePileScene->instance());
	pickup->timeUntilActive = timeUntilActive;
	return PutAtAndChildToLevel(pickup, gridCoord, level,true);
}
//float Random() { return (std::rand() / (float)(RAND_MAX+1)); }


