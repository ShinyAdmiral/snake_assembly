#pragma once
#include "Color.h"

class Sprite : public Trackable
{
	public:
		//constuctors
		Sprite();
		Sprite(char character, Color color);

		//deconstructor
		~Sprite() {};

		//mutators
		void setCharacter(char newCharacter);
		void setColor(Color newColor);

		//accessors
		char getCharacter() const;
		Color getColor() const;

	private:
		char mCharacter;	//shape of sprite
		Color mColor;		//Color of sprite
};