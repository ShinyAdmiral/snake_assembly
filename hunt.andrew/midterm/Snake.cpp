#pragma once
#include <cmath>
#include "Snake.h"
#include "Game.h"
#include <intrin.h>

//constructors
Snake::Snake()
{
	mLocation = Vector2D(-1, -1);
	mAnimation = Animation();
	mMoveTime = 200;
	mCurrentTime = 0;
	mSegments = std::vector<Unit*>();
	mDirection = Vector2D(1, 0);
	mSpeedRatio = 1;
	mTailLocation = Vector2D(-1, -1);
	mFoodCount = 0;
	mEdibleCount = 0;
}

Snake::Snake(Vector2D loc, Animation anim, float startSpeed)
{
	mLocation = loc;
	mAnimation = anim;
	mMoveTime = startSpeed;
	mSegments = std::vector<Unit*>();
	mDirection = Vector2D(1, 0);
	mCurrentTime = 0;
	mSpeedRatio = 1;
	mTailLocation = Vector2D(-1, -1);
	mFoodCount = 0;
	mEdibleCount = 0;
}

//functionality
void Snake::update() 
{
	//set up
	Game* game = Game::getInstance();
	auto input = game->getInput();
	double gameFPS = game->getFPS();

	int xAxis = input->getKeyState(Input::X_AXIS);
	int yAxis = input->getKeyState(Input::Y_AXIS);
	
	__m128d moveTimeVec = _mm_set1_pd(mMoveTime);
	__m128d speedRatioVec = _mm_set1_pd(mSpeedRatio);
	__m128d result = _mm_mul_pd(moveTimeVec, speedRatioVec);
	__m128d cmpRes = _mm_cmpgt_pd(_mm_set1_pd(mCurrentTime), result);
	int flags = _mm_movemask_pd(cmpRes);

	if (flags & 0x01) {	//checking for the greater than flag
		int VxDirection = xAxis;
		int VyDirection = yAxis;
		int VCxDirection = -mDirection.getX();
		int VCyDirection = -mDirection.getY();

		_asm {
			push eax
			push ebx

			mov eax, VxDirection
			cmp eax, 0

			je skip1
			mov ebx, VCxDirection
			cmp eax, ebx
			je resetAll
			jmp resetY

			skip1 :
				mov eax, VyDirection
				cmp eax, 0
				je resetAll
				mov ebx, VCyDirection
				cmp eax, ebx
				je resetAll
				jmp skipall

			resetAll :
				mov VxDirection, 0
				mov VyDirection, 0
				jmp skipall

			resetY :
				mov VyDirection, 0

				skipall :
				pop ebx
				pop eax
		};

		int i_x, i_y;
		__m128 xmm_dir = _mm_set_ps(0.0f, VyDirection, VxDirection, 0.0f);
		__m128 cmp = _mm_cmpneq_ps(xmm_dir, _mm_setzero_ps());
		flags = _mm_movemask_ps(cmp);
		__m128 xmm_mDirection = _mm_set_ps(mDirection.getX(), mDirection.getY(), 0.0f, 0.0f);

		if (flags != 0) {
			xmm_mDirection = _mm_set_ps(VxDirection, VyDirection, 0.0f, 0.0f);
			//temporary
			int i_x = _mm_extract_ps(xmm_mDirection, 3);
			int i_y = _mm_extract_ps(xmm_mDirection, 2);
			mDirection = Vector2D(reinterpret_cast<float&>(i_x), reinterpret_cast<float&>(i_y));
		}

		i_x = _mm_extract_ps(xmm_mDirection, 3);
		i_y = _mm_extract_ps(xmm_mDirection, 2);
		__m128 xmm_moveDist = _mm_set_ps(reinterpret_cast<float&>(i_x), reinterpret_cast<float&>(i_y), 0.0f, 0.0f);
		
		//remove later
		i_y = _mm_extract_ps(xmm_moveDist, 3);
		i_y = _mm_extract_ps(xmm_moveDist, 2);
		Vector2D moveDist = Vector2D(reinterpret_cast<float&>(i_x), reinterpret_cast<float&>(i_y));

		//get number of segment and get the tail location
		int length = mSegments.size();
		mTailLocation = mSegments[length - 1]->getLocation();

		__m128 xmm_mTailLocation = _mm_set_ps(mTailLocation.getX(), mTailLocation.getY(), 0.0f, 0.0f);

		//greater than check
		__m128i _m128i_length = _mm_set1_epi32(length);
		__m128i mask = _mm_cmpgt_epi64(_m128i_length, _mm_set1_epi64x(1));
		flags = _mm_movemask_epi8(mask);

		//for more than one segment
		if (flags != 0)
		{
			//set new location based on current direction and new direction
			__m128 xmm_mLocation = _mm_set_ps(mLocation.getX(), mLocation.getY(), 0.0f, 0.0f);
			__m128 xmm_new_loc_result = _mm_add_ps(xmm_mLocation, xmm_moveDist);
			xmm_new_loc_result = _mm_floor_ps(xmm_new_loc_result);
			i_x = _mm_extract_ps(xmm_new_loc_result, 3);
			i_y = _mm_extract_ps(xmm_new_loc_result, 2);
			setLocation(Vector2D(reinterpret_cast<float&>(i_x), reinterpret_cast<float&>(i_y)));

			//make a list of pass position
			std::vector<__m128> lastPosition;


			//set up and loop
			_m128i_length = _mm_set1_epi32(length);
			__m128i m_index = _mm_set1_epi32(0);
			__m128i m_end_limit = _mm_sub_epi32(_m128i_length, _mm_set1_epi32(1));
			mask = _mm_cmplt_epi32(m_index, m_end_limit);
			flags = _mm_movemask_epi8(mask);
			while (flags != 0) 
			{
				int i = _mm_extract_epi32(m_index, 4);
				Vector2D c_loc = mSegments[i]->getLocation();
				__m128 xmm_mLocation = _mm_set_ps(c_loc.getX(), c_loc.getY(), 0.0f, 0.0f);
				lastPosition.push_back(xmm_mLocation);

				m_index = _mm_add_epi32(m_index, _mm_set1_epi32(1));
				mask = _mm_cmplt_epi32(m_index, m_end_limit);
				flags = _mm_movemask_epi8(mask);
			}

			//for (int i = 0; i < length - 1; i++)
			//{
			//	Vector2D c_loc = mSegments[i]->getLocation();
			//	__m128 xmm_mLocation = _mm_set_ps(c_loc.getX(), c_loc.getY(), 0.0f, 0.0f);
			//	lastPosition.push_back(xmm_mLocation);
			//}


			//set second segment behind head
			__m128 xmm_segmentLoc = _mm_sub_ps(xmm_new_loc_result, xmm_mDirection);
			i_x = _mm_extract_ps(xmm_segmentLoc, 3);
			i_y = _mm_extract_ps(xmm_segmentLoc, 2);
			mSegments[0]->setLocation(Vector2D(reinterpret_cast<float&>(i_x), reinterpret_cast<float&>(i_y)));
			static_cast<Wall*>(mSegments[0])->updateDirection(xmm_mDirection);

			m_end_limit = _mm_set1_epi32(length);
			m_index = _mm_set1_epi32(1);
			mask = _mm_cmplt_epi32(m_index, m_end_limit);
			flags = _mm_movemask_epi8(mask);

			//set each one after that behind one another
			while (flags != 0) 
			{
				int i = _mm_extract_epi32(m_index, 4);
				__m128i m_index_sub = _mm_sub_epi32(m_index, _mm_set1_epi32(1));
				int im1 = _mm_extract_epi32(m_index_sub, 4);

				i_x = _mm_extract_ps(lastPosition[im1], 3);
				i_y = _mm_extract_ps(lastPosition[im1], 2);
				mSegments[i]->setLocation(Vector2D(reinterpret_cast<float&>(i_x), reinterpret_cast<float&>(i_y)));
				static_cast<Wall*>(mSegments[i])->updateDirection(lastPosition[im1]);

				m_index = _mm_add_epi32(m_index, _mm_set1_epi32(1));
				mask = _mm_cmplt_epi32(m_index, m_end_limit);
				flags = _mm_movemask_epi8(mask);
			}
		}

		else if (length == 1)
		{
			//set the new location of thw head
			__m128 xmm_mLocation = _mm_set_ps(mLocation.getX(), mLocation.getY(), 0.0f, 0.0f);
			__m128 xmm_new_loc_result = _mm_add_ps(xmm_mLocation, xmm_moveDist);
			xmm_new_loc_result = _mm_floor_ps(xmm_new_loc_result);
			i_x = _mm_extract_ps(xmm_new_loc_result, 3);
			i_y = _mm_extract_ps(xmm_new_loc_result, 2);
			setLocation(Vector2D(reinterpret_cast<float&>(i_x), reinterpret_cast<float&>(i_y)));

			//set the other segment behind the head
			__m128 xmm_segmentLoc = _mm_sub_ps(xmm_new_loc_result, xmm_mDirection);
			i_x = _mm_extract_ps(xmm_segmentLoc, 3);
			i_y = _mm_extract_ps(xmm_segmentLoc, 2);
			mSegments[0]->setLocation(Vector2D(reinterpret_cast<float&>(i_x), reinterpret_cast<float&>(i_y)));
			static_cast<Wall*>(mSegments[0])->updateDirection(xmm_mDirection);
		}

		//else just move the head
		else
		{
			__m128 xmm_mLocation = _mm_set_ps(mLocation.getX(), mLocation.getY(), 0.0f, 0.0f);
			__m128 xmm_new_loc_result = _mm_add_ps(xmm_mLocation, xmm_moveDist);
			xmm_new_loc_result = _mm_floor_ps(xmm_new_loc_result);
			i_x = _mm_extract_ps(xmm_new_loc_result, 3);
			i_y = _mm_extract_ps(xmm_new_loc_result, 2);
			setLocation(Vector2D(reinterpret_cast<float&>(i_x), reinterpret_cast<float&>(i_y)));
		}

		//reset current time
		mCurrentTime = 0;
	}
	else {
		double packedfloat[2] = {mCurrentTime, 0};
		__m128d currentTime = _mm_set1_pd(mCurrentTime);
		__m128d gamefps = _mm_set1_pd(gameFPS);
		__m128d addresult = _mm_add_pd(currentTime, gamefps);
		//__m128d packed = _mm_cvtpd_pd(addresult);
		_mm_store_pd(packedfloat, addresult);
		mCurrentTime = packedfloat[0];
	}

	//update animation
	mAnimation.update(gameFPS);
}

