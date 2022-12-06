#pragma once
#include "Unit.h"
#include "Wall.h"
#include "Food.h"
#include "PowerUp.h"
#include <assert.h>

class UnitManager : public Trackable {
	public:
		//constuctor
		UnitManager() {};

		//deconstructor
		~UnitManager() { cleanUp(); };

		//init and cleanup
		void cleanUp();

		//accessors
		Unit getUnit(long int id);
		std::vector<Unit*> getUnitList();

		//update functions
		void updateUnits(const double gameFPS, Vector2D boundary, InputManager input);
		void drawUnits(Display& display);

		//create and destroy functions
		long int instanceCreate(Vector2D loc = Vector2D(0,0), Animation anim = Animation());
		long int instanceCreate(Unit* newUnit);
		void instanceDestroy(long int id);

	private:
		std::vector<Unit*> mUnitList = std::vector<Unit*>();	//list of all units
		long int mNextID			= 0;						//id for each unit
};