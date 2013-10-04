
#include "Equalizer.h"
#include "iir_filter.h"

int Equalizer::Init(int _num,int sr){
	num=_num;
	sampleRate=sr;
	enableLowShelving=false;
	enableHighShelving=false;
	memset(EQ_A,0,sizeof(EQ_A));
	memset(EQ_B,0,sizeof(EQ_B));
	for(int i=0;i<num;i++){
		EQ_A[i][0]=1.0f;
		EQ_A[i][1]=0.0f;
		EQ_A[i][2]=0.0f;
		EQ_B[i][0]=1.0f;
		EQ_B[i][1]=0.0f;
		EQ_B[i][2]=0.0f;
	}
	memset(EQ_ORG,0,sizeof(EQ_ORG));
	memset(EQ_ARR,0,sizeof(EQ_ARR));
	hMutex=CreateMutex(NULL,false,NULL);
	eqc=0.0f;
	qecc=0;
	return 0;
}
int Equalizer::SetDefault(int i){
	EQ_A[i][0]=1.0f;
	EQ_A[i][1]=0.0f;
	EQ_A[i][2]=0.0f;
	EQ_B[i][0]=1.0f;
	EQ_B[i][1]=0.0f;
	EQ_B[i][2]=0.0f;
	for(int m=0;m<=2;m++){
		EQ_ARR[i][m][0]=0.0f;
		EQ_ARR[i][m][1]=0.0f;
	}
	return 0;
}

int Equalizer::Close(){
	CloseHandle(hMutex);
	return 0;
}

int Equalizer::SetLowShelving(bool flag){
	enableLowShelving=flag;
	return 0;
}
int Equalizer::SetHighShelving(bool flag){
	enableHighShelving=flag;
	return 0;
}
int Equalizer::SetParam(int _id,float _freq,float _Q,float _gain){
	WaitForSingleObject(hMutex,INFINITE);
	if(_freq<100)_freq=100;
	if(_freq>16000)_freq=16000;
	if(_gain<-1.0)_gain=-1.0;
	IIR_peaking(_freq/SAMPLE_RATE,_Q,_gain,EQ_A[_id],EQ_B[_id]);
	ReleaseMutex(hMutex);
	return 0;
}

int Equalizer::Run(float* left,float* right){
	int i,m;
	const int I=2;
	const int J=2;
	for(i=0;i<J;i++){
		EQ_ORG[i][0]=EQ_ORG[i+1][0];
		EQ_ORG[i][1]=EQ_ORG[i+1][1];
	}
	EQ_ORG[J][0]=(*left);
	EQ_ORG[J][1]=(*right);

	WaitForSingleObject(hMutex,INFINITE);
	for (i = 0; i < num; i++)
	{
		for(m=0;m<J;m++){
			EQ_ARR[i][m][0]=EQ_ARR[i][m+1][0];
			EQ_ARR[i][m][1]=EQ_ARR[i][m+1][1];
		}
		EQ_ARR[i][J][0] = 0.0f;
		EQ_ARR[i][J][1] = 0.0f;

		for (m = 0; m <= J; m++)
		{
			if(i==0){
				EQ_ARR[i][J][0] += EQ_B[i][m] * EQ_ORG[J-m][0];
				EQ_ARR[i][J][1] += EQ_B[i][m] * EQ_ORG[J-m][1];
			}else{
				EQ_ARR[i][J][0] += EQ_B[i][m] * EQ_ARR[i-1][J-m][0];
				EQ_ARR[i][J][1] += EQ_B[i][m] * EQ_ARR[i-1][J-m][1];
			}
		}
		for (m = 1; m <= I; m++)
		{
			EQ_ARR[i][J][0] += -EQ_A[i][m] * EQ_ARR[i][J-m][0];
			EQ_ARR[i][J][1] += -EQ_A[i][m] * EQ_ARR[i][J-m][1];
		}
	}
	ReleaseMutex(hMutex);

	if(num!=0){
		(*left)=EQ_ARR[num-1][J][0];
		(*right)=EQ_ARR[num-1][J][1];
		if((*left)>=1.0f){
			(*left)=1.0f;
		}
		if((*left)<=-1.0f){
			(*left)=-1.0f;
		}
		if((*right)>=1.0f){
			(*right)=1.0f;
		}
		if((*right)<=-1.0f){
			(*right)=-1.0f;
		}
	}
	return 0;
}