
#include "SerialAdapter.h"
#include "Logger.h"
#include "ApplicationProperty.h"

SerialAdapter* SerialAdapter::myInstance=NULL;

SerialAdapter* SerialAdapter::CreateInstance(){
	if(myInstance!=NULL)return myInstance;
	else{
		myInstance=new SerialAdapter;
		return myInstance;
	}
}
void SerialAdapter::DestroyInstance(){
	SAFE_DELETE(myInstance);
}

bool SerialAdapter::Init()
{
	f_init=true;
	f_loop=false;
	f_start=false;

	Logger::Println("[SerialPort]	Initialize");

	string mySection="Serial";
	string str = ApplicationProperty::ReadSetupString(mySection.c_str(),"ComName","COM3");
	int fps = ApplicationProperty::ReadSetupInt(mySection.c_str(),"Fps",60);
	strcpy_s(comName,"COM3");
	ncomcfg=sizeof(comcfg);

	hPort = CreateFile(
		comName,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	if(hPort == INVALID_HANDLE_VALUE){
		Logger::Println("[SerialPort]	Error : CreateFile Fail");
		goto error;
	}

	if(!SetupComm(hPort,1024,1024)){
		Logger::Println("[SerialPort]	Error : SetupComm Fail");
		goto error;
	}

	if(!PurgeComm(hPort,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR)){
		Logger::Println("[SerialPort]	Error : PurgeComm Fail");
		goto error;
	}

	GetCommConfig(hPort,&comcfg,&ncomcfg);
	if(!SetCommConfig(hPort, &comcfg,ncomcfg))
	{
		Logger::Println("[SerialPort]	Error : SetCommState Fail");
		goto error;
	}
	
	COMMTIMEOUTS timeout;
	SetupTimeout(&timeout);
	if(!SetCommTimeouts(hPort, &timeout)){
		Logger::Println("[SerialPort]	Error : SetCommTimeouts Fail");
		goto error;
	}

	unsigned int id;
	myHandle=(HANDLE)_beginthreadex(NULL,0,SerialAdapter::Launch,this,0,&id);
	if(myHandle==NULL){
		Logger::Println("[SerialPort]	Error : Thread Error");
		goto error;
	}
	return true;

error:
	f_init=false;
	return false;
}

void SerialAdapter::Loop(){
	f_loop=true;
	for(;f_loop&&f_init;){
		if(!f_start){
		}else{
			DWORD dwError, dwRead;
			COMSTAT ComStat;
			char buf[1024];

			ClearCommError(hPort,&dwError,&ComStat);
			if(ComStat.cbInQue){
				if(ReadFile(hPort,buf,ComStat.cbInQue,&dwRead,NULL )){
					buf[dwRead]='\0';
				}
				else{
					Logger::Println("[SerialPort]	Error : ReadFile Fail" ) ;
					f_loop=false;
				}
			}
		}
		Sleep(1000/fps);
	}
}
unsigned int SerialAdapter::Launch(void* arg){
	((SerialAdapter*)arg)->Loop();
	return 0;
}
void SerialAdapter::Close(void){
	if(f_loop){
		Logger::Println("[SerialPort]	Killing Serial Port Thread...");
		f_loop=false;
		WaitForSingleObject(myHandle,INFINITE);
		Logger::Println("[SerialPort]	Serial Port Thread End");
		CloseHandle(myHandle);
	}
	if(hPort!=INVALID_HANDLE_VALUE){
		CloseHandle(hPort);
		hPort=NULL;
	}
}
void SerialAdapter::Run(){
	f_start=true;
}
void SerialAdapter::Stop(){
	f_start=false;
}

void SerialAdapter::SetupTimeout(COMMTIMEOUTS *timeout){
	timeout->ReadIntervalTimeout = 500; // �����Ǎ����̂Q�����Ԃ̑S�̑҂����ԁimsec�j
                                           
	timeout->ReadTotalTimeoutMultiplier = 0; //�Ǎ��̂P����������̎���
	timeout->ReadTotalTimeoutConstant = 500; //�Ǎ��G���[���o�p�̃^�C���A�E�g����
		 //(��M�g�[�^���^�C���A�E�g) = ReadTotalTimeoutMultiplier * (��M�\��o�C�g��) + ReadTotalTimeoutConstant

	timeout->WriteTotalTimeoutMultiplier = 0; //�������݂P����������̑҂�����
	timeout->WriteTotalTimeoutConstant = 500;//�������݃G���[���o�p�̃^�C���A�E�g����
		 //(���M�g�[�^���^�C���A�E�g) = WriteTotalTimeoutMultiplier * (���M�\��o�C�g��) + WriteTotalTimeoutConstant
}
