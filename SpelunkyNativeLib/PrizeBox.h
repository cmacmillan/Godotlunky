#pragma once
#include "Common.h"
#include <Node2D.hpp>
#include "Level.h"
#include "Body.h"

class PrizeBox : public Node2D, IDamageReciever, IThrowAction
{
	GODOT_CLASS(PrizeBox, Node2D);

public:
	Body body;
	Level* level;

	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);

	void OpenBox(vector<HitboxData*>* hitboxesToRemove);
	virtual void DoThrowAction();
	virtual bool TakeDamage(int damageAmount, bool stun, vector<HitboxData*>* hitboxesToRemove);
};

