#pragma once
#include "Common.h"
#include "Bullet.h"
#include "Rock.h"
#include "Shotgun.h"
#include "Snake.h"
#include "Rope.h"
#include "Bomb.h"
#include "Bat.h"
#include "BloodSpurt.h"

Bullet* SpawnBullet(Level* level);
Rock* SpawnRock(Level* level);
Shotgun* SpawnShotgun(Level* level);
Snake* SpawnSnake(Level* level);
Rope* SpawnRope(Level* level);
Bomb* SpawnBomb(Level* level);
Bat* SpawnBat(Level* level);
BloodSpurt* SpawnBloodSpurt(Level* level,Vector2 gridCoord);
float Random();