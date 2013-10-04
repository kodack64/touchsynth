#include "recorder.h"

Recorder::Recorder(){
}

Recorder::~Recorder(){
	SAFE_DELETE(wsl);
	SAFE_DELETE(wsr);
	SAFE_DELETE(wl);
}

int Recorder::Init(){
	wsl = new WaveShell;
	wsr = new WaveShell;
	wl = new WaveLoader;
	Reset();
	return 0;
}

int Recorder::Close(){
	SAFE_DELETE(wsl);
	SAFE_DELETE(wsr);
	SAFE_DELETE(wl);
	return 0;
}
int Recorder::Reset(){
	wsl->Init();
	wsr->Init();
	recflag=false;
	playflag=false;
	playcount=0;
	reccount=0;
	return 0;
}

int Recorder::Start(){
	if(recflag){
		cout << "Record	Error : Now Recording" <<endl;
		return -1;
	}
	if(playflag){
		cout << "Record	Error : Play Recording Now" << endl;
		return -1;
	}

	string fname;
	wsl->Clear();
	wsr->Clear();

	recflag=true;
	reccount=0;
	cout << "Recording : Start" <<endl;

	return 0;
}

int Recorder::Next(float left,float right){
	if(recflag){
		wsl->Add(left);
		wsr->Add(right);
		reccount++;
	}
	return 0;
}

int Recorder::Stop(){
	if(recflag==false){
		cout << "Recording Error : Not Recording Now" <<endl;
		return -1;
	}else{
		cout << "Recording : Stop Recording" << endl;
		printf("%ld Samples = %.2f Second Saved\n",reccount,((float)reccount/SAMPLE_RATE));
		recflag=false;
	}
	return 0;
}

int Recorder::Save(){
	if(recflag){
		cout << "Recording Error : Recording Now" << endl;
		return -1;
	}
	string str = wl->GetSaveWaveFileName();
	str="out.wav";
	if(str!=""){
		wl->WriteWaveFile(str,wsl,wsr);
		cout << "Recording : Wave Saved"<< endl;
	}
	return 0;
}

int Recorder::Play(){
	if(recflag){
		cout << "record now\n";
		return -1;
	}
	cout << "play record\n";
	playflag=true;
	playcount=0;
	return 0;
}
void Recorder::Make(){
	if(playflag){
		left=wsl->GetPos(playcount);
		right=wsr->GetPos(playcount);
		playcount++;
		if(min(wsl->GetLength(),wsr->GetLength())<=playcount){
			cout << "record reaches end\n";
			playflag=false;
		}
	}else{
		left=right=0.0f;
	}
}