bool Snake::collision()
{
	//set up
	auto unitManager = Game::getInstance()->getUnitManagerReference();
	auto uList = unitManager->getUnitList();
	int length = uList.size();

	//loop through each unit
	for (int i = 0; i < length; i++) 
	{
		//is there collision?
		if (getLocation() == uList[i]->getLocation()) 
		{
			//with what?
			if (typeid(Wall).name() == typeid(*uList[i]).name())
			{
				//we lost
				return true;
			}
			else if(typeid(Food).name() == typeid(*uList[i]).name())
			{
				//destroy food and create new segment
				createSegment();
				long int tempID = uList[i]->getID();
				unitManager->instanceDestroy(tempID);

				//score up
				mFoodCount++;
				mEdibleCount++;
			}
			else if (typeid(PowerUp).name() == typeid(*uList[i]).name()) 
			{
				//determite type of power up and change speed
				if (static_cast<PowerUp*>(uList[i])->isPowerUpSpeed()) 
				{
					mSpeedRatio *= 0.6;
				}
				else
				{
					mSpeedRatio *= 1.0 / 0.6;
				}

				//increase score and destroy instance
				long int tempID = uList[i]->getID();
				unitManager->instanceDestroy(tempID);
				mEdibleCount++;
			}
		}
	}

	//still going in game
	return false;
}

