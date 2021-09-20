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
#include "Spider.h"
#include "DoorOpener.h"
#include "Godolmec.h"

class PrizeBox;
class Godolmec;
class DoorOpener;

Bullet* SpawnBullet(Level* level,Vector2 gridCoord);
Rock* SpawnRock(Level* level,Vector2 gridCoord);
Shotgun* SpawnShotgun(Level* level,Vector2 gridCoord);
Snake* SpawnSnake(Level* level,Vector2 gridCoord);
Rope* SpawnRope(Level* level,Vector2 gridCoord,Vector2 startVel);
Bomb* SpawnBomb(Level* level,Vector2 gridCoord,Vector2 startVel);
Bat* SpawnBat(Level* level,Vector2 gridCoord);
BloodSpurt* SpawnBloodSpurt(Level* level,Vector2 gridCoord);
PrizeBox* SpawnPrizeBox(Level* level, Vector2 gridCoord);
Spider* SpawnSpider(Level* level, Vector2 gridCoord);
DoorOpener* SpawnDoorOpener(Level* level, Vector2 gridCoord);
Godolmec* SpawnGodolmec(Level* level, Vector2 gridCoord);

//AutoPickup
AutoPickup* SpawnLargeGoldPile(Level* level, Vector2 gridCoord,float timeUntilActive);
AutoPickup* SpawnLargeBombBox(Level* level, Vector2 gridCoord,float timeUntilActive);
AutoPickup* SpawnSmallBombPile(Level* level, Vector2 gridCoord,float timeUntilActive);
AutoPickup* SpawnSmallRopePile(Level* level, Vector2 gridCoord,float timeUntilActive);

//float Random();