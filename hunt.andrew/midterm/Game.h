#pragma once
#include "Display.h"
#include "UnitManager.h"
#include "InputManager.h"
#include "Snake.h"
#include <Timer.h>
#include <assert.h>
#include "Level.h"

class Game : public Trackable
{
	public:
		//static instance creators
		static Game* getInstance() { assert(mGameInstance != nullptr); return mGameInstance; };
		static void initInstance() { mGameInstance = new Game; };

		//init
		void init(double fps);

		//cleanup
		void cleanUp();
		static void cleanUpInstance();

		//game loop funcitonality
		void doLoop();
		void gameUpate();

		//mutators and accessors
		double getFPS();
		double getFPSNum();
		void setFPS(double newFPS);

		UnitManager* getUnitManagerReference();
		Snake getSnake();
		Display* getDisplay();
		Level* getLevel(int id);
		Sprite getBackgroundColor();
		InputManager* getInput();

		void setSnake(Snake snake);

	private:
		//constructors
		Game() {};

		//deconstructors
		~Game();

		static Game* mGameInstance;			//Memory Pointer to the Game

		InputManager mInput;				//Input Manager Memory Reference

		UnitManager mUnitList;				//Unit Manager Memory Reference

		Display mDisplay;					//display of game

		Snake mSnake;						//snake instance of game

		std::vector<Level> mLevels;

		double mFPS = 30.0f;				//frame rate of game (in milisecs)
		double mFPSNumber = 30.0f;			//frame rate of game

		Sprite mBackgroundSprite;			//background sprite when clearing
};