
#include "Common.h"

#define MAX_EQ 10

class WavePos{
private:
	float x;
	float y;
public:
	WavePos(){x=0.0f,y=0.0f;};
};

struct EQSetRequest{
	int id;
	float Q;
	float gain;
	float freq;
};

class Equalizer{
private:
	int num;
	int sampleRate;
	float EQ_A[MAX_EQ][3];
	float EQ_B[MAX_EQ][3];
	float EQ_ORG[3][2];
	float EQ_ARR[MAX_EQ][3][2];
	bool enableLowShelving;
	bool enableHighShelving;
	long count;
	HANDLE hMutex;
	float eqc;
	int qecc;
public:
	virtual int Init(int num,int sampleRate);
	virtual int Close();
	virtual int SetLowShelving(bool flag);
	virtual int SetHighShelving(bool flag);
	virtual int SetParam(int num,float freq,float Q,float gain);
	virtual int Run(float* left,float* right);
	virtual int SetDefault(int num);
};