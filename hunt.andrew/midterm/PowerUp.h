#pragma once
#include "Unit.h"

class PowerUp : public Unit 
{
	public:
		//constructor
		PowerUp();
		PowerUp(Vector2D loc, Animation anim, bool type);

		//destructor
		~PowerUp() {};

		//mutator
		void changePowerUpType(bool type);

		//accessor
		bool isPowerUpSpeed();

	private:
		bool mSpeedUp = false;	//powerup type
};