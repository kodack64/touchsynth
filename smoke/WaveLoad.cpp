#include "waveload.h"


string WaveLoader::GetOpenWaveFileName(){
	static char filename_full[MAX_PATH];
	static char filename[MAX_PATH];
	string str;
	sprintf_s(filename_full,sizeof(filename_full),"");
	sprintf_s(filename,sizeof(filename),"");
	char curdir[MAX_PATH];

	GetCurrentDirectory(MAX_PATH,curdir);

	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof(ofn) );
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = ("wave(*.wav)\0*.wav\0");
	ofn.lpstrFile = filename_full;
	ofn.lpstrFileTitle = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;
	ofn.lpstrTitle = ("ファイルを開く");
	ofn.lpstrDefExt = ("wav");

	if( !GetOpenFileName( &ofn ) ){
		cout << "File is not selected" << cout;
		str="";
	}else{
		str = filename_full;
	}
	SetCurrentDirectory(curdir);
	return str;
}
string WaveLoader::GetSaveWaveFileName(){
	static char filename_full[MAX_PATH];
	static char filename[MAX_PATH];
	string str;
	sprintf_s(filename_full,sizeof(filename_full),"");
	sprintf_s(filename,sizeof(filename),"");
	char curdir[MAX_PATH];

	GetCurrentDirectory(MAX_PATH,curdir);

	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof(ofn) );
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = ("wave(*.wav)\0*.wav\0");
	ofn.lpstrFile = filename_full;
	ofn.lpstrFileTitle = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = ("waveファイルを保存");
	ofn.lpstrDefExt = ("wav");

	if( !GetSaveFileName( &ofn ) ){
		cout << "File is not selected" << cout;
		str="";
	}else{
		str = filename_full;
	}
	SetCurrentDirectory(curdir);
	return str;
}

int WaveLoader::LoadWaveFile(string filepath,WaveShell* wsl,WaveShell* wsr){
	FILE* wlfp;
	BYTE id[4];
	unsigned long file_size,fmt_size,sample_rate,avg_bytes_sec,data_size;
	unsigned short tag,channels,block_align,bits_per_sample;
	fopen_s(&wlfp,filepath.c_str(),"rb");
	if(wlfp==NULL){
		cout << "Load Error : File not found" << endl;
		return -1;
	}
	fread(id,sizeof(BYTE),4,wlfp);
	if(memcmp(id,"RIFF",4)!=0){
		cout << "Load Error : No RIFF ID" << endl;
		fclose(wlfp);
		return -1;
	}
	fread(&file_size,sizeof(DWORD),1,wlfp);
	fread(id,sizeof(BYTE),4,wlfp);
	if(memcmp(id,"WAVE",4)!=0){
		cout << "Load Error : No WAVE ID" << endl;
		fclose(wlfp);
		return -1;
	}
	fread(id,sizeof(BYTE),4,wlfp);
	if(memcmp(id,"fmt ",4)!=0){
		cout << "Load Error : No fmt ID" << endl;
		fclose(wlfp);
		return -1;
	}

    fread(&fmt_size, sizeof(DWORD),1,wlfp);
    fread(&tag, sizeof(short), 1, wlfp);
	if(tag!=0x0001){
		cout << "Load Error : Not PCM WAVE" << endl;
		fclose(wlfp);
		return -1;
	}
    fread(&channels, sizeof(short),1,wlfp);
    fread(&sample_rate, sizeof(DWORD), 1, wlfp);
    fread(&avg_bytes_sec, sizeof(DWORD), 1, wlfp);
    fread(&block_align, sizeof(short), 1, wlfp);
    fread(&bits_per_sample, sizeof(short), 1, wlfp); 

	fread(id,sizeof(BYTE),4,wlfp);
	if(memcmp(id,"data",4)!=0){
		cout << "Load Error : No data ID" << endl;
		fclose(wlfp);
		return -1;
	}
	fread(&data_size, sizeof(DWORD), 1, wlfp); 

	printf("Loader : File info:\n");
	printf("	Channel		:%d\n",channels);
	printf("	SampleRate	:%d\n",sample_rate);
	printf("	BitPerSample	:%d\n",bits_per_sample);
	printf("	DataSize	:%d\n",data_size);
	if(sample_rate!=44100){
		cout << "Load Error : Unsupported Sample Rate (supported only 44100)" << endl;
		fclose(wlfp);
		return -1;
	}
	if(bits_per_sample!=8 && bits_per_sample!=16){
		cout << "Load Error : Unsupported bits per sample (supported only 8 or 16)" << endl;
		fclose(wlfp);
		return -1;
	}

	wsl->Resize(data_size/channels/(bits_per_sample/8));
	wsr->Resize(data_size/channels/(bits_per_sample/8));
	wsl->SetCursor(0);
	wsr->SetCursor(0);

	unsigned long i,j;
	short spos;
	float pos;
	unsigned char a,b;
	for(i=0;i<data_size;){
		for(j=0;j<(unsigned)channels;j++){
			if(bits_per_sample==8){
				spos=GetNext(wlfp)-128;
				pos=spos/128.0f;
				i++;
			}else if(bits_per_sample==16){
				a=GetNext(wlfp);
				b=GetNext(wlfp);
				spos=(short)(a+b*256);
				pos=spos/32768.0f;
				i+=2;
			}
			if(channels==1){
				wsl->SetCursorPos(pos);
				wsr->SetCursorPos(pos);
				wsl->Forward();
				wsr->Forward();
			}else{
				if(j==0){
					wsl->SetCursorPos(pos);
					wsl->Forward();
				}else{
					wsr->SetCursorPos(pos);
					wsr->Forward();
				}
			}
		}
	}
	cout << "Loader : File Loaded" << endl;
	fclose(wlfp);
	return 0;
}

