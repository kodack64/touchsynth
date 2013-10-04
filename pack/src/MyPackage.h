
#pragma once

#include "package.h"

class MyPackage:public Package{
private:
	ImageId img1;
	ImageId img2;
	ImageId img3;
	AudioId snd1;
	AudioId snd2;




/*Do Not Edit Below*/
private:
	static MyPackage* myInstance;
public:
	virtual void Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi);
	virtual void Close();
	virtual void Run();
	virtual void AudioCallback(float* in,float* out,int frames);
	virtual void GraphicCallback();
	virtual void InputCallback(TOUCH_EVENT ev,int id,int x,int y);

	static __declspec(dllexport) Package* CALLBACK CreateInstance();
	static __declspec(dllexport) void CALLBACK DestroyInstance();
	static __declspec(dllexport) void CALLBACK GetName(char* buf,int max);
};