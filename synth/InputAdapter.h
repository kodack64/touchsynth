#pragma once

#include "Common.h"
#include "InputHandler.h"

class InputAdapter{
private:
public:
	InputHandler* inputHandler;
	virtual bool Init()=0;
	virtual void SetInputHandler(InputHandler* snd){
		inputHandler=snd;
	};
	virtual void Close()=0;
	virtual void Run()=0;
	virtual void Stop()=0;
};