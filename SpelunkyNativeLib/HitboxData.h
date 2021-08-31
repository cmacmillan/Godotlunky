#pragma once
#include "Common.h"
#include <vector>

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
	//beautiful https://stackoverflow.com/questions/20925818/algorithm-to-check-if-two-boxes-overlap
	bool overlaps1D(float min1, float max1, float min2, float max2) {
		return max1 >= min2 && max2 >= min1;
	}
};

enum HitboxMask : unsigned int {
	Nothing = 0,
	Player = 1,
	Enemy = 2,
	Everything = 4294967295,
};

class Body;

struct HitboxData {
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
	void SetValues(SpelAABB box, int damageAmount, HitboxMask mask, Vector2 knockInDirectionAmount, float knockAwayAmount,bool stun,Body* self);
};
