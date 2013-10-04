#pragma once

#include "Common.h"
#include "Package.h"
#include "DllManager.h"

#include "GraphicApi.h"
#include "AudioApi.h"
#include "InputApi.h"

class PackageManager{
private:
	static PackageManager* myInstance;
	vector<Package*> package;
	int choice;

	DllManager dllManager;

	GraphicApi* gapi;
	InputApi* iapi;
	AudioApi* aapi;

	bool endFlag;

public:
	PackageManager();
	virtual ~PackageManager();

	virtual bool Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi);
	virtual void Close();

	static PackageManager* CreateInstance();
	static void DestroyInstance();

	static void TimerCallbackProc(void);
	static void GraphicCallbackProc();
	static void InputCallbackProc(int ev,int id,int x,int y);
	static void AudioCallbackProc(float*,float*,int);

	static void BackToMenu();
	virtual void BackToMenuProc();
	static void BackToStart();
	virtual void BackToStartProc();

	virtual void TimerMain();
	virtual void AudioMain(float*,float*,int);
	virtual void GraphicMain();
	virtual void InputMain(int ev,int id,int x,int y);

	virtual bool IsActive(int choice);
	virtual bool IsEnd();
};