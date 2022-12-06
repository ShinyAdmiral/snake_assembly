#pragma once
#include "Level.h"
#include "Game.h"
#include <time.h>
#include <sstream>

Level::Level(std::string fileName)
{
	//set File Name
	mFileName = fileName;
}

void Level::loadLevel() 
{
	//open file
	std::ifstream fin;
	fin.open(mFileName, std::ios::in);

	//see if it opens?
	if (fin.fail()) 
	{
		std::cout << "Error: Level Failed to Load!";
		system("pause");
		exit(1);
	}

	//loop until the end of file
	while (!fin.eof()) 
	{
		//get line
		std::string line;
		std::getline(fin, line);

		//set variables depending on file Header
		if (line == "LEVEL NAME")
			mLevelName = getNextLine(fin);

		else if (line == "INTRO MESSAGE")
			mLevelIntroMessage = getNextLine(fin);

		else if (line == "VICTORY MESSAGE")
			mLevelVictoryMessage = getNextLine(fin);

		else if (line == "DEATH MESSAGE")
			mLevelDeathMessage = getNextLine(fin);

		else if (line == "START SPEED")
			mStartingWaitTime = stod(getNextLine(fin));

		else if (line == "POWERUP FREQUENCY")
			mPowerUpFrequency = stoi(getNextLine(fin));

		else if (line == "POINTS GAINED")
			mPointsForEating = stoi(getNextLine(fin));

		else if (line == "START LENGTH")
			mStartingSegments = stoi(getNextLine(fin));

		else if (line == "REQUIRED FOOD")
			mRequireFood = stoi(getNextLine(fin));

		else if (line == "SPEED POWERUP ANIMATION")
			mSpeedPowerUpAnim = makeAnimation(fin);

		else if (line == "SLOW POWERUP ANIMATION")
			mSlowPowerUpAnim = makeAnimation(fin);

		else if (line == "SNAKE ANIMATION")
			mSnakeAnim = makeAnimation(fin);

		else if (line == "WALL ANIMATION")
			mWallAnim = makeAnimation(fin);

		else if (line == "FOOD ANIMATION")
			mFoodAnim = makeAnimation(fin);

		else if (line == "OBJECT PLACEMENT")
			constructLevel(fin);
	}

	//close file
	fin.close();
}

std::string Level::getNextLine(std::ifstream& fin)
{
	//get next line 
	std::string input;
	std::getline(fin, input);
	return input;
}

Animation Level::makeAnimation(std::ifstream& fin)
{
	//declare sprite List
	std::vector<Sprite> spriteList;

	//loop through and build animation
	while (true)
	{
		//build color
		std::string type;
		std::getline(fin, type);

		//if END header, leave
		if (type == "END")
			break;

		std::string color;
		std::getline(fin, color);

		//get character dor sprite
		std::string sprite;
		std::getline(fin, sprite);

		//build and add sprite
		Sprite sp(std::stoi(sprite), fileToColor(type, color));
		spriteList.push_back(sp);
	}

	//make animation
	return Animation(spriteList, 5.0f);
}

Color Level::fileToColor(std::string type, std::string color)
{
	//color type depends on back or fronst string
	if (type == "back")
	{
		//retrun correct color
		if (color == "blue") return					Color(BackgroundColor::BLUE);
		else if (color == "red") return				Color(BackgroundColor::RED);
		else if (color == "green") return			Color(BackgroundColor::GREEN);
		else if (color == "turqoise") return		Color(BackgroundColor::TURQOISE);
		else if (color == "yellow") return			Color(BackgroundColor::YELLOW);
		else if (color == "purple") return			Color(BackgroundColor::PURPLE);
		else if (color == "light blue") return		Color(BackgroundColor::LIGHT_BLUE);
		else if (color == "light red") return		Color(BackgroundColor::LIGHT_RED);
		else if (color == "light green") return		Color(BackgroundColor::LIGHT_GREEN);
		else if (color == "light turqoise") return	Color(BackgroundColor::LIGHT_TURQOISE);
		else if (color == "light yellow") return	Color(BackgroundColor::LIGHT_YELLOW);
		else if (color == "light purple") return	Color(BackgroundColor::LIGHT_PURPLE);
		else if (color == "white") return			Color(BackgroundColor::WHITE);
		else if (color == "light grey") return		Color(BackgroundColor::LIGHT_GREY);
		else if (color == "grey") return			Color(BackgroundColor::GREY);
		else return									Color(BackgroundColor::BLACK);
	}
	else 
	{
		//retrun correct color
		if (color == "blue") return					Color(ForegroundColor::BLUE);
		else if (color == "red") return				Color(ForegroundColor::RED);
		else if (color == "green") return			Color(ForegroundColor::GREEN);
		else if (color == "turqoise") return		Color(ForegroundColor::TURQOISE);
		else if (color == "yellow") return			Color(ForegroundColor::YELLOW);
		else if (color == "purple") return			Color(ForegroundColor::PURPLE);
		else if (color == "light blue") return		Color(ForegroundColor::LIGHT_BLUE);
		else if (color == "light red") return		Color(ForegroundColor::LIGHT_RED);
		else if (color == "light green") return		Color(ForegroundColor::LIGHT_GREEN);
		else if (color == "light turqoise") return	Color(ForegroundColor::LIGHT_TURQOISE);
		else if (color == "light yellow") return	Color(ForegroundColor::LIGHT_YELLOW);
		else if (color == "light purple") return	Color(ForegroundColor::LIGHT_PURPLE);
		else if (color == "white") return			Color(ForegroundColor::WHITE);
		else if (color == "light grey") return		Color(ForegroundColor::LIGHT_GREY);
		else if (color == "grey") return			Color(ForegroundColor::GREY);
		else return									Color(ForegroundColor::BLACK);
	}
}

