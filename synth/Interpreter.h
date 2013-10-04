#pragma once

#include "InputHandler.h"

class Interpreter{
private:
public:
	InputHandler* inputHandler;
	virtual void SetHandler(InputHandler* handler){
		inputHandler=handler;
	}
};