#include "Display.h"
#include <sstream>
#include <iostream>

Display::Display()
{ 
	//set up console hanlde
	init(Vector2D(DEFAULT_WIDTH, DEFAULT_HEIGHT));
}

Display::Display(const Vector2D& size) 
{
	//initialize
	init(size);
}

void Display::init(const Vector2D& size) 
{
	//get the handle for the console window
	mConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	//set the variables for size
	mWindowSize = size;
	SHORT sizeX = (SHORT)mWindowSize.getX();
	SHORT sizeY = (SHORT)mWindowSize.getY();

	//set the size of the window and screen buffer
	SMALL_RECT rect = { 0,0, sizeX - 1, sizeY - 1 };
	COORD displaySize = { sizeX, sizeY };
	bool retval;
	retval = SetConsoleScreenBufferSize(mConsoleHandle, displaySize);
	retval = SetConsoleWindowInfo(mConsoleHandle, false, &rect);

	//Name Console
	//send size to a stream and convert to a string
	std::string title;
	std::stringstream ss;
	ss << "XSize:" << sizeX << "__|__YSize:" << sizeY;
	ss >> title;

	//convert string to character string and display
	LPCTSTR consoleTitle = title.c_str();
	SetConsoleTitle(consoleTitle);
}

void Display::cleanUp() 
{
	Color black(BackgroundColor::BLACK);
	Sprite blank(' ', black);
	clearDisplay(blank);

	//get the handle for the console window
	mConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	//set the size of the window and screen buffer
	SMALL_RECT rect = { 0, 0, 0, 0};
	COORD displaySize = { 0, 0 };
	bool retval;
	retval = SetConsoleScreenBufferSize(mConsoleHandle, displaySize);
	retval = SetConsoleWindowInfo(mConsoleHandle, false, &rect);

	//set title
	LPCTSTR consoleTitle = "";
	SetConsoleTitle(consoleTitle);
}

void Display::clearDisplay(const Sprite& sprite) 
{
	//create a string
	std::string background = "";

	float loopLength = mWindowSize.getX() * mWindowSize.getY();

	//make the string fill up the window
	for (int i = 0; i < loopLength; i++)
	{
		//check for end of screen and a new line if need be
		if (i % (int)mWindowSize.getX() == 0 && i != 0) 
		{
			background += "\n";
		}

		//fill space
		background += sprite.getCharacter();
	}

	//draw to console
	writeText(background, SCREEN_ORIGIN, sprite.getColor());
}

void Display::draw(const Sprite& sprite, const Vector2D& loc)
{
	//create the string
	std::string text;
	text += sprite.getCharacter();

	//draw to screen
	writeText(text, loc, sprite.getColor());
}

void Display::writeText(const std::string& text, const Vector2D& loc, const Color& color)
{	
	//set location of text
	SHORT locX = (SHORT) loc.getX();
	SHORT locY = (SHORT) loc.getY();

	//error check
	if (locX < 0 || locX > mWindowSize.getX())
	{
		return;
		errorMessage(OUT_OF_RANGE_MESSAGE);
	}

	if (locY < 0 || locY > mWindowSize.getY())
	{
		return;
		errorMessage(OUT_OF_RANGE_MESSAGE);
	}

	//prepare buffer
	CONSOLE_SCREEN_BUFFER_INFO cInfo;
	GetConsoleScreenBufferInfo(mConsoleHandle, &cInfo);
	WORD oldColors = cInfo.wAttributes;
	COORD oldPos = cInfo.dwCursorPosition;

	//set color and location
	SetConsoleTextAttribute(mConsoleHandle, color.mColorValue);
	COORD newLoc = { locX, locY };
	SetConsoleCursorPosition(mConsoleHandle, newLoc);

	//write text to console
	DWORD numWritten;
	WriteConsole(mConsoleHandle, text.c_str(), (DWORD)text.size(), &numWritten, NULL);

	SetConsoleTextAttribute(mConsoleHandle, oldColors);
	SetConsoleCursorPosition(mConsoleHandle, oldPos);
}

void Display::writeText(const std::string& text, const Vector2D& loc, const Color& color, TextAlignment alignment)
{
	//set up by getting length of text
	int text_length = text.size();
	Vector2D newLoc;

	//y position should not be free
	newLoc.setY(loc.getY());

	//get the x position which might change
	float xPos = loc.getX();

	switch (alignment) 
	{
		//does not change
		case TextAlignment::RIGHT:
			newLoc.setX(xPos);
			break;
	
		//center text
		case TextAlignment::CENTER:
			newLoc.setX(xPos - (text_length/2));
			break;
	
		//move text's anchor point to the left
		case TextAlignment::LEFT:
			newLoc.setX(xPos - text_length);
			break;
	
		//set default and show error message
		default:
			errorMessage("alignment does not exits");
			newLoc.setX(xPos);
			break;
	}

	//run normal text function
	writeText(text, newLoc, color);
}

void Display::changeTitle(const std::string& text) 
{
	//convert string to character string and display
	LPCTSTR consoleTitle = text.c_str();
	SetConsoleTitle(consoleTitle);
}

void Display::errorMessage(const std::string& text) 
{
	//display error message
	std::cout << text;
}

Vector2D Display::getSize() const
{
	//return window size
	return mWindowSize;
}