int WaveLoader::WriteWaveFile(string filepath,WaveShell* wsl,WaveShell* wsr){
	FILE* wwfp;
	BYTE id[4];
	unsigned long file_size,fmt_size,sample_rate,avg_bytes_sec,data_size,header_size;
	unsigned short tag,channels,block_align,bits_per_sample;

	sample_rate=SAMPLE_RATE;
	channels=2;
	bits_per_sample=16;
	block_align=channels*bits_per_sample/8;
	avg_bytes_sec=block_align*sample_rate;
	data_size=block_align*min(wsl->GetLength(),wsr->GetLength());
	header_size=44;
	file_size=data_size+header_size-8;
	fmt_size=16;
	tag=1;

	fopen_s(&wwfp,filepath.c_str(),"wb");
	if(wwfp==NULL)return -1;

	/* riff */
	memcpy(id,"RIFF",4);
	BYTE id2[4];
	id2[0]='R';
	id2[1]='I';
	id2[2]='F';
	id2[3]='F';
	fwrite(id,1,4,wwfp);
	fwrite(&file_size, sizeof(file_size), 1, wwfp);
	memcpy(id,"WAVE",4);
	fwrite(id,1,4,wwfp);

	/* fmt */
	memcpy(id,"fmt ",4);
	fwrite(id,1,4,wwfp);
	fwrite(&fmt_size, sizeof(fmt_size), 1, wwfp);
	fwrite(&tag, sizeof(tag), 1, wwfp);
	fwrite(&channels, sizeof(channels), 1, wwfp);
	fwrite(&sample_rate, sizeof(sample_rate), 1, wwfp);
	fwrite(&avg_bytes_sec, sizeof(avg_bytes_sec), 1, wwfp);
	fwrite(&block_align, sizeof(block_align), 1, wwfp);
	fwrite(&bits_per_sample, sizeof(bits_per_sample), 1, wwfp);

	/* data */
	memcpy(id,"data",4);
	fwrite(id,1,4,wwfp);
	fwrite(&data_size, sizeof(data_size), 1, wwfp);

	unsigned long n,sn;
	float fs;
	short ss;
	for (n=sn=0;n<data_size;sn++)
	{
		fs=wsl->GetPos(sn)*32768.0f;
		if (fs<-32768.0f)	fs=-32768.0f;
		if (fs>32767.0f)	fs=32767.0f;
		ss = (short)(fs+0.5);
		fwrite(&ss,sizeof(ss),1,wwfp);
		n+=2;

		fs=wsr->GetPos(sn)*32768.0f;
		if (fs<-32768.0f)	fs=-32768.0f;
		if (fs>32767.0f)	fs=32767.0f;
		ss = (short)(fs+0.5);
		fwrite(&ss,sizeof(ss),1,wwfp);
		n+=2;
	}
  	fclose(wwfp);
	cout << "Writer : File Saved" << endl;
	return 0;
}

unsigned char WaveLoader::GetNext(FILE* myfp){
	unsigned char buf;
	fread(&buf,sizeof(unsigned char),1,myfp);
	return buf;
}