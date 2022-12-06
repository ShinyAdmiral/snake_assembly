#pragma once
#include "Animation.h"
#include "Display.h"
#include "InputEnum.h"
#include "InputManager.h"

class Unit : public Trackable
{
	public:
		//constructor
		Unit();
		Unit(Vector2D loc, Animation anim, long int id);

		//destructor
		~Unit() {};

		//functionality
		virtual void update(const double gameFPS, Vector2D boundary, InputManager input);
		void draw(Display& display);

		//accessors
		Vector2D getLocation();
		Animation getAnimation();
		long int getID();

		//mutators
		void setLocation(Vector2D loc);
		void setAnimation(Animation anim);
		void setID(long int newID);

	protected:
		long int mID;			//id of the object
		Vector2D mLocation;		//location of unit
		Animation mAnimation;	//animation of unit
};