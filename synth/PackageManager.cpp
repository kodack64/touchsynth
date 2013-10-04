
#include "PackageManager.h"
#include "Logger.h"
#include "Package.h"
#include "MasterPackage.h"

PackageManager* PackageManager::myInstance=NULL;

PackageManager::PackageManager()
:gapi(NULL)
,aapi(NULL)
,iapi(NULL){
}

PackageManager::~PackageManager(){
}

PackageManager* PackageManager::CreateInstance(){
	if(myInstance==NULL){
		myInstance=new PackageManager;
	}
	return myInstance;
}
void PackageManager::DestroyInstance(){
	SAFE_DELETE(myInstance);
}


bool PackageManager::Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi){
	Logger::Println("[PackageManager]	Initialize");

	gapi=_gapi;
	aapi=_aapi;
	iapi=_iapi;

	dllManager.Init();
	dllManager.Search();

	package.clear();
	package.push_back(MasterPackage::CreateInstance());
	package[0]->Init(gapi,aapi,iapi);
	for(int i=0;i<dllManager.GetDllNum();i++){
		dllManager.GetDllPath(i);
		package.push_back(dllManager.CreatePackageInstance(i));
		package[package.size()-1]->Init(gapi,aapi,iapi);
	}

	endFlag=false;
	choice=0;
	return true;
}

void PackageManager::Close(){
	Logger::Println("[PackageManager]	Close");
	endFlag=true;
	package[0]->Close();
	MasterPackage::DestroyInstance();
	for(unsigned int i=1;i<package.size();i++){
		package[i]->Close();
		dllManager.DestroyPackageInstance(i-1);
	}
	dllManager.Close();
}

void PackageManager::TimerMain(){
	if(IsActive(choice)){
		int res=package[choice]->Run();
		if(res!=0){
			if(choice==0){
				choice=res;
				package[choice]->Reset();
			}
			else{
				package[choice]->End();
				choice=0;
			}
		}
	}
}

void PackageManager::AudioMain(float* in,float* out,int frames){
	if(IsActive(choice)){
		package[choice]->AudioCallback(in,out,frames);
	}
	else{
		for(int i=0;i<frames;i++){
			*out++ = 0.0f;
			*out++ = 0.0f;
		}
	}
}

void PackageManager::GraphicMain(){
	if(IsActive(choice)){
		package[choice]->GraphicCallback();
	}
}

void PackageManager::InputMain(int ev,int id,int x,int y){
	if(IsActive(choice)){
		switch(ev){
		case 0:
			package[choice]->InputCallback(TOUCH_ON,id,x,y);
			break;
		case 1:
			package[choice]->InputCallback(TOUCH_CONTINUE,id,x,y);
			break;
		case 2:
			package[choice]->InputCallback(TOUCH_OFF,id,x,y);
			break;
		}
	}
}

bool PackageManager::IsEnd(){
	return endFlag;
}

void PackageManager::TimerCallbackProc(){
	if(myInstance!=NULL){
		myInstance->TimerMain();
	}
}
void PackageManager::AudioCallbackProc(float* in,float* out,int frames){
	if(myInstance!=NULL){
		myInstance->AudioMain(in,out,frames);
	}
}

void PackageManager::GraphicCallbackProc(){
	if(myInstance!=NULL){
		myInstance->GraphicMain();
	}
}

void PackageManager::InputCallbackProc(int ev,int id,int x,int y){
	if(myInstance!=NULL){
		myInstance->InputMain(ev,id,x,y);
	}
	return;
}

bool PackageManager::IsActive(int c){
	if(0<=c && c<(signed)package.size()){
		if(package[c]!=NULL && !endFlag)	return true;
		else return false;
	}else return false;
}

void PackageManager::BackToMenu(){
	myInstance->BackToMenuProc();
}

void PackageManager::BackToMenuProc(){
	package[choice]->End();
	choice=0;
	package[choice]->Reset();
}

void PackageManager::BackToStart(){
	myInstance->BackToStartProc();
}

void PackageManager::BackToStartProc(){
	package[choice]->End();
	package[choice]->Reset();
}