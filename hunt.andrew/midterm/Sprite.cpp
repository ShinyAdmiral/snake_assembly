#include "Sprite.h"

Sprite::Sprite() 
{
	//set default variables
	mCharacter = ' ';
	mColor.setColor(BackgroundColor::BLACK);
}

Sprite::Sprite(char character, Color color)
{
	//set variables
	this->mCharacter = character;
	this->mColor = color;
}

void Sprite::setCharacter(char newCharacter)
{
	//set shape of sprite
	this->mCharacter = newCharacter;
}

void Sprite::setColor(Color newColor) 
{
	//set color
	this->mColor = newColor;
}


char Sprite:: getCharacter() const
{
	//get character
	return mCharacter;
}

Color Sprite::getColor() const
{
	//get color
	return mColor;
}
