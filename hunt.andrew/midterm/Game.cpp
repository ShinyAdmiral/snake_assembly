#pragma once
#include "Game.h"

//singleton instance
Game* Game::mGameInstance = nullptr;

Game::~Game() 
{
	//cleanup
	cleanUp();
}


void Game::init(double fps)
{
	//initialize framerate
	this->mFPSNumber = fps;
	this->mFPS = ONE_SECOND / fps;

	//set up room
	//set to default display
	mDisplay.init(Vector2D(DEFAULT_WIDTH, DEFAULT_HEIGHT));

	//set up levels
	mLevels.push_back(Level("Level1.txt"));
	mLevels.push_back(Level("Level2.txt"));
	mLevels.push_back(Level("Level3.txt"));

	Color backgroundColor(BackgroundColor::BLACK);
	mBackgroundSprite = Sprite(' ', backgroundColor);
}


void Game::cleanUp()
{
	//clean up display
	mDisplay.cleanUp();
	
	//clean up manager
	mInput.cleanup();
	mUnitList.cleanUp();
}

void Game::cleanUpInstance() 
{
	//clean up game Instance
	delete mGameInstance;
	mGameInstance = nullptr;
}

void Game::doLoop()
{

	//get size of level list
	int length = mLevels.size();

	//loop for each level
	for (int levelNum = 0; levelNum < length; levelNum++)
	{
		//==========================================
		// Level Initialization
		//==========================================

		//need an empty Unit List
		mUnitList.cleanUp();

		//set up room
		mLevels[levelNum].loadLevel();

		//create segments of snake
		int segmentsToCreate = mLevels[levelNum].getStartingSegments();
		mSnake.init(segmentsToCreate);

		//spawn food
		mLevels[levelNum].spawnFood(mUnitList);

		if (mLevels[levelNum].introBuffer()) return;

		//clear display
		mDisplay.clearDisplay(mBackgroundSprite);

		//draw uit
		mUnitList.drawUnits(mDisplay);
		mSnake.draw(mDisplay);

		//buffer so that way we don't exit right away
		bool inputReady = false;

		//did we win?
		bool victory = false;

		//perform game loop
		while (true)
		{
			//set up timer
			Timer timer;
			double executionTime = 0;
			timer.start();

			//==========================================
			// INPUT
			//==========================================

			//gather input
			mInput.update();

			//exit if escape is pressed
			if (mInput.getKeyState(Input::QUIT_APPLICATION) && inputReady)
				return;

			//==========================================
			// UPDATE
			//==========================================

			//run instance code for each instance
			mUnitList.updateUnits(mFPS, mDisplay.getSize(), mInput);
			mLevels[levelNum].update();
			mSnake.update();
			if (mSnake.collision())
				break;

			//update score
			mLevels[levelNum].updateScore();

			//==========================================
			// RENDER
			//==========================================

			//-----------------
			//commented out for efficiency 
			//-----------------
			//clear background
			//Color backgroundColor(BackgroundColor::BLACK);
			//Sprite backgroundSprite(' ', backgroundColor);
			//mDisplay.clearDisplay(backgroundSprite);

			//clear powerups and snake
			mLevels[levelNum].clearDisplay(mDisplay);
			mSnake.clearDisplay(mDisplay);

			//render each instance
			//-----------------
			//commented out for efficiency 
			//-----------------
			//mUnitList.drawUnits(mDisplay);
			mLevels[levelNum].draw(mDisplay);
			mSnake.draw(mDisplay);

			//==========================================
			// LOOP
			//==========================================

			//stop timer and calc sleep time
			timer.stop();
			executionTime = timer.getElapsedTime();
			double sleepTime = mFPS - executionTime;

			//sleep for some time if sleep time is above 0
			if (sleepTime > 0)
				timer.sleep(sleepTime);

			//check score, if we won, exit
			if (mSnake.getFoodNumber() >= mLevels[levelNum].getRequireFood())
			{
				victory = true;
				break;
			}

			//set input as ready
			if (!inputReady && !mInput.getKeyState(Input::QUIT_APPLICATION))
				inputReady = true;
		}

		//did we win?
		if (!victory)
		{
			//display loss message
			if (mLevels[levelNum].deathBuffer()) return;

			levelNum -= 1;
		}
		else
		{
			//display victory message
			if (mLevels[levelNum].victoryBuffer()) return;
		}
	}

	//===================================
	//GameOver
	//===================================

	bool replay = false;
	
	//prompt question
	mDisplay.clearDisplay(mBackgroundSprite);
	mDisplay.writeText("You Beat the Game! Press 'Escape' to Leave or 'Enter' to Replay!", Vector2D(50, 25), Color(ForegroundColor::WHITE), TextAlignment::CENTER);

	//buffer for input
	while (true)
	{
		//set up timer
		Timer timer;
		double executionTime = 0;
		timer.start();
		//gather input
		mInput.update();

		//exit if escape is pressed
		if (mInput.getKeyState(Input::QUIT_APPLICATION))
			return;

		//continue if enter is pressed
		else if (mInput.getKeyStatePressed(Input::ENTER)) 
		{
			replay = true;
			break;
		}

		//stop timer and calc sleep time
		timer.stop();
		executionTime = timer.getElapsedTime();
		double sleepTime = mFPS - executionTime;

		//sleep for some time if sleep time is above 0
		if (sleepTime > 0)
			timer.sleep(sleepTime);
	}

	//clear display
	mDisplay.clearDisplay(mBackgroundSprite);

	//shall we play again?
	if (replay)
		doLoop();
}

//obsolete for snake
void Game::gameUpate() 
{
	//check if enter key is pressed...
	if (mInput.getKeyStatePressed(Input::ENTER))
	{
		//check for maximum unit count
		if (mUnitList.getUnitList().size() <= 20) 
		{
			//create unit in random position
			float xPos = (float)(rand() % DEFAULT_WIDTH - 1);
			float yPos = (float)(rand() % DEFAULT_HEIGHT - 1);

			mUnitList.instanceCreate(Vector2D(xPos, yPos));
		}
	}

	//destroy random unit if delete key is pressed and the released
	if (mInput.getKeyStateReleased(Input::DEL))
	{
		mUnitList.instanceDestroy(-1);
	}
}

double Game::getFPS()
{
	//return fps
	return mFPS;
}

double Game::getFPSNum()
{
	//return fps
	return mFPSNumber;
}


void Game::setFPS(double newFPS)
{
	//set new fps
	mFPS = newFPS;
}

UnitManager* Game::getUnitManagerReference()
{
	//return unit list
	return &mUnitList;
}

Snake Game::getSnake()
{
	//return copy of snake
	return mSnake;
}

Display* Game::getDisplay()
{
	//return reference to display
	return &mDisplay;
}

void Game::setSnake(Snake snake)
{
	//set snake enitity
	mSnake = snake;
}

Level* Game::getLevel(int id)
{
	//get reference to current level
	return &mLevels[id];
}

Sprite Game::getBackgroundColor()
{
	//return copy of sprite
	return mBackgroundSprite;
}

InputManager* Game::getInput()
{
	return &mInput;
}