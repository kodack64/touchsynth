
#pragma once

#include "Common.h"
#include "waveshell.h"

class WaveLoader{
private:
	virtual unsigned char GetNext(FILE*);
public:
	virtual int LoadWaveFile(string,WaveShell*,WaveShell*);
	virtual int WriteWaveFile(string,WaveShell*,WaveShell*);
	virtual string GetOpenWaveFileName();
	virtual string GetSaveWaveFileName();
};
