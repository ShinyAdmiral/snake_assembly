#pragma once
#include "Unit.h"

Unit::Unit()
{
	//set defaults
	mLocation = Vector2D(-1, -1);
	mAnimation = Animation();
	mID = rand();
}

Unit::Unit(Vector2D loc, Animation anim, long int id)
{
	//set variables
	mLocation = loc;
	mAnimation = anim;
	mID = id;
}

void Unit::update(const double gameFPS, Vector2D boundary, InputManager input)
{
	//update animation
	mAnimation.update(gameFPS);
}

void Unit::draw(Display& display)
{
	//draw sprite to display
	display.draw(mAnimation.getCurrentSprit(), mLocation);
}


Vector2D Unit::getLocation() 
{
	//return location
	return mLocation;
}

Animation Unit::getAnimation() 
{
	//return animation
	return mAnimation;
}

long int Unit::getID()
{
	//return unit id
	return mID;
}

void Unit::setLocation(Vector2D loc)
{
	//set new location
	mLocation = loc;
}

void Unit::setAnimation(Animation anim)
{
	//set new animation
	mAnimation = anim;
}

void Unit::setID(long int newID) 
{
	//set new ID
	mID = newID;
}