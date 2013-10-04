
#pragma once

#include "Common.h"
#include "InputAdapter.h"

class KeyboardAdapter:public InputAdapter{
private:
	static KeyboardAdapter* myInstance;
	BYTE key[256];
	BYTE oldkey[256];
	int fps;

	HANDLE myHandle;
	bool f_init;
	bool f_loop;
	bool f_start;
	static unsigned int CALLBACK Launch(void*);
	virtual void Loop();

public:
	static KeyboardAdapter* CreateInstance();
	static void DestroyInstance();

	virtual bool Init();
	virtual void Run();
	virtual void Close();
	virtual void Stop();
};

