#pragma once

#include "Common.h"
#include "InputAdapter.h"

class MidiAdapter:public InputAdapter{
private:
	static MidiAdapter* myInstance;

	HMIDIIN midiIn;
	HMIDIOUT midiOut;
	MIDIOUTCAPS midiOutCaps;
	MIDIINCAPS midiInCaps;
	int inDevNum;
	int outDevNum;
	int outDevCount;
	int inDevCount;
	bool f_start;
	bool f_open;

	static void CALLBACK MidiInProc(HMIDIIN hMidiIn,UINT wMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2);
	virtual void MidiInCallback(HMIDIIN hMidiIn,UINT wMsg,DWORD dwParam1,DWORD dwParam2);
	virtual void OnMimData(DWORD dwData, DWORD dwTimeStamp);

public:
	static MidiAdapter* CreateInstance();
	static void DestroyInstance();
	MidiAdapter(){};
	~MidiAdapter(){};

	virtual bool Init();
	virtual void Run();
	virtual void Close();
	virtual void Stop();
	virtual void SetInputHandler(InputHandler*);
};
