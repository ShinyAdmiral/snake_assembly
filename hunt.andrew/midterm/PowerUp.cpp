#include "PowerUp.h"

//constructor
PowerUp::PowerUp()
{
	//set defaults
	mLocation = Vector2D(0, 0);
	mAnimation = Animation();
	mID = rand();
	mSpeedUp = true;
}

PowerUp::PowerUp(Vector2D loc, Animation anim, bool type)
{
	//set defaults
	mLocation = loc;
	mAnimation = anim;
	mID = rand();
	mSpeedUp = type;
}

//mutator
void PowerUp::changePowerUpType(bool type)
{
	//change type of powerup
	mSpeedUp = type;
}

//accessor
bool PowerUp::isPowerUpSpeed()
{
	//return type of powerup
	return mSpeedUp;
}