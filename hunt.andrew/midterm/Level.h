#pragma once
#include "UnitManager.h"
#include "Snake.h"
#include <fstream>

class Level : public Trackable
{
	public:
		//constructor
		Level(std::string fileName);

		//destructor
		~Level() {};

		//functionality
		void loadLevel();
		void update();
		void updateScore();
		void spawnFood(UnitManager& uM);
		void spawnPowerUp(UnitManager& uM);
		void draw(Display& display);
		void clearDisplay(Display& display);

		//buffers
		bool introBuffer();
		bool deathBuffer();
		bool victoryBuffer();

		//accessors
		int getPointsForEating();
		int getStartingSegments();
		int getRequireFood();
		int calcScore(Snake* snake);

		//text accessors
		std::string getLevelIntroMessage();
		std::string getLevelVictoryMessage();
		std::string getLevelDeathMessage();

	private:
		//file ultility functions
		std::string getNextLine(std::ifstream& fin);
		Animation makeAnimation(std::ifstream& fin);
		Color fileToColor(std::string type, std::string color);
		void constructLevel(std::ifstream& fin);

		//file variables
		std::string mFileName;					//name of level file
		std::string mLevelName;					//level name
		std::string mLevelIntroMessage;			//level intro message
		std::string mLevelVictoryMessage;		//level victory message
		std::string mLevelDeathMessage;			//level death message
		Snake mSnake;							//snake Reference
		std::vector<Unit*> mWallList;			//Wall Unit List
		std::vector<Vector2D> mSpawnLocations;	//food Spawn Location
		std::vector<Vector2D> mPowerLocations;	//powerUp Spawn Locations
		int mRequireFood;						//required Food
		int mPowerUpFrequency;					//Power Up Frequency
		int mPointsForEating;					//Poinst for each thing eaten
		int mStartingSegments;					//starting segment number
		double mStartingWaitTime;				//speed of Snake

		//Animation
		Animation mSpeedPowerUpAnim;			//Animation of Speed Powerup
		Animation mSlowPowerUpAnim;				//Animation of Slow Powerup
		Animation mSnakeAnim;					//Animation of Snake
		Animation mFoodAnim;					//Animation of Food
		Animation mWallAnim;					//Animation of Wall
};