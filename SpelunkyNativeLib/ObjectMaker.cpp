#pragma once
#include "ObjectMaker.h"

Bullet* SpawnBullet(Level* level) { return level->cast_to<Bullet>(level->bulletScene->instance());}
Rock* SpawnRock(Level* level) {return level->cast_to<Rock>(level->rockScene->instance());}
Shotgun* SpawnShotgun(Level* level) { return level->cast_to<Shotgun>(level->shotgunScene->instance());}
Snake* SpawnSnake(Level* level) { return level->cast_to<Snake>(level->snakeScene->instance());}
Bomb* SpawnBomb(Level* level) { return level->cast_to<Bomb>(level->bombScene->instance());}
Rope* SpawnRope(Level* level) { return level->cast_to<Rope>(level->ropeScene->instance());}
Bat* SpawnBat(Level* level) { return level->cast_to<Bat>(level->batScene->instance());}
BloodSpurt* SpawnBloodSpurt(Level* level,Vector2 gridCoord) { 
	auto retr = level->cast_to<BloodSpurt>(level->bloodSpurtScene->instance());
	retr->set_position(level->GridToWorld(gridCoord));
	level->add_child(retr);
	return retr;
}
float Random() { return (std::rand() / (float)RAND_MAX); }


