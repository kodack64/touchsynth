
#include "TuioAdapter.h"
#include "ApplicationProperty.h"
#include "Logger.h"

TuioAdapter* TuioAdapter::myInstance=NULL;

TuioAdapter* TuioAdapter::CreateInstance(){
	if(myInstance==NULL){
		myInstance=new TuioAdapter;
	}
	return myInstance;
}

void TuioAdapter::DestroyInstance(){
	SAFE_DELETE(myInstance);
}

bool TuioAdapter::Init(){
	Logger::Println("[TUIOAdapter] Initialized");
	int port = ApplicationProperty::ReadSetupInt("Tuio","Port",3333);
	tuioClient = new TUIO::TuioClient(port);
	myTuioListener = new MyTuioListener;
	myTuioListener->Init(inputHandler);
	tuioClient->addTuioListener(myTuioListener);
	return true;
}

void TuioAdapter::Close(){
	Logger::Println("[TUIOAdapter] Closed");
	if(tuioClient->isConnected())tuioClient->disconnect();
	tuioClient->removeAllTuioListeners();

	Logger::Println("[TUIOAdapter] Wait Network Disconnect");
	for(;;){
		if(!tuioClient->isConnected()){
			SAFE_DELETE(tuioClient);
			SAFE_DELETE(myTuioListener);
			break;
		}
	}
	Logger::Println("[TUIOAdapter] Disconnected");
}

void TuioAdapter::Run(){
	tuioClient->connect(false);
}

void TuioAdapter::Stop(){
	tuioClient->disconnect();
}