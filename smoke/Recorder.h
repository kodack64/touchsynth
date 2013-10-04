#pragma once

#include "Common.h"
#include "WaveShell.h"
#include "WaveLoad.h"

class Recorder{
private:
	bool playflag;
	bool recflag;
	long reccount;
	long playcount;
	WaveLoader *wl;
	WaveShell *wsl;
	WaveShell *wsr;
	float left;
	float right;
public:
	Recorder();
	virtual ~Recorder();
	virtual int Init();
	virtual int Start();
	virtual int Reset();
	virtual int Close();
	virtual int Next(float left,float right);
	virtual int Stop();
	virtual int Save();
	virtual int Play();
	virtual void Make();
	virtual float GetLeft(){return left;};
	virtual float GetRight(){return right;};
};
