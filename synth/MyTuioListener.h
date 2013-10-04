
#pragma once

#include "InputHandler.h"
#include <TuioListener.h>

class MyTuioListener:public TUIO::TuioListener{
private:
	InputHandler* inputHandler;
	map<int,int>idMap;
	int winx;
	int winy;
public:
	MyTuioListener();
	virtual ~MyTuioListener();
	void refresh(TUIO::TuioTime){}

	virtual void addTuioCursor(TUIO::TuioCursor*);
	virtual void updateTuioCursor(TUIO::TuioCursor*);
	virtual void removeTuioCursor(TUIO::TuioCursor*);
	virtual void addTuioObject(TUIO::TuioObject*);
	virtual void updateTuioObject(TUIO::TuioObject*);
	virtual void removeTuioObject(TUIO::TuioObject*);

	virtual void Init(InputHandler*);
};