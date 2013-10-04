
#include "InputHandler.h"
#include "PackageManager.h"
#include "Logger.h"

InputHandler* InputHandler::myInstance=NULL;
HANDLE InputHandler::hMutex=NULL;

InputHandler::InputHandler(){
}

InputHandler::~InputHandler(){
}

InputHandler* InputHandler::CreateInstance(){
	if(myInstance==NULL){
		myInstance=new InputHandler;
	}
	return myInstance;
}
void InputHandler::DestroyInstance(){
	SAFE_DELETE(myInstance);
}

bool InputHandler::Init(){
	Logger::Println("[InputHandler]	Initialize");
	touchCursorList.clear();
	deadCursorList.clear();
	hMutex=CreateMutex(NULL,false,NULL);
	if(hMutex==NULL){
		Logger::Println("[InputHandler] Create Mutex Fail");
		return false;
	}
	isLock=false;
	count=0;
	return true;
}

void InputHandler::Close(){
	Logger::Println("[InputHandler]	Close");
	CloseHandle(hMutex);
}

int InputHandler::GetCursorNum(){
	Lock();
	int a=count;
	UnLock();
	return a;
}
void InputHandler::GetCursorList(vector<TouchCursor> *send){
	Lock();
	send->clear();
	for(UINT i=0;i<touchCursorList.size();i++){
		if(touchCursorList[i].IsActive()){
			send->push_back(touchCursorList[i]);
		}
	}
	UnLock();
	return;
}
void InputHandler::GetCursorList(TouchCursor* send,int *num){
	Lock();
	for(UINT i=0;i<touchCursorList.size();i++){
		send[i]=touchCursorList[i];
	}
	(*num)=touchCursorList.size();
	UnLock();
}

unsigned int InputHandler::CreatePosition(int x,int y){
	Lock();
	int id;
	if(deadCursorList.empty()){
		id=touchCursorList.size();
		touchCursorList.push_back(TouchCursor(id,x,y));
	}else{
		id=deadCursorList[0];
		touchCursorList[id].SetActive(true);
		touchCursorList[id].SetX(x);
		touchCursorList[id].SetY(y);
		deadCursorList.erase(deadCursorList.begin());
	}
	Logger::Debugln("[InputHandler]	Touch No.%d",id);
	PackageManager::InputCallbackProc(0,id,x,y);
	UnLock();
	return id;
}
void InputHandler::UpdatePosition(unsigned int id,int x,int y){
	Lock();
	if(0<=id && id<touchCursorList.size()){
		touchCursorList[id].SetX(x);
		touchCursorList[id].SetY(y);
	}
	PackageManager::InputCallbackProc(1,id,x,y);
	UnLock();
}
void InputHandler::UpdatePosition(unsigned int id,int x,int y,float velocity,float accel,long life){
	Lock();
	if(0<=id && id<touchCursorList.size()){
		touchCursorList[id].SetX(x);
		touchCursorList[id].SetY(y);
		touchCursorList[id].SetVelocity(velocity);
		touchCursorList[id].SetAccel(accel);
		touchCursorList[id].SetLife(life);
	}
	PackageManager::InputCallbackProc(1,id,x,y);
	UnLock();
}

void InputHandler::DestroyPosition(unsigned int id){
	Lock();
	if(0<=id && id<touchCursorList.size()){
		touchCursorList[id].SetActive(false);
		deadCursorList.push_back(id);
	}
	count--;
	PackageManager::InputCallbackProc(2,id,0,0);
	Logger::Debugln("[InputHandler]	UnTouch No.%d",id);
	UnLock();
}


void InputHandler::Lock(){
	WaitForSingleObject(hMutex,INFINITE);
	isLock=true;
}
void InputHandler::UnLock(){
	ReleaseMutex(hMutex);
	isLock=false;
}