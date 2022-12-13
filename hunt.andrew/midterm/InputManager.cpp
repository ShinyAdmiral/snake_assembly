#include "InputManager.h"
#include <intrin.h>
void InputManager::update() 
{
	//get previouse input
	mEnterKeyStatePrev	= mEnterKeyState;
	mDeleteKeyStatePrev = mDeleteKeyState;
	mSpaceKeyStatePrev	= mSpaceKeyState;

	//reset pressed and released inputs
	mSpaceKeyStateReleased	= false;
	mEnterKeyStateReleased	= false;
	mDeleteKeyStateReleased = false;

	mSpaceKeyStatePressed	= false;
	mEnterKeyStatePressed	= false;
	mDeleteKeyStatePressed	= false;

	//==========================================
	// Continuous Input
	//==========================================
	//make x_axis
	unsigned char scanCode = _inp(0x60);
	int right	= GetAsyncKeyState(VK_RIGHT)|| GetAsyncKeyState('D');
	int left	= GetAsyncKeyState(VK_LEFT)	|| GetAsyncKeyState('A');
	mXaxis		= right - left;

	//make y_axis
	int up		= GetAsyncKeyState(VK_UP)	|| GetAsyncKeyState('W');
	int down	= GetAsyncKeyState(VK_DOWN)	|| GetAsyncKeyState('S');
	mYaxis		= down - up;

	//store if space is currelty pressed
	mSpaceKeyState	= GetAsyncKeyState(VK_SPACE);

	//store if Enter is currelty pressed
	mEnterKeyState	= GetAsyncKeyState(VK_RETURN);
	//store if Delete is currelty pressed
	mDeleteKeyState = GetAsyncKeyState(VK_DELETE)	|| GetAsyncKeyState(VK_BACK);

	//store if escape is currelty pressed
	mEscKeyState = GetAsyncKeyState(VK_ESCAPE);

	//==========================================
	// Pressed Input
	//==========================================
	if (mSpaceKeyState	&& !mSpaceKeyStatePrev)  mSpaceKeyStatePressed	= true;
	if (mEnterKeyState	&& !mEnterKeyStatePrev)  mEnterKeyStatePressed = true;
	if (mDeleteKeyState	&& !mDeleteKeyStatePrev) mDeleteKeyStatePressed	= true;

	//==========================================
	// Released Input
	//==========================================
	if (!mSpaceKeyState	 && mSpaceKeyStatePrev)  mSpaceKeyStateReleased		= true;
	if (!mEnterKeyState	 && mEnterKeyStatePrev)  mEnterKeyStateReleased		= true;
	if (!mDeleteKeyState && mDeleteKeyStatePrev) mDeleteKeyStateReleased	= true;
}

int InputManager::getKeyState(Input input)
{
	//return input based on enum
	switch (input) 
	{
		case Input::X_AXIS:
			return mXaxis;
			break;

		case Input::Y_AXIS:
			return mYaxis;
			break;

		case Input::SPACE:
			return mSpaceKeyState;
			break;

		case Input::ENTER:
			return mEnterKeyState;
			break;

		case Input::DEL:
			return mDeleteKeyState;
			break;

		case Input::QUIT_APPLICATION:
			return mEscKeyState;
			break;
	}

	return false;
}

int InputManager::getKeyStatePressed(Input input)
{
	//return input based on enum
	switch (input)
	{
	case Input::SPACE:
		return mSpaceKeyStatePressed;
		break;

	case Input::ENTER:
		return mEnterKeyStatePressed;
		break;

	case Input::DEL:
		return mDeleteKeyStatePressed;
		break;
	}

	return false;
}

int InputManager::getKeyStateReleased(Input input)
{
	//return input based on enum
	switch (input)
	{
	case Input::SPACE:
		return mSpaceKeyStateReleased;
		break;

	case Input::ENTER:
		return mEnterKeyStateReleased;
		break;

	case Input::DEL:
		return mDeleteKeyStateReleased;
		break;
	}

	return false;
}