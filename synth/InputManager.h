#pragma once

#include "Common.h"
#include "InputAdapter.h"
#include "InputHandler.h"

typedef enum{
	ADP_MOUSE,
	ADP_KEYBOARD,
	ADP_TUIO,
	ADP_OPENCV,
	ADP_MIDI,
	ADP_SERIAL
}AdapterKind;

class InputManager{
private:
	static InputManager* myInstance;
	vector <InputAdapter*> inputList;
	InputHandler* inputHandler;

	int adpMouse;
	int adpKeyboard;
	int adpTuio;
	int adpOpenCV;
	int adpMidi;
	int adpSerial;
	map <int,int> adpMap;

public:
	static InputManager* CreateInstance();
	static void DestroyInstance();

	InputManager();
	virtual ~InputManager();

	virtual bool Init();
	virtual void Close();
	virtual void Run();

	virtual void SetAdapterState(AdapterKind,bool);

	virtual InputApi* GetAPI();
};