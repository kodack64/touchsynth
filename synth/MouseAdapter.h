#pragma once

#include "Common.h"
#include "InputAdapter.h"

class MouseAdapter:public InputAdapter{
private:
	static MouseAdapter* myInstance;

	POINT cursorPos;
	POINT cursorPosPrev;
	bool buttonState[3];
	bool buttonStatePrev[3];
	HANDLE myHandle;

	int myInputId;
	bool f_init;
	bool f_loop;
	bool f_start;
	int fps;
	static unsigned int CALLBACK Launch(void*);
	virtual void Loop();

public:
	static MouseAdapter* CreateInstance();
	static void DestroyInstance();
	MouseAdapter(){};
	~MouseAdapter(){};

	virtual bool Init();
	virtual void Run();
	virtual void Close();
	virtual void Stop();
};
