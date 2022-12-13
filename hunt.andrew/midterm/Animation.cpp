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

	double volatile lengthTime;
	double volatile oneSecond = ONE_SECOND;
	double volatile animationFPS = mAnimationFPS;

	_asm {
		movsd xmm0, oneSecond
		movsd xmm1, animationFPS
		divsd xmm0, xmm1
		movsd lengthTime, xmm0
	}
	mLengthTime = lengthTime;
}

Sprite Animation::getCurrentSprit() 
{
	//return current frame of aniamtion
	return mSpriteList[mCurrentIndex];
}

void Animation::update(double frameTime)
{
	double volatile currentTime = mCurrentTime;
	double volatile lengthTime = mLengthTime;
	double volatile vframeTime = frameTime;
	int volatile animationLength = mAnimationLength;
	int volatile currentIndex;
	bool volatile resetAnimation = mResetAnimation;

	_asm {
		push eax
		//update index and add to time
		movsd xmm0, currentTime
		movsd xmm1, lengthTime
		divsd xmm0, xmm1
		cvttsd2si eax, xmm0
		mov currentIndex, eax

		movsd xmm0, currentTime
		movsd xmm1, vframeTime
		addsd xmm0, xmm1
		movsd currentTime, xmm0

		//reset if at the end of animation
		cmp eax, animationLength

		jg reset

		// else case
		mov resetAnimation, 0
		jmp done

		// if case
		reset:
		xor eax, eax
		cvtsi2sd xmm0, eax
		mov currentIndex, 0
		movsd currentTime, xmm0
		mov resetAnimation, 1

		done:
		pop eax
	}

	mCurrentTime = currentTime;
	mLengthTime = lengthTime;
	mCurrentIndex = currentIndex;
	mResetAnimation = resetAnimation;

}