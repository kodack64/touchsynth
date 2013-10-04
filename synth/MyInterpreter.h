#pragma once

#include "Interpreter.h"

class MyInterpreter:public Interpreter{
private:
public:
	MyInterpreter(){
		inputHandler=NULL;
	}
};