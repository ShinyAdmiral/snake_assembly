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
	//mCurrentIndex = (int)mCurrentTime / (int)mLengthTime;
	//mCurrentTime += frameTime;
	volatile int currentIndex = mCurrentIndex;
	volatile double lengthTime = mLengthTime;
	volatile double currentTime = mCurrentTime;
	volatile int animationLength = mAnimationLength;
	volatile bool resetAnimation = mResetAnimation;

	__asm {
		//mov eax, currentIndex
		//movq xmm0, qword ptr[lengthTime]
		//cvttsd2si ebx, xmm0
		//cmp eax, 0
		//jz skip_div
		//cmp ebx, 0
		//jz skip_div
		//div ebx
		//cvtsi2sd xmm0, ebx
		//skip_div:
		//mov eax, 0
		//movsd qword ptr[currentTime], xmm0
		//movsd xmm1, qword ptr[frameTime]
		//addsd xmm0, xmm1

	//	//cvttsd2si eax, qword ptr[animationLength]
	//	//cmp dword ptr[currentIndex], eax
	//	//jle skip_reset
	//	//
	//	//// Reset animation
	//	//mov currentIndex, 0
	//	//xorps xmm0, xmm0
	//	//movsd qword ptr[currentTime], xmm0
	//	//mov resetAnimation, 1
	//	//
	//	//skip_reset:
	//	//mov resetAnimation, 0
	//	// Do nothing
	}
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