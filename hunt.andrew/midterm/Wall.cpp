#pragma once
#include "Wall.h"

Wall::Wall()
{
	//set defaults
	mLocation = Vector2D(-1, -1);
	mAnimation = Animation();
	mID = rand();
	mDirection = Vector2D(1, 0);
}

Wall::Wall(Vector2D loc, Animation anim, Vector2D dir)
{
	//set defaults
	mLocation = loc;
	mAnimation = anim;
	mID = rand();
	mDirection = dir;
}

//mutator
void Wall::updateDirection(Vector2D newDir)
{
	mDirection = newDir;
}

//accessor
Vector2D Wall::getDirection()
{
	return mDirection;
}