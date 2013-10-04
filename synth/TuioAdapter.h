
#pragma once

#include "InputAdapter.h"
#include "MyTuioListener.h"
#include <TuioClient.h>

class TuioAdapter:public InputAdapter{
private:
	static TuioAdapter* myInstance;
	TUIO::TuioClient* tuioClient;
	MyTuioListener* myTuioListener;

public:
	static TuioAdapter* CreateInstance();
	static void DestroyInstance();
	virtual bool Init();
	virtual void Close();
	virtual void Run();
	virtual void Stop();
};