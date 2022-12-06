#pragma once
#include "Unit.h"

class Food : public Unit
{
	public:
		//constructor
		Food();
		Food(Vector2D loc, Animation anim);

		//destructor
		~Food() {};
};