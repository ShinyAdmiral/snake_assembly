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
void Wall::updateDirection(__m128 newDir)
{
	int i_x = _mm_extract_ps(newDir, 3);
	int i_y = _mm_extract_ps(newDir, 2);
	mDirection = Vector2D(reinterpret_cast<float&>(i_x), reinterpret_cast<float&>(i_y));
}

void Wall::updateDirection(Vector2D newDir)
{
	mDirection = newDir;
}

//accessor
Vector2D Wall::getDirection()
{
	return mDirection;
}