
#include "KeyboardAdapter.h"
#include "Logger.h"
#include "ApplicationProperty.h"

KeyboardAdapter* KeyboardAdapter::myInstance=NULL;

KeyboardAdapter* KeyboardAdapter::CreateInstance(){
	if(myInstance!=NULL)return myInstance;
	else{
		myInstance=new KeyboardAdapter;
		return myInstance;
	}
}
void KeyboardAdapter::DestroyInstance(){
	SAFE_DELETE(myInstance);
}

bool KeyboardAdapter::Init()
{
	Logger::Println("[KeybaordAdapter]	Initialize");
	memset(key,0,sizeof(key));
	memset(oldkey,0,sizeof(oldkey));
	f_init=true;
	f_loop=false;
	f_start=false;

	fps = ApplicationProperty::ReadSetupInt("Keyboard","Fps",60);

	unsigned int id;
	Logger::Println("[KeybaordAdapter]	Launch Keyboard Thread");
	myHandle=(HANDLE)_beginthreadex(NULL,0,KeyboardAdapter::Launch,this,0,&id);
	if(myHandle==NULL){
		Logger::Println("[KeyboardAdapter]	CreateHandle Fail");
		f_init=false;
		return false;
	}
	return true;
}

void KeyboardAdapter::Loop(){
	f_loop=true;
	for(;f_loop;){
		if(f_start){
			memcpy(key,oldkey,sizeof(key));
			SetKeyboardState(key);
		}
		Sleep(1000/fps);
	}
	return;
}

void KeyboardAdapter::Close(void){
	if(f_init==NULL)return;

	Logger::Println("[KeyboardAdapter]	Killing Keyboard Thread ...");
	f_loop=false;
	WaitForSingleObject(myHandle,INFINITE);
	Logger::Println("[KeybaordAdapter]	Keyboard Thread End");
	CloseHandle(myHandle);
}

void KeyboardAdapter::Run(){
	f_start=true;
}

void KeyboardAdapter::Stop(){
	f_start=false;
}

unsigned int KeyboardAdapter::Launch(void* arg){
	((KeyboardAdapter*)arg)->Loop();
	return 0;
}