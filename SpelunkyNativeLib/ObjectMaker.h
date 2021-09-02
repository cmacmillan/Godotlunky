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
#include "PrizeBox.h"
#include "AutoPickup.h"

Bullet* SpawnBullet(Level* level,Vector2 gridCoord);
Rock* SpawnRock(Level* level,Vector2 gridCoord);
Shotgun* SpawnShotgun(Level* level,Vector2 gridCoord);
Snake* SpawnSnake(Level* level,Vector2 gridCoord);
Rope* SpawnRope(Level* level,Vector2 gridCoord);
Bomb* SpawnBomb(Level* level,Vector2 gridCoord,Vector2 startVel);
Bat* SpawnBat(Level* level,Vector2 gridCoord);
BloodSpurt* SpawnBloodSpurt(Level* level,Vector2 gridCoord);
PrizeBox* SpawnPrizeBox(Level* level, Vector2 gridCoord);

//AutoPickup
AutoPickup* SpawnLargeGoldPile(Level* level, Vector2 gridCoord);
AutoPickup* SpawnLargeBombBox(Level* level, Vector2 gridCoord);
AutoPickup* SpawnSmallBombPile(Level* level, Vector2 gridCoord);
AutoPickup* SpawnSmallRopePile(Level* level, Vector2 gridCoord);

float Random();