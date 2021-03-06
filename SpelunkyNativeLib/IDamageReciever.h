#pragma once
#include "DamageSource.h"
#include "Common.h"
#include <vector>
#include "HitboxData.h"

class IDamageReciever {
public:
	virtual bool TakeDamage(int damageAmount,bool stun,vector<HitboxData*>* hitboxesToRemove,DamageSource source) { return false; }//returns true if this hurtbox should unregister aka if the thing died
};
class ISmushReciever {
public:
	virtual void TakeSmush() = 0;
};

class IPicker {
public:
	virtual Vector2 GetPickPosition()=0;
	virtual void PickedBodyDestroyed()=0;
	virtual Body* GetBody()=0;
};

class IThrowAction {
public:
	virtual void DoThrowAction() = 0;
};
