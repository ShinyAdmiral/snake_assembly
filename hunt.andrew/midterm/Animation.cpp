#pragma once
#include "Animation.h"

Animation::Animation() 
{
	//create a default animation
	//set colors
	Color green(ForegroundColor::GREEN);
	Color blue(ForegroundColor::BLUE);
	Color red(ForegroundColor::RED);
	Color yellow(ForegroundColor::YELLOW);

	//set animation sprite list
	std::vector<Sprite> playerFrames;

	//make sprites
	Sprite playerFrame1('P', green);
	Sprite playerFrame2('p', blue);
	Sprite playerFrame3('P', red);
	Sprite playerFrame4('p', yellow);

	//add sprites to list
	playerFrames.push_back(playerFrame1);
	playerFrames.push_back(playerFrame2);
	playerFrames.push_back(playerFrame3);
	playerFrames.push_back(playerFrame4);

	//set variables to default
	mSpriteList = playerFrames;
	mAnimationLength = mSpriteList.size() - 1;
	mCurrentIndex = 0;
	mAnimationFPS = 10.0f;
	mResetAnimation = false;
	mLengthTime = ONE_SECOND / mAnimationFPS;
}

Animation::Animation(std::vector<Sprite> arrayOfSprites, double fps)
{
	//initialize variables
	mSpriteList = arrayOfSprites;
	mAnimationLength = arrayOfSprites.size() - 1;
	mCurrentIndex = 0;
	mAnimationFPS = fps;
	mResetAnimation = false;
	mLengthTime = ONE_SECOND / mAnimationFPS;
}

Sprite Animation::getCurrentSprit() 
{
	//return current frame of aniamtion
	return mSpriteList[mCurrentIndex];
}

void Animation::update(double frameTime)
{
	//update index and add to time
	mCurrentIndex = (int)mCurrentTime / (int)mLengthTime;
	mCurrentTime += frameTime;

	//reset if at the end of animation
	if (mCurrentIndex > mAnimationLength)
	{
		mCurrentIndex = 0;
		mCurrentTime = 0;
		mResetAnimation = true;
	}

	else 
		mResetAnimation = false;
	
}