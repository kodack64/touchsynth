
#include "MyPackage.h"
#include "Common.h"

#define SAMPLE_RATE 44100

void MyPackage::Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi){
	gapi=_gapi;
	iapi=_iapi;
	aapi=_aapi;

	img1=gapi->CreateImageFromFile("Image\\particle0.png");
	img2=gapi->CreateImageFromFile("Image\\particle1.png");
	img3=gapi->CreateImageFromFile("Image\\particle2.png",Color4f(255,0,0,255));	
	snd1=aapi->LoadWaveFile("Audio\\SampleLoop.wav");
	aapi->SetState(snd1,Audio_LoopPlay);
}
void MyPackage::Close(){
	gapi->ReleaseImage(img1);
	gapi->ReleaseImage(img2);
	gapi->ReleaseImage(img3);
	aapi->ReleaseWave(snd1);
}
void MyPackage::Run(){
}

void MyPackage::GraphicCallback(){
	vector <TouchCursor> cp;
	int i;

	iapi->GetCursorList(&cp);

	for(i=0;i<(signed)cp.size();i++){
		int id=cp[i].GetId();
		Color4f c(255*((id%2)+1)/2,255*((id%3)+1)/3,255*((id%5)+1)/5,255);
		gapi->DrawImage(cp[i].GetX(),cp[i].GetY(),img1);
	}
	cp.clear();
}

void MyPackage::AudioCallback(float* in,float* out,int frames){
	int i;
	float left,right;
	for(i=0;i<frames;i++){
		left=right=0.0f;
		aapi->GetWaveDataAll(&left,&right);
		*out++ = left;
		*out++ = right;
	}
}

void MyPackage::InputCallback(TOUCH_EVENT ev,int id,int x,int y){
	switch(ev){
	case TOUCH_ON:
		break;
	case TOUCH_OFF:
		break;
	case TOUCH_CONTINUE:
		break;
	}
}