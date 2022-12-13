#pragma once
#include "Unit.h"
#include <intrin.h>

class Wall : public Unit
{
	public:
		//constructors
		Wall();
		Wall(Vector2D loc, Animation anim, Vector2D dir);

		//destructor
		~Wall() {};

		//funciton
		//virtual void update(const double gameFPS, Vector2D boundary, InputManager input);

		//mutator
		void updateDirection(__m128 newDir);
		void updateDirection(Vector2D newDir);

		//accessor
		Vector2D getDirection();

	private:
		Vector2D mDirection;	//current direction
};