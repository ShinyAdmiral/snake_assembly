#pragma once
#include <cmath>
#include "Snake.h"
#include "Game.h"

//constructors
Snake::Snake()
{
	mLocation = Vector2D(-1, -1);
	mAnimation = Animation();
	mMoveTime = 200;
	mCurrentTime = 0;
	mSegments = std::vector<Unit*>();
	mDirection = Vector2D(1, 0);
	mSpeedRatio = 1;
	mTailLocation = Vector2D(-1, -1);
	mFoodCount = 0;
	mEdibleCount = 0;
}

Snake::Snake(Vector2D loc, Animation anim, float startSpeed)
{
	mLocation = loc;
	mAnimation = anim;
	mMoveTime = startSpeed;
	mSegments = std::vector<Unit*>();
	mDirection = Vector2D(1, 0);
	mCurrentTime = 0;
	mSpeedRatio = 1;
	mTailLocation = Vector2D(-1, -1);
	mFoodCount = 0;
	mEdibleCount = 0;
}

//functionality
void Snake::update() 
{
	//set up
	Game* game = Game::getInstance();
	auto input = game->getInput();
	double gameFPS = game->getFPS();

	//move when its time
	if (mCurrentTime > mMoveTime * mSpeedRatio)
	{
		int xAxis = input->getKeyState(Input::X_AXIS);
		int yAxis = input->getKeyState(Input::Y_AXIS);
	
		//gather input
		if (xAxis != 0)
		{
			//cannot go backwards
			if (xAxis != -mDirection.getX())
				mDirection = Vector2D(xAxis, 0);
		}
		else if(yAxis != 0)
		{
			//cannot go backwards
			if (yAxis != -mDirection.getY())
				mDirection = Vector2D(0, yAxis);
		}
		
		//make vector for moving
		Vector2D moveDist = Vector2D(mDirection.getX(), mDirection.getY());
		
		//get number of segment and get the tail location
		int length = mSegments.size();
		mTailLocation = mSegments[length - 1]->getLocation();

		//for more than one segment
		if (length > 1)
		{
			//set the new location of thw head
			Vector2D newLocation = Vector2D(std::floor(mLocation.getX() + moveDist.getX()), std::floor(mLocation.getY() + moveDist.getY()));
			setLocation(newLocation);
			
			//make a list of pass position
			std::vector<Vector2D> lastPosition;
			for (int i = 0; i < length-1; i++) 
			{
				lastPosition.push_back(mSegments[i]->getLocation());
			}

			//set second segment behind head
			Vector2D segmentLoc = getLocation() - mDirection;
			mSegments[0]->setLocation(segmentLoc);
			static_cast<Wall*>(mSegments[0])->updateDirection(mDirection);

			//set each one after that behind one another
			for (int i = 1; i < length; i++)
			{
				mSegments[i]->setLocation(lastPosition[i-1]);
				static_cast<Wall*>(mSegments[i])->updateDirection(mSegments[i]->getLocation());
			}
		}

		else if (length == 1)
		{	
			//set the new location of thw head
			Vector2D newLocation = Vector2D(std::floor(mLocation.getX() + moveDist.getX()), std::floor(mLocation.getY() + moveDist.getY()));
			setLocation(newLocation);
			
			//set the other segment behind the head
			Vector2D segmentLoc = getLocation() - mDirection;
			mSegments[0]->setLocation(segmentLoc);
			static_cast<Wall*>(mSegments[0])->updateDirection(mDirection);
		}

		//else just move the head
		else
		{
			Vector2D newLocation = Vector2D(std::floor(mLocation.getX() + moveDist.getX()), std::floor(mLocation.getY() + moveDist.getY()));
			setLocation(newLocation);
		}
	
		//reset current time
		mCurrentTime = 0;
	}
	else
	//add to current time
		mCurrentTime += gameFPS;

	//update animation
	mAnimation.update(gameFPS);
}

bool Snake::collision()
{
	//set up
	auto unitManager = Game::getInstance()->getUnitManagerReference();
	auto uList = unitManager->getUnitList();
	int length = uList.size();

	//loop through each unit
	for (int i = 0; i < length; i++) 
	{
		//is there collision?
		if (getLocation() == uList[i]->getLocation()) 
		{
			//with what?
			if (typeid(Wall).name() == typeid(*uList[i]).name())
			{
				//we lost
				return true;
			}
			else if(typeid(Food).name() == typeid(*uList[i]).name())
			{
				//destroy food and create new segment
				createSegment();
				long int tempID = uList[i]->getID();
				unitManager->instanceDestroy(tempID);

				//score up
				mFoodCount++;
				mEdibleCount++;
			}
			else if (typeid(PowerUp).name() == typeid(*uList[i]).name()) 
			{
				//determite type of power up and change speed
				if (static_cast<PowerUp*>(uList[i])->isPowerUpSpeed()) 
				{
					mSpeedRatio *= 0.6;
				}
				else
				{
					mSpeedRatio *= 1.0 / 0.6;
				}

				//increase score and destroy instance
				long int tempID = uList[i]->getID();
				unitManager->instanceDestroy(tempID);
				mEdibleCount++;
			}
		}
	}

	//still going in game
	return false;
}

void Snake::draw(Display& display) 
{
	//draw sprite to display
	int length = mSegments.size();
	for (int i = 0; i < length; i++)
		display.draw(mAnimation.getCurrentSprit(), mSegments[i]->getLocation());

	display.draw(mAnimation.getCurrentSprit(), mLocation);
}

void Snake::clearDisplay(Display& display)
{
	//clear segments
	Color backgroundColor(BackgroundColor::BLACK);
	Sprite backgroundSprite(' ', backgroundColor);
	display.draw(backgroundSprite, mTailLocation);
}

void Snake::createSegment()
{
	//set up
	Game* game = Game::getInstance();
	auto unitManager = game->getUnitManagerReference();

	//gather data for wall unit
	int length = mSegments.size();
	Vector2D loc;
	Vector2D prevDir;
	
	//create off of last segment or ehad
	if (length > 0)
	{
		loc = mSegments[length - 1]->getLocation();
		prevDir = static_cast<Wall*>(mSegments[length - 1])->getDirection();
	}
	else
	{
		loc = getLocation();
		prevDir = mDirection;
	}

	//makelocation and copy animation
	loc = Vector2D(loc.getX() - prevDir.getX(), loc.getY() - prevDir.getY());
	Animation anim = getAnimation();

	//create another unit
	Unit* segment = new Wall(loc, anim, prevDir);

	//create segment
	long int id = unitManager->instanceCreate(segment);
	mSegments.push_back(segment);
}

void Snake::init(int num) 
{
	for (int i = 0; i < num; i++)
		createSegment();
}

//accessors
Vector2D Snake::getLocation() 
{
	//return location
	return mLocation;
}

Animation Snake::getAnimation()
{
	//return animation
	return mAnimation;
}

int Snake::getFoodNumber()
{
	//return food eaten
	return mFoodCount;
}

int Snake::getEdibleNumber()
{
	//return edibles eaten
	return mEdibleCount;
}

//mutators
void Snake::setLocation(Vector2D loc) 
{
	//set new location
	mLocation = loc;
}

void Snake::setAnimation(Animation anim)
{
	//set new animation
	mAnimation = anim;
}