#pragma once
#include "ObjectMaker.h"

Bullet* SpawnBullet(Level* level) { return level->cast_to<Bullet>(level->bulletScene->instance());}
Rock* SpawnRock(Level* level) {return level->cast_to<Rock>(level->rockScene->instance());}
Shotgun* SpawnShotgun(Level* level) { return level->cast_to<Shotgun>(level->shotgunScene->instance());}
Snake* SpawnSnake(Level* level) { return level->cast_to<Snake>(level->snakeScene->instance());}
Bomb* SpawnBomb(Level* level) { return level->cast_to<Bomb>(level->bombScene->instance());}
Rope* SpawnRope(Level* level) { return level->cast_to<Rope>(level->ropeScene->instance());}
float Random() { return (std::rand() / (float)RAND_MAX); }


