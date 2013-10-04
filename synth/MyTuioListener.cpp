
#include "MyTuioListener.h"
#include "Logger.h"
#include "ApplicationProperty.h"

using namespace TUIO;

MyTuioListener::MyTuioListener(){
	inputHandler=NULL;
}
MyTuioListener::~MyTuioListener(){
}
void MyTuioListener::addTuioObject(TuioObject* tCur){
	if(inputHandler!=NULL){
		int x=(int)(winx-winx*tCur->getX());
		int y=(int)(winy-winy*tCur->getY());
		int given=inputHandler->CreatePosition(x,y);
		idMap.insert(map<int,int>::value_type(tCur->getSessionID(),given));
	}
}
void MyTuioListener::updateTuioObject(TuioObject* tCur){
	if(inputHandler!=NULL){
		int x=(int)(winx-winx*tCur->getX());
		int y=(int)(winy-winy*tCur->getY());
		inputHandler->UpdatePosition(idMap[tCur->getSessionID()],x,y,tCur->getMotionSpeed(),tCur->getMotionAccel(),tCur->getTuioTime().getMicroseconds());
	}
}
void MyTuioListener::removeTuioObject(TuioObject* tCur){
	if(inputHandler!=NULL){
		inputHandler->DestroyPosition(idMap[tCur->getSessionID()]);
		idMap.erase(tCur->getSessionID());
	}
}

void MyTuioListener::addTuioCursor(TuioCursor* tCur){
//	Logger::Println("Added %d",tCur->getCursorID());
	if(inputHandler!=NULL){
		int x=(int)(winx*tCur->getX());
		int y=(int)(winy*tCur->getY());
		int given=inputHandler->CreatePosition(x,y);
		idMap.insert(map<int,int>::value_type(tCur->getCursorID(),given));
	}
}
void MyTuioListener::updateTuioCursor(TuioCursor* tCur){
	if(inputHandler!=NULL){
		int x=(int)(winx*tCur->getX());
		int y=(int)(winy*tCur->getY());
		inputHandler->UpdatePosition(idMap[tCur->getCursorID()],x,y,tCur->getMotionSpeed(),tCur->getMotionAccel(),tCur->getTuioTime().getMicroseconds());
	}
}
void MyTuioListener::removeTuioCursor(TuioCursor* tCur){
	if(inputHandler!=NULL){
		inputHandler->DestroyPosition(idMap[tCur->getCursorID()]);
		idMap.erase(tCur->getCursorID());
	}
}
void MyTuioListener::Init(InputHandler* input){
	inputHandler=input;
	winx = ApplicationProperty::ReadSetupInt("Graphic","WindowSizeX",640);
	winy = ApplicationProperty::ReadSetupInt("Graphic","WindowSizeY",640);
}