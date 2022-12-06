#pragma once
#include <string>
#include "Sprite.h"
#include <Vector2d.h>

const short int DEFAULT_WIDTH = 100;	//default width
const short int DEFAULT_HEIGHT = 50;	//defaul height

const Vector2D SCREEN_ORIGIN(0, 0);		//screen origin

//out of range message for error messages
const std::string OUT_OF_RANGE_MESSAGE = "!!!!!Image or Text out of Rang!!!!!";

//enum class for specific alignemt of text
enum class TextAlignment 
{
	RIGHT	= 0,
	CENTER	= 1,
	LEFT	= 2
};

class Display : public Trackable 
{
	public:
		//constuctors
		Display();
		Display(const Vector2D& size);
		
		//deconstuctors
		~Display() { cleanUp(); };
		
		//initializing and resseting
		void init(const Vector2D& size);
		void cleanUp();

		//draw funciton
		void clearDisplay(const Sprite& sprite);
		void draw(const Sprite& sprite, const Vector2D& loc);
		void writeText(const std::string& text, const Vector2D& loc, const Color& color);
		void writeText(const std::string& text, const Vector2D& loc, const Color& color, TextAlignment alignment);
		void changeTitle(const std::string& text);

		//accessors
		Vector2D getSize() const;

	private:
		//error message for invalid locations
		void errorMessage(const std::string& text);

		HANDLE mConsoleHandle;	//console Hanfle
		Vector2D mWindowSize;	//size of display
};