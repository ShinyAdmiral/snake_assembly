#include "Color.h"

Color::Color() 
{
	//default color is black
	mColorValue = BLACK;
}

Color::Color(BackgroundColor color) 
{
	//set background color
	//constructor 
	mColorValue = static_cast<WORD>(color);
}

Color::Color(ForegroundColor color) 
{
	//set foreground color
	//constructor 
	mColorValue = static_cast<WORD>(color);
}

void Color::setColor(BackgroundColor color) 
{
	//set background color
	mColorValue = static_cast<WORD>(color);
}

void Color::setColor(ForegroundColor color) 
{
	//set foreground color
	mColorValue = static_cast<WORD>(color);
}