void Snake::draw(Display& display) 
{
	//draw sprite to display
	int length = mSegments.size();
	for (int i = 0; i < length; i++)
		display.draw(mAnimation.getCurrentSprit(), mSegments[i]->getLocation());

	display.draw(mAnimation.getCurrentSprit(), mLocation);
}

void Snake::clearDisplay(Display& display)
{
	//clear segments
	Color backgroundColor(BackgroundColor::BLACK);
	Sprite backgroundSprite(' ', backgroundColor);
	display.draw(backgroundSprite, mTailLocation);
}

void Snake::createSegment()
{
	//set up
	Game* game = Game::getInstance();
	auto unitManager = game->getUnitManagerReference();

	//gather data for wall unit
	int length = mSegments.size();
	Vector2D loc;
	Vector2D prevDir;
	
	//create off of last segment or ehad
	if (length > 0)
	{
		loc = mSegments[length - 1]->getLocation();
		prevDir = static_cast<Wall*>(mSegments[length - 1])->getDirection();
	}
	else
	{
		loc = getLocation();
		prevDir = mDirection;
	}

	//makelocation and copy animation
	loc = Vector2D(loc.getX() - prevDir.getX(), loc.getY() - prevDir.getY());
	Animation anim = getAnimation();

	//create another unit
	Unit* segment = new Wall(loc, anim, prevDir);

	//create segment
	long int id = unitManager->instanceCreate(segment);
	mSegments.push_back(segment);
}

void Snake::init(int num) 
{
	for (int i = 0; i < num; i++)
		createSegment();
}

//accessors
Vector2D Snake::getLocation() 
{
	//return location
	return mLocation;
}

Animation Snake::getAnimation()
{
	//return animation
	return mAnimation;
}

int Snake::getFoodNumber()
{
	//return food eaten
	return mFoodCount;
}

int Snake::getEdibleNumber()
{
	//return edibles eaten
	return mEdibleCount;
}

//mutators
void Snake::setLocation(Vector2D loc) 
{
	//set new location
	mLocation = loc;
}

void Snake::setAnimation(Animation anim)
{
	//set new animation
	mAnimation = anim;
}