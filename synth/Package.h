#pragma once

#include <Windows.h>
#include "GraphicApi.h"
#include "InputApi.h"
#include "AudioApi.h"

typedef enum{
	TOUCH_ON,
	TOUCH_OFF,
	TOUCH_CONTINUE
}TOUCH_EVENT;

class Package{
private:
protected:
	InputApi* iapi;
	AudioApi* aapi;
	GraphicApi* gapi;
public:
	Package(){
		iapi=NULL;
		aapi=NULL;
		gapi=NULL;
	}
	virtual ~Package(){
	}
	virtual int Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi)=0;
	virtual int Reset()=0;
	virtual int End()=0;
	virtual int Close()=0;
	virtual int Run()=0;
	virtual int AudioCallback(float* in,float* out,int frames)=0;
	virtual int GraphicCallback()=0;
	virtual int InputCallback(TOUCH_EVENT ev,int id,int x,int y)=0;
};