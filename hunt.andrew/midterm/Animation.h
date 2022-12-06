#pragma once
#include "Sprite.h"
#include <vector>

const double ONE_SECOND = 1000.0;

class Animation : Trackable 
{
	public:
		//constructors
		Animation();
		Animation(std::vector<Sprite> arrayOfSprites, double fps);
		
		//deconstructors
		~Animation() {};

		//accessor for current sprite
		Sprite getCurrentSprit();

		//update animation
		void update(double frameTime);

	private:
		std::vector<Sprite> mSpriteList;	//list of sprites in animation
		int mAnimationLength	= 0;		//length of animatiion (in frames)
		int mCurrentIndex	= 0;			//current index of animation
		double mAnimationFPS = 0.0f;		//animation framerate (seperate from game)
		double mCurrentTime	= 0.0f;			//current time stamp in animations
		double mLengthTime	= 0.0f;			//max time of one frame
		bool mResetAnimation = false;		//reset animation?	
};