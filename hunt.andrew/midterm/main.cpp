#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <cstdlib>

#include <Vector2d.h>
#include <Timer.h>
#include <PerformanceTracker.h>
#include <MemoryTracker.h>
#include <Trackable.h>
#include "Game.h"

using namespace std;

int main(void)
{	
	//create game
	Game::initInstance();
	Game* game = Game::getInstance();
	game->init(30);
	
	//start loop
	game->doLoop();
	
	//clean up
	game->cleanUp();
	Game::cleanUpInstance();
	
	//See if any unallocated heap exists
	MemoryTracker::getInstance()->reportAllocations(cout);

	//pause before exiting
	system("pause");
	return 0;
}