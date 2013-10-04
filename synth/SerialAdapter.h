
#pragma once

#include "Common.h"
#include "InputAdapter.h"

class SerialAdapter:public InputAdapter{
private:
	static SerialAdapter* myInstance;
	
	HANDLE hPort;
	COMMCONFIG comcfg;
	unsigned long ncomcfg;
	TCHAR comName[256];
	bool f_init;
	bool f_start;
	bool f_loop;
	HANDLE myHandle;
	int fps;

	virtual void SetupTimeout(COMMTIMEOUTS* timeout);
	static unsigned int CALLBACK Launch(void*);
	virtual void Loop();
public:
	static SerialAdapter* CreateInstance();
	static void DestroyInstance();

	virtual bool Init();
	virtual void Run();
	virtual void Close();
	virtual void Stop();
};

