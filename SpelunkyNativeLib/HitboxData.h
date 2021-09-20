#pragma once
#include "DamageSource.h"
#include "Common.h"
#include <vector>
#include <Math.hpp>

struct SpelAABB
{
	Vector2 center;
	Vector2 size;
	bool overlaps(SpelAABB& other) {
		Vector2 half = size/2;
		Vector2 otherHalf= other.size/2;
		return overlaps1D(other.center.x - otherHalf.x, other.center.x + otherHalf.x, center.x - half.x, center.x + half.x) && \
			overlaps1D(other.center.y - otherHalf.y, other.center.y + otherHalf.y, center.y - half.y, center.y + half.y);
	}
	Vector2 unintersect(SpelAABB& other,float& magnitude) {
		Vector2 half = size/2;
		Vector2 otherHalf= other.size/2;
		float xOffset = uninstersect1D(other.center.x - otherHalf.x, other.center.x + otherHalf.x, center.x - half.x, center.x + half.x);
		float yOffset = uninstersect1D(other.center.y - otherHalf.y, other.center.y + otherHalf.y, center.y - half.y, center.y + half.y);
		if (abs(xOffset)-.05f < abs(yOffset)) //fudge to prevent stupid death
		{
			magnitude = xOffset;
			other.center.x += xOffset;
			return Vector2(godot::Math::sign(xOffset),0);
		}
		else 
		{
			magnitude = yOffset;
			other.center.y += yOffset;
			return Vector2(0,godot::Math::sign(yOffset));
		}
	}
	float uninstersect1D(float min1, float max1, float min2, float max2) {
		float increaseAmount = max2 - min1;
		float decreaseAmount = min2 - max1;
		if (increaseAmount < -decreaseAmount) {
			return increaseAmount;
		}
		else 
		{
			return decreaseAmount;
		}
	}
	//beautiful https://stackoverflow.com/questions/20925818/algorithm-to-check-if-two-boxes-overlap
	bool overlaps1D(float min1, float max1, float min2, float max2) {
		return max1 >= min2 && max2 >= min1;
	}
	bool overlaps(Vector2 point) {
		Vector2 half = size / 2;
		return center.x - half.x<point.x&& center.x + half.x>point.x && center.y - half.y<point.y&& center.y + half.y>point.y;
	}
};

enum class HitboxMask : unsigned int {
	Nothing = 0,
	Player = 1,
	Enemy = 2,
	Item = 4,
	Everything = 0xFFFF,
	ItemAndEnemy = Item | Enemy,
};


class Body;

struct HitboxData {
	DamageSource source;
	SpelAABB aabb;
	int damageAmount;
	HitboxMask mask;
	Vector2 knockInDirectionAmount;
	float knockAwayAmount;
	Body* self;
	Body* creatorToEscape;
	Body* assignCreatorToEscapeToMoveFastHitbox;
	bool stun;
	bool autoUnregister;
	std::vector<Body*>* bodiesAlreadyDamaged=nullptr;
	void InitOrClearBodiesAlreadyDamagedList();
	void SetValues(SpelAABB box, int damageAmount, HitboxMask mask, Vector2 knockInDirectionAmount, float knockAwayAmount,bool stun,Body* self,DamageSource source);
};
