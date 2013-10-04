
#include "MyPackage.h"
#include "Common.h"

#define SAMPLE_RATE 44100

int MyPackage::Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi){
	gapi=_gapi;
	iapi=_iapi;
	aapi=_aapi;

//	img_piano=gapi->CreateImageFromFile("Image\\piano\\piano2.png");
	img_piano=gapi->CreateImageFromFile("Image\\piano\\bigpiano2.png");
	img_touch=gapi->CreateImageFromFile("Image\\piano\\particle0.png");

	char pfName[PF_NUM][256]={
		"c3.wav",
		"c#3.wav",
		"d3.wav",
		"d#3.wav",
		"e3.wav",
		"f3.wav",
		"f#3.wav",
		"g3.wav",
		"g#3.wav",
		"a4.wav",
		"a#4.wav",
		"b4.wav",

		"c4.wav",
		"c#4.wav",
		"d4.wav",
		"d#4.wav",
		"e4.wav",
		"f4.wav",
		"f#4.wav",
		"g4.wav",
		"g#4.wav",
		"a5.wav",
		"a#5.wav",
		"b5.wav",

		"c5.wav",
		"c#5.wav",
		"d5.wav",
		"d#5.wav",
		"e5.wav",
		"f5.wav",
		"f#5.wav",
		"g5.wav",
		"g#5.wav",
		"a6.wav",
		"a#6.wav",
		"b6.wav",

		"c6.wav",
		"c#6.wav",
		"d6.wav",
		"d#6.wav",
		"e6.wav",
		"f6.wav",
		"f#6.wav",
		"g6.wav",
		"g#6.wav",
		"a7.wav",
		"a#7.wav",
		"b7.wav",
		"c7.wav",
		"c7.wav" // dummy sound
	};

	for(int i=0;i<PF_NUM;i++){
		char an[256];
		sprintf_s(an,256,"Audio\\piano\\pf\\%s",pfName[i]);
		sndPiano[i]=aapi->LoadWaveFile(an);
		aapi->SetFadeSpeed(sndPiano[i],0.0001f);
	}

	int pos[PF_NUM][5]={
		{26	,0,53	,127,1},
		{81	,0,107	,127,3},
		{155,0,180	,127,6},
		{202,0,227	,127,8},
		{251,0,276	,127,10},
		{0	,0,46	,209,0},
		{46	,0,89	,209,2},
		{89	,0,131	,209,4},
		{131,0,173	,209,5},
		{173,0,215	,209,7},
		{215,0,258	,209,9},
		{258,0,300	,209,11},

		{326,0,353	,127,13},
		{381,0,407	,127,15},
		{455,0,480	,127,18},
		{502,0,527	,127,20},
		{551,0,576	,127,22},
		{300,0,346	,209,12},
		{346,0,389	,209,14},
		{389,0,431	,209,16},
		{431,0,473	,209,17},
		{473,0,515	,209,19},
		{515,0,558	,209,21},
		{558,0,600	,209,23},
		{600,0,640	,209,24},

		{26	,210,53		,334,25},
		{81	,210,107	,334,27},
		{155,210,180	,334,30},
		{202,210,227	,334,32},
		{251,210,276	,334,34},
		{0	,210,46		,416,24},
		{46	,210,89		,416,26},
		{89	,210,131	,416,28},
		{131,210,173	,416,29},
		{173,210,215	,416,31},
		{215,210,258	,416,33},
		{258,210,300	,416,35},

		{326,210,353	,334,37},
		{381,210,407	,334,39},
		{455,210,480	,334,42},
		{502,210,527	,334,44},
		{551,210,576	,334,46},
		{300,210,346	,416,36},
		{346,210,389	,416,38},
		{389,210,431	,416,40},
		{431,210,473	,416,41},
		{473,210,515	,416,43},
		{515,210,558	,416,45},
		{558,210,600	,416,47},
		{600,210,640	,416,48}
	};
	for(int i=0;i<PF_NUM;i++){
		for(int j=0;j<5;j++){
			posPiano[i][j]=pos[i][j];
		}
	}
	return 0;
}

int MyPackage::Reset(){
	for(int i=0;i<MAX_TOUCH;i++){
		touchPitch[i]=-1;
	}
	for(int i=0;i<PF_NUM;i++){
		pressCount[i]=0;
	}
	return 0;
}
int MyPackage::Close(){
	gapi->ReleaseImage(img_piano);
	for(int i=0;i<PF_NUM;i++){
		aapi->ReleaseWave(sndPiano[i]);
	}
	return 0;
}
int MyPackage::End(){
	return 0;
}
int MyPackage::Run(){
	iapi->GetCursorList(&touch);
	return 0;
}

int MyPackage::GraphicCallback(){
	gapi->DrawImageLU(-464,0,0,0,0,0,0.63f,0.0f,img_piano);
	gapi->DrawImageLU(-464,330,0,0,0,0,0.63f,0.0f,img_piano);
	for(unsigned int i=0;i<touch.size();i++){
		gapi->DrawImage(touch[i].GetX(),touch[i].GetY(),0,0,0,0,0.25f,0.0f,Color4f(255,0,0,255),img_touch);
	}
	return 0;
}

int MyPackage::AudioCallback(float* in,float* out,int frames){
	int i;
	float left,right;
	for(i=0;i<frames;i++){
		left=right=0.0f;
		aapi->GetWaveDataAll(&left,&right);
		*out++ = left*0.2f;
		*out++ = right*0.2f;
	}
	return 0;
}

int MyPackage::InputCallback(TOUCH_EVENT ev,int id,int x,int y){
	int pitch=GetPianoPitch(x,y);
	switch(ev){
	case TOUCH_ON:
		break;
	case TOUCH_CONTINUE:
		if(touchPitch[id]!=pitch){
			if(touchPitch[id]!=-1){
				pressCount[touchPitch[id]]--;
				if(pressCount[touchPitch[id]]==0){
					aapi->SetState(sndPiano[touchPitch[id]],Audio_FadeOut);
				}
			}
			if(pitch!=-1){
				pressCount[pitch]++;
				if(pressCount[pitch]==1){
					aapi->Rewind(sndPiano[pitch]);
					aapi->SetState(sndPiano[pitch],Audio_Play);
//					aapi->SetState(sndPiano[pitch],Audio_FadeIn);
				}
			}
			touchPitch[id]=pitch;
		}
		break;
	case TOUCH_OFF:
		if(touchPitch[id]!=-1){
			pressCount[touchPitch[id]]--;
			if(pressCount[touchPitch[id]]==0){
				aapi->SetState(sndPiano[touchPitch[id]],Audio_FadeOut);
			}
		}
		touchPitch[id]=-1;
		break;
	}
	return 0;
}

int MyPackage::GetPianoPitch(int x,int y){
	for(int i=0;i<PF_NUM;i++){
		if(posPiano[i][0]<=x && x<=posPiano[i][2] && posPiano[i][1]<=y && y<=posPiano[i][3]){
			return posPiano[i][4];
		}
	}
	return -1;
}