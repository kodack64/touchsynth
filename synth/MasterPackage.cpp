
#include "MasterPackage.h"
#include "Common.h"
#include "Logger.h"

#define SAMPLE_RATE 44100

MasterPackage* MasterPackage::myInstance=NULL;

int MasterPackage::Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi){
	Logger::Println("[MasterPackage]	Initialize");
	gapi=_gapi;
	iapi=_iapi;
	aapi=_aapi;

	img_point=gapi->CreateImageFromFile("Image\\master\\particle0.png");
	img_icon_piano=gapi->CreateImageFromFile("Image\\master\\piano.jpg");
	img_icon_map=gapi->CreateImageFromFile("Image\\master\\map.jpg");
	img_icon_smoke=gapi->CreateImageFromFile("Image\\master\\smoke.jpg");
	img_icon_game=gapi->CreateImageFromFile("Image\\master\\game.jpg");
	hMutex=CreateMutex(NULL,false,NULL);
	Reset();
	return 0;
}

int MasterPackage::Reset(){
	memset(activeCount,0,sizeof(activeCount));
	choice=0;
	return 0;
}
int MasterPackage::Close(){
	Logger::Println("[MasterPackage]	Close");
	gapi->ReleaseImage(img_point);
	gapi->ReleaseImage(img_icon_piano);
	gapi->ReleaseImage(img_icon_map);
	gapi->ReleaseImage(img_icon_smoke);
	gapi->ReleaseImage(img_icon_game);
	CloseHandle(hMutex);
	return 0;
}
int MasterPackage::Run(){
	return choice;
}

int MasterPackage::GraphicCallback(){
	TouchCursor tc[MAX_TOUCH];
	int num;
	iapi->GetCursorList(tc,&num);
	for(int i=0;i<MAX_TOUCH;i++){
		if(tc[i].IsActive()){
			gapi->DrawImage(tc[i].GetX(),tc[i].GetY(),0,0,0,0,0.25f,0.0f,Color4f(255,0,0,255),img_point);
		}
	}
/*	gapi->DrawImageLU(100,0,0,0,0,0,240/128.0f,0.0f,img_icon_map);
	gapi->DrawImageLU((int)(320*128.0f/240),0,0,0,0,0,240/128.0f,0.0f,img_icon_piano);
	gapi->DrawImageLU((int)(320*128.0f/240),(int)(240*128.0f/240),0,0,0,0,240/128.0f,0.0f,img_icon_smoke);
	gapi->DrawImageLU(100,(int)(240*128.0f/240),0,0,0,0,240/128.0f,0.0f,img_icon_game);*/
	gapi->DrawImageLU(100,60,0,0,0,0,1.0f,0.0f,img_icon_game);
	gapi->DrawImageLU(400,60,0,0,0,0,1.0f,0.0f,img_icon_piano);
	gapi->DrawImageLU(100,300,0,0,0,0,1.0f,0.0f,img_icon_map);
	gapi->DrawImageLU(400,300,0,0,0,0,1.0f,0.0f,img_icon_smoke);
	return 0;
}

int MasterPackage::InputCallback(TOUCH_EVENT ev,int id,int x,int y){
	int c=GetChoiceFromVector(x,y);
	switch(ev){
	case TOUCH_ON:
		touch[id]=c;
		activeCount[c]++;
		break;
	case TOUCH_CONTINUE:
		if(touch[id]!=c){
			activeCount[touch[id]]--;
			activeCount[c]++;
			touch[id]=c;
		}
		break;
	case TOUCH_OFF:
		if(touch[id]!=0)choice=touch[id];
		activeCount[touch[id]]--;
		touch.erase(id);
		break;
	}
	return 0;
}
int MasterPackage::GetChoiceFromVector(int x,int y){
	if(x<320 && y<240)			return 1;
	else if(x<320 && y>=240)	return 2;
	else if(x>=320 && y<240)	return 3;
	else if(x>=320 && y>=240)	return 4;
	else return 0;
}


int MasterPackage::AudioCallback(float* in,float* out,int frames){
	int i;
	float left,right;
	for(i=0;i<frames;i++){
		left=right=0.0f;
		aapi->GetWaveDataAll(&left,&right);
		*out++ = left;
		*out++ = right;
	}
	return 0;
}
int MasterPackage::End(){
	return 0;
}