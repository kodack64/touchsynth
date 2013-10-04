
#pragma once

#include "package.h"
#include "b2Factory.h"

class MyPackage:public Package{
private:

	/*Audio*/
	AudioId snd_shot;
	AudioId snd_goal;
	ImageId img_back;

	/*Field*/
	int ballSize;
	int merg;
	int half;
	int maxBarLength;
	int frameCount;

	/*Finger*/
	TouchCursor tc[MAX_TOUCH];
	int tcUse[MAX_TOUCH];
	vector < pair<int,int> > psl;
	vector < pair<int,int> > psr;
	vector <int> pslid;
	vector <int> psrid;
	HANDLE touchMutex;

	/*Box2D*/
	b2Factory* b2m;


	/*Do Not Edit Below*/
private:
	static MyPackage* myInstance;
public:
	virtual int Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi);
	virtual int Reset();
	virtual int Close();
	virtual int End();
	virtual int Run();
	virtual int AudioCallback(float* in,float* out,int frames);
	virtual int GraphicCallback();
	virtual int InputCallback(TOUCH_EVENT ev,int id,int x,int y);

	static __declspec(dllexport) Package* CALLBACK CreateInstance();
	static __declspec(dllexport) void CALLBACK DestroyInstance();
	static __declspec(dllexport) void CALLBACK GetName(char* buf,int max);
};