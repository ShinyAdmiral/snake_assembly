#pragma once
#include "Food.h"

//constructor
Food::Food() 
{
	//set defaults
	mLocation = Vector2D(0, 0);
	mAnimation = Animation();
	mID = rand();
}

Food::Food(Vector2D loc, Animation anim)
{
	//set variables
	mLocation = loc;
	mAnimation = anim;
	mID = rand();
}