void Level::constructLevel(std::ifstream& fin)
{
	//get game instance
	Game* game = Game::getInstance();

	//size of level
	int width = (int)game->getDisplay()->getSize().getX();
	int height = (int)game->getDisplay()->getSize().getY();

	//go through each space in level
	for (int i = 0; i < height; i++)
	{
		//read line
		std::string row;
		getline(fin, row);

		for (int j = 0; j < width; j++)
		{
			//depedning on character, spawn object
			if (row[j] == 'O')
			{
				//spawn wall
				Unit* wall = new Wall(Vector2D(j, i), mWallAnim, Vector2D(0, 0));
				game->getUnitManagerReference()->instanceCreate(wall);
			}
			else if (row[j] == 'S')
			{
				//spawn snake
				mSnake = Snake(Vector2D(j, i), mSnakeAnim, (float)mStartingWaitTime);
				game->setSnake(mSnake);
			}
			else if (row[j] == 'F')
			{
				//get possible food location
				mSpawnLocations.push_back(Vector2D(j, i));
			}
			else if (row[j] == 'P')
			{
				//get possible powerup location
				mPowerLocations.push_back(Vector2D(j, i));
			}
		}
	}
}

void Level::spawnFood(UnitManager& uM)
{
	//get random spot
	int length = mSpawnLocations.size();
	srand((unsigned int)time(NULL));
	int ran = rand() % length;

	//spawn food
	Vector2D foodPos = mSpawnLocations[ran];
	Unit* food = new Food(foodPos, mFoodAnim);
	uM.instanceCreate(food);
}

void Level::spawnPowerUp(UnitManager& uM)
{
	//get random spot
	int length = mPowerLocations.size();
	srand((unsigned int)time(NULL));
	int ran = rand() % length;

	//create powerup pointer
	Vector2D powerPos = mPowerLocations[ran];
	Unit* power;

	//determine type
	int type = rand() % 2;
	if (type)
		power = new PowerUp(powerPos, mSpeedPowerUpAnim, true);
	else
		power = new PowerUp(powerPos, mSlowPowerUpAnim, false);

	//create instance
	long int id = uM.instanceCreate(power);

	//make sure there is no collision
	auto uList = uM.getUnitList();
	length = uM.getUnitList().size();
	for (int i = 0; i < length; i++)
	{
		//if colliding with another powerup, delete it
		if (power->getLocation() == uList[i]->getLocation() && power!= uList[i])
			if (typeid(PowerUp).name() == typeid(*uList[i]).name())
				uM.instanceDestroy(id);
	}
}

void Level::update()
{
	//set up
	auto uM = Game::getInstance()->getUnitManagerReference();
	auto uList = uM->getUnitList();
	int length = uList.size();
	bool found = false;

	//random chance to spawn powerup
	srand((unsigned int)time(NULL));
	int powerUpSpawn = rand() % 100;
	bool spawnPower = false;
	int powerCount = 0;

	//is lady luck with us?
	if (powerUpSpawn < mPowerUpFrequency)
		spawnPower = true;

	//check for other instances of food or powerup
	for (int i = 0; i < length; i++) 
	{
		if (typeid(Food).name() == typeid(*uList[i]).name())
			found = true;
		
		if (typeid(PowerUp).name() == typeid(*uList[i]).name())
			powerCount++;
	}
	
	//no food?
	if (!found)
		//spawn it
		spawnFood(*uM);

	//not enough powerups?
	if (spawnPower && powerCount < 3)
		//spawn one more
		spawnPowerUp(*uM);
}

void Level::updateScore()
{
	Game* game = Game::getInstance();

	std::string gameInfo;
	std::stringstream ss;
	ss << "Score:_" << calcScore(&game->getSnake()) << "_|_FPS:_" << game->getFPSNum();
	ss >> gameInfo;
	game->getDisplay()->changeTitle(gameInfo);
}

void Level::draw(Display& display)
{
	//set up unit list
	auto uM = Game::getInstance()->getUnitManagerReference();
	auto uList = uM->getUnitList();

	//draw only food and powerups
	int length = uList.size();
	for (int i = 0; i < length; i++)
	{
		if (typeid(Food).name()		== typeid(*uList[i]).name() ||
			typeid(PowerUp).name()	== typeid(*uList[i]).name())
			uList[i]->draw(display);
	}
}

void Level::clearDisplay(Display& display) 
{
	//set up unit list
	auto uM = Game::getInstance()->getUnitManagerReference();
	auto uList = uM->getUnitList();
	
	//create background
	Color backgroundColor(BackgroundColor::BLACK);
	Sprite backgroundSprite(' ', backgroundColor);

	//clear display only for food and powerups
	int length = uList.size();
	for (int i = 0; i < length; i++)
	{
		if (typeid(Food).name() == typeid(*uList[i]).name() ||
			typeid(PowerUp).name() == typeid(*uList[i]).name())
			display.draw(backgroundSprite, uList[i]->getLocation());
	}
}

//=========================
//Buffers
//=========================
bool Level::introBuffer()
{
	//set up
	Game* game = Game::getInstance();
	Sprite background = game->getBackgroundColor();
	auto input= game->getInput();
	double fps = game->getFPS();

	//display intro message
	game->getDisplay()->clearDisplay(background);
	game->getDisplay()->writeText(mLevelIntroMessage, Vector2D(50, 25), Color(ForegroundColor::WHITE), TextAlignment::CENTER);

	//buffer for input
	while (true)
	{
		//set up timer
		Timer timer;
		double executionTime = 0;
		timer.start();
		//gather input
		input->update();

		//exit if escape is pressed
		if (input->getKeyState(Input::QUIT_APPLICATION))
			return true;

		//continue if space is pressed
		else if (input->getKeyStatePressed(Input::SPACE))
			break;

		//stop timer and calc sleep time
		timer.stop();
		executionTime = timer.getElapsedTime();
		double sleepTime = fps - executionTime;

		//sleep for some time if sleep time is above 0
		if (sleepTime > 0)
			timer.sleep(sleepTime);
	}

	return false;
}

bool Level::deathBuffer()
{
	//set up
	Game* game = Game::getInstance();
	auto input = game->getInput();
	double fps = game->getFPS();

	//display Loss message
	game->getDisplay()->writeText(mLevelDeathMessage, Vector2D(50, 25), Color(ForegroundColor::WHITE), TextAlignment::CENTER);

	while (true)
	{
		//set up timer
		Timer timer;
		double executionTime = 0;
		timer.start();
		//gather input
		input->update();

		//exit if escape is pressed
		if (input->getKeyState(Input::QUIT_APPLICATION))
			return true;

		//restart if space is pressed
		else if (input->getKeyStatePressed(Input::ENTER))
			break;

		//stop timer and calc sleep time
		timer.stop();
		executionTime = timer.getElapsedTime();
		double sleepTime = fps - executionTime;

		//sleep for some time if sleep time is above 0
		if (sleepTime > 0)
			timer.sleep(sleepTime);
	}

	return false;
}

bool Level::victoryBuffer()
{
	//set up
	Game* game = Game::getInstance();
	auto input = game->getInput();
	double fps = game->getFPS();

	//display Victory message
	game->getDisplay()->writeText(mLevelVictoryMessage, Vector2D(50, 25), Color(ForegroundColor::WHITE), TextAlignment::CENTER);

	while (true)
	{
		//set up timer
		Timer timer;
		double executionTime = 0;
		timer.start();
		//gather input
		input->update();

		//exit if escape is pressed
		if (input->getKeyState(Input::QUIT_APPLICATION))
			return true;

		//continue to next level if enter is pressed
		else if (input->getKeyStatePressed(Input::ENTER))
			break;

		//stop timer and calc sleep time
		timer.stop();
		executionTime = timer.getElapsedTime();
		double sleepTime = fps - executionTime;

		//sleep for some time if sleep time is above 0
		if (sleepTime > 0)
			timer.sleep(sleepTime);
	}

	return false;
}

int Level::getPointsForEating()
{
	//return points for eating
	return mPointsForEating;
}

int Level::getStartingSegments()
{
	//return starting snake segments
	return mStartingSegments;
}

int Level::getRequireFood()
{
	//return required food 
	return mRequireFood;
}

int Level::calcScore(Snake* snake)
{
	//calc current score
	return snake->getEdibleNumber() * mPointsForEating;
}

std::string Level::getLevelIntroMessage()
{
	//return custom message
	return mLevelIntroMessage;
}

std::string Level::getLevelVictoryMessage()
{
	//return custom message
	return mLevelVictoryMessage;
}

std::string Level::getLevelDeathMessage() 
{
	//return custom message
	return mLevelDeathMessage;
}