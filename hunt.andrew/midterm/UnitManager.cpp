#include "UnitManager.h"

std::vector<Unit*> UnitManager::getUnitList()
{
	//return unit list
	return mUnitList;
}

void UnitManager::cleanUp() 
{
	//get size of unit list
	int size = mUnitList.size();

	//loop until matching id is found
	for (int i = 0; i < size; i++)
	{
		delete mUnitList[0];
		mUnitList[0] = nullptr;

		mUnitList.erase(mUnitList.begin());
	}
}

Unit UnitManager::getUnit(long int id) 
{
	//get size of unit list
	int size = mUnitList.size();
	
	//loop until matching id is found
	for (int i = 0; i < size; i++) 
	{
		if (id == mUnitList[i]->getID()) 
		{
			return *mUnitList[i];
		}
	}

	//else return random unit
	return *mUnitList[rand() % size-1];
}

void UnitManager::updateUnits(const double gameFPS, Vector2D boundary, InputManager input)
{
	//get size of unit list
	int size = mUnitList.size();

	//update each unit
	for (int i = 0; i < size; i++)
	{
		mUnitList[i]->update(gameFPS, boundary, input);
	}
}

void UnitManager::drawUnits(Display& display)
{
	//get size of unit list
	int size = mUnitList.size();

	//draw each unit
	for (int i = 0; i < size; i++)
	{
		mUnitList[i]->draw(display);
	}
}

long int UnitManager::instanceCreate(Vector2D loc, Animation anim)
{
	//create new unit
	Unit* newUnit = new Unit(loc, anim, mNextID);

	//put unit in list
	mUnitList.push_back(newUnit);

	//increment next id
	mNextID++;

	//return id
	return mNextID - 1;
}

long int UnitManager::instanceCreate(Unit * unitInput)
{
	//put unit in list
	mUnitList.push_back(unitInput);
	unitInput->setID(mNextID);

	//increment next id
	mNextID++;

	//return id
	return mNextID - 1;
}

void UnitManager::instanceDestroy(long int id)
{
	//get size of unit list
	int size = mUnitList.size();

	//error check
	if (size > 0) {

		//serach through each unit
		for (int i = 0; i < size; i++)
		{
			//erase unit of specified ID
			if (id == mUnitList[i]->getID())
			{
				delete mUnitList[i];
				mUnitList[i] = nullptr;
				mUnitList.erase(mUnitList.begin() + i);
				return;
			}
		}

		//delete random unit
		if (size != 1)
		{
			int ran = rand() % size;
			delete mUnitList[ran];
			mUnitList[ran] = nullptr;
			mUnitList.erase(mUnitList.begin() + rand() % size);
		}
		else 
		{
			int ran = rand() % size;
			delete mUnitList[ran];
			mUnitList[ran] = nullptr;
			mUnitList.erase(mUnitList.begin());
		}
	}
}