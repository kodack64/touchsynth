
#include "MouseAdapter.h"
#include "Logger.h"
#include "ApplicationProperty.h"

MouseAdapter* MouseAdapter::myInstance=NULL;

MouseAdapter* MouseAdapter::CreateInstance(){
	if(myInstance!=NULL)return myInstance;
	else{
		myInstance=new MouseAdapter;
		return myInstance;
	}
}
void MouseAdapter::DestroyInstance(){
	SAFE_DELETE(myInstance);
}

bool MouseAdapter::Init(){
	Logger::Println("[MouseAdapter]	Initialize");
	cursorPos.x=0;
	cursorPos.y=0;
	cursorPosPrev.x=0;
	cursorPosPrev.y=0;
	myInputId=-1;
	myHandle=NULL;
	f_init=true;
	f_loop=false;
	f_start=false;

	fps = ApplicationProperty::ReadSetupInt("Mouse","Fps",60);

	Logger::Println("[MouseAdapter]	Launch Mouse Adapter");
	myHandle = (HANDLE)_beginthreadex(NULL,0,MouseAdapter::Launch,this,0,NULL);
	if(myHandle==NULL){
		Logger::Println("[MouseAdapter] Create Thread Fail");
		f_init=false;
		return false;
	}

	return true;
}

void MouseAdapter::Run(){
	f_start=true;
	return;
}

void MouseAdapter::Stop(){
	f_start=false;
}

void MouseAdapter::Close(){
	Logger::Println("[MouseAdapter]	Close");

	if(myHandle==NULL)return;

	Logger::Println("[MouseAdapter]	Killing Mouse Thread...");
	f_loop=false;
	WaitForSingleObject(myHandle,INFINITE);
	Logger::Println("[MouseAdapter]	End Mouse Thread");

	CloseHandle(myHandle);

	return;
}

void MouseAdapter::Loop(){
	f_loop=true;
	for(;f_loop;){
		if(f_start){
			cursorPosPrev.x=cursorPos.x;
			cursorPosPrev.y=cursorPos.y;
			GetCursorPos(&cursorPos);
			ScreenToClient(ApplicationProperty::hWnd,&cursorPos);
			if(GetAsyncKeyState(VK_LBUTTON) & 0x8000){
				if(myInputId==-1){
					myInputId = inputHandler->CreatePosition(cursorPos.x,cursorPos.y);
				}else{
					inputHandler->UpdatePosition(myInputId,cursorPos.x,cursorPos.y);
				}
			}else{
				if(myInputId>=0){
					inputHandler->DestroyPosition(myInputId);
					myInputId=-1;
				}
			}
		}
		Sleep(1000/10);
	}
}
unsigned int CALLBACK MouseAdapter::Launch(void* arg){
	MouseAdapter* me = (MouseAdapter*)arg;
	me->Loop();
	return 0;
}