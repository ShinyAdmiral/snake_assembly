#pragma once
#include <Trackable.h>
#include <windows.h>
#include "InputEnum.h"

class InputManager : public Trackable 
{
	public:
		//contructor
		InputManager() {};

		//deconstructor
		~InputManager() { cleanup(); };

		//clean up
		void cleanup() {};

		//update keys
		void update();

		//get key states
		int getKeyState(Input input);
		int getKeyStatePressed(Input input);
		int getKeyStateReleased(Input input);

	private:
		int mXaxis						= 0;		// Input x axis 
		int mYaxis						= 0;		// Input y axis
													
		bool mEnterKeyState				= false;	// Input for Enter Key
		bool mDeleteKeyState			= false;	// Input for Delet Key
		bool mSpaceKeyState				= false;	// Input for Space Key
		bool mEscKeyState				= false;	// Input for Excape Key
													
		bool mEnterKeyStatePrev			= false;	// Previous Input for Enter Key
		bool mDeleteKeyStatePrev		= false;	// Previous Input for Delete Key
		bool mSpaceKeyStatePrev			= false;	// Previous Input for Space Key
													
		bool mEnterKeyStateReleased		= false;	// Input for released Enter Key
		bool mDeleteKeyStateReleased	= false;	// Input for released Delete Key
		bool mSpaceKeyStateReleased		= false;	// Input for released Space Key
													
		bool mEnterKeyStatePressed		= false;	// Input for pressed Enter Key
		bool mDeleteKeyStatePressed		= false;	// Input for pressed Delete Key
		bool mSpaceKeyStatePressed		= false;	// Input for pressed Space Key
};	