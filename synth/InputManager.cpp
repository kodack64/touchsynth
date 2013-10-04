
#include "InputManager.h"
#include "MidiAdapter.h"
#include "MouseAdapter.h"
#include "OpenCVAdapter.h"
#include "TuioAdapter.h"
#include "SerialAdapter.h"
#include "KeyboardAdapter.h"
#include "Logger.h"
#include "ApplicationProperty.h"

#define DEF_USE_MOUSE		1
#define DEF_USE_TUIO		1
#define DEF_USE_KEYBOARD	1
#define DEF_USE_OPENCV		0
#define DEF_USE_MIDI		0
#define DEF_USE_SERIAL		0

InputManager* InputManager::myInstance=NULL;

InputManager::InputManager()
:inputHandler(NULL){
}

InputManager::~InputManager(){
}

InputManager* InputManager::CreateInstance(){
	if(myInstance==NULL){
		myInstance=new InputManager;
	}
	return myInstance;
}
void InputManager::DestroyInstance(){
	SAFE_DELETE(myInstance);
}


bool InputManager::Init(){
	Logger::Println("[InputManager]	Initialize");

	inputHandler=new InputHandler;
	if(!inputHandler->Init()){
		return false;
	}

	int c=0;
	adpMidi=adpMouse=adpOpenCV=adpTuio=adpSerial=adpKeyboard=-1;

	string section="Input";
	if(ApplicationProperty::ReadSetupInt(section.c_str(),"UseMidi",DEF_USE_MIDI)!=0){
		inputList.push_back(MidiAdapter::CreateInstance());
		adpMidi=c++;
		adpMap.insert(map<int,int>::value_type(ADP_MIDI,adpMidi));
	}
	if(ApplicationProperty::ReadSetupInt(section.c_str(),"UseMouse",DEF_USE_MOUSE)!=0){
		inputList.push_back(MouseAdapter::CreateInstance());
		adpMouse=c++;
		adpMap.insert(map<int,int>::value_type(ADP_MOUSE,adpMouse));
	}
	if(ApplicationProperty::ReadSetupInt(section.c_str(),"UseOpenCV",DEF_USE_OPENCV)!=0){
		inputList.push_back(OpenCVAdapter::CreateInstance());
		adpOpenCV=c++;
		adpMap.insert(map<int,int>::value_type(ADP_OPENCV,adpOpenCV));
	}
	if(ApplicationProperty::ReadSetupInt(section.c_str(),"UseTuio",DEF_USE_TUIO)!=0){
		inputList.push_back(TuioAdapter::CreateInstance());
		adpTuio=c++;
		adpMap.insert(map<int,int>::value_type(ADP_TUIO,adpTuio));
	}
	if(ApplicationProperty::ReadSetupInt(section.c_str(),"UseSerial",DEF_USE_SERIAL)!=0){
		inputList.push_back(SerialAdapter::CreateInstance());
		adpSerial=c++;
		adpMap.insert(map<int,int>::value_type(ADP_SERIAL,adpSerial));
	}
	if(ApplicationProperty::ReadSetupInt(section.c_str(),"UseKeyboard",DEF_USE_KEYBOARD)!=0){
		inputList.push_back(KeyboardAdapter::CreateInstance());
		adpKeyboard=c++;
		adpMap.insert(map<int,int>::value_type(ADP_KEYBOARD,adpKeyboard));
	}

	bool flag=true;
	for(unsigned int i=0;i<inputList.size();i++){
		inputList[i]->SetInputHandler(inputHandler);
		if(!inputList[i]->Init()){
			flag=false;
		}
	}
	return flag;
}

void InputManager::Close(){
	Logger::Println("[InputManager]	Close");
	for(unsigned int i=0;i<inputList.size();i++){
		if(inputList[i]!=NULL)inputList[i]->Close();
	}
	inputHandler->Close();

	if(adpMidi>=0)MidiAdapter::DestroyInstance();
	if(adpMouse>=0)MouseAdapter::DestroyInstance();
	if(adpOpenCV>=0)OpenCVAdapter::DestroyInstance();
	if(adpTuio>=0)TuioAdapter::DestroyInstance();
	if(adpKeyboard>=0)KeyboardAdapter::DestroyInstance();
	if(adpSerial>=0)SerialAdapter::DestroyInstance();
	SAFE_DELETE(inputHandler);
}

void InputManager::Run(){
	Logger::Println("[InputManager]	Run");
	for(unsigned int i=0;i<inputList.size();i++){
		if(inputList[i]!=NULL)inputList[i]->Run();
	}
}

void InputManager::SetAdapterState(AdapterKind ak,bool flag){
	if(flag)	inputList[adpMap[ak]]->Run();
	else		inputList[adpMap[ak]]->Stop();
}

InputApi* InputManager::GetAPI(){
	return inputHandler;
}