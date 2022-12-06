#pragma once
#include "UnitManager.h"

class Snake : public Trackable 
{
	public:
		//constructors
		Snake();
		Snake(Vector2D loc, Animation anim, float startSpeed);

		//destructor
		~Snake() {};

		//functionality
		void update();
		bool collision();
		void draw(Display& display);
		void clearDisplay(Display& display);
		void createSegment();
		void init(int num);


		//accessors
		Vector2D getLocation();
		Animation getAnimation();
		int getFoodNumber();
		int getEdibleNumber();

		//mutators
		void setLocation(Vector2D loc);
		void setAnimation(Animation anim);

	private:
		Vector2D mLocation;					//location of unit
		Animation mAnimation;				//animation of unit
		double mMoveTime = 0;				//speed of snake
		double mCurrentTime = 0;			//current time for move
		double mSpeedRatio = 1;				//
		Vector2D mDirection;				//move direction
		std::vector<Unit*> mSegments;		//body segments of snake
		Vector2D mTailLocation;				//location of Tail

		int mFoodCount = 0;					//number of food ate
		int mEdibleCount = 0;				//number of edible consumed
};