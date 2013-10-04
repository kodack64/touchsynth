
#include "MidiAdapter.h"
#include "Logger.h"
#include "ApplicationProperty.h"

MidiAdapter* MidiAdapter::myInstance=NULL;

MidiAdapter* MidiAdapter::CreateInstance(){
	if(myInstance!=NULL)return myInstance;
	else{
		myInstance=new MidiAdapter;
		return myInstance;
	}
}
void MidiAdapter::DestroyInstance(){
	SAFE_DELETE(myInstance);
}

void MidiAdapter::SetInputHandler(InputHandler* s){
	inputHandler=s;
}


bool MidiAdapter::Init(){
	Logger::Println("[MidiAdapter]	Initialize");
	outDevCount=0;
	inDevCount=0;
	midiIn=NULL;
	midiOut=NULL;
	memset(&midiInCaps,0,sizeof(MIDIINCAPS));
	memset(&midiOutCaps,0,sizeof(MIDIOUTCAPS));

	f_open=false;
	f_start=false;

	string mySection = "Midi";
	inDevNum=ApplicationProperty::ReadSetupInt(mySection.c_str(),"InputDeviceNumber",0);
	outDevNum=ApplicationProperty::ReadSetupInt(mySection.c_str(),"OutputDeviceNumber",0);
	inDevCount=midiInGetNumDevs();
	outDevCount=midiOutGetNumDevs();

	if(ApplicationProperty::ReadSetupInt(mySection.c_str(),"UseSetup",1)==1){
		int i;
		cout << "[MidiAdapter] "<< inDevCount << " device(s) found " << endl;
		for(i=0;i<inDevCount;i++){
			midiInGetDevCaps(i,&midiInCaps,sizeof(midiInCaps));
			cout << "	[" << i << "] " << midiInCaps.szPname << endl;
		}
		cout << "	[" << i << "] " << "No Midi Input" << endl;
		while(1){
			cout << "chose Midi Input Device > ";
			cin >> inDevNum;
			if(0<=inDevNum && inDevNum<=inDevCount){
				break;
			}else{
				cout << "invalid number" << endl;
			}
		}

		cout << "[MidiAdapter] "<< outDevCount << " device(s) found " << endl;
		for(i=0;i<outDevCount;i++){
			midiOutGetDevCaps(i,&midiOutCaps,sizeof(midiOutCaps));
			cout << "	[" << i << "] " << midiOutCaps.szPname << endl;
		}
		cout << "	[" << i << "] " << "No Midi Output" << endl;
		while(1){
			cout << "chose Midi Output Device > ";
			cin >> outDevNum;
			if(0<=outDevNum && outDevNum<=outDevCount){
				break;
			}else{
				cout << "invalid number" << endl;
			}
		}
	}


	if(inDevNum>=inDevCount || inDevNum<0)return true;

	Logger::Println("[MidiAdapter]	Open Stream");
	MMRESULT res;
	res=midiInOpen(&midiIn,inDevNum,(DWORD)(MidiInProc),(DWORD_PTR)this,CALLBACK_FUNCTION);
	switch(res) {
	case MMSYSERR_NOERROR:
		f_open=true;
		break;
	case MMSYSERR_ALLOCATED:
		printf("[MidiAdapter] Error : 指定されたリソースは既に割り当てられています\n");
		break;
	case MMSYSERR_BADDEVICEID:
		printf("[MidiAdapter] Error : 指定されたデバイス識別子 %d は範囲外です。\n", inDevNum);
		break;
	case MMSYSERR_INVALFLAG:
		printf("[MidiAdapter] Error : dwFlags パラメータで指定されたフラグは無効です。\n");
		break;
	case MMSYSERR_INVALPARAM:
		printf("[MidiAdapter] Error : 指定されたポインタまたは構造体は無効です。\n");
		break;
	case MMSYSERR_NOMEM:
		printf("[MidiAdapter] Error : システムはメモリを割り当てられないか、またはロックできません。\n");
		break;
	default:
		printf("[MidiAdapter] Error : 不明なエラーです。\n");
		break;
	}
	return f_open;
}

void MidiAdapter::Run(){
	MMRESULT res;
	if(f_open=true){
		if((res=midiInStart(midiIn)) != MMSYSERR_NOERROR) {
			if(res == MMSYSERR_INVALHANDLE)
				Logger::Println("[MidiAdapter] Error : 指定されたデバイスハンドルは無効です。\n");
			else
				Logger::Println("[MidiAdapter] Error : 不明なエラーです。\n");
			midiInClose(midiIn);
			return;
		}else{
			f_start=true;
		}
	}
	return;
}
void MidiAdapter::Close(){
	Logger::Println("[MidiAdapter]	Close");
	if(f_start){
		midiInStop(midiIn);
		f_start=false;
	}
	if(f_open){
		midiInClose(midiIn);
		f_open=false;
	}
	return;
}
void MidiAdapter::Stop(){
	MMRESULT res;
	res = midiInStop(midiIn);
	f_start=false;
	if(res!=MMSYSERR_NOERROR){
		Logger::Println("[MidiAdapter]	Stream Stop Fail");
		midiInClose(midiIn);
		f_open=false;
	}
}


void CALLBACK MidiAdapter::MidiInProc(HMIDIIN hMidiIn,UINT wMsg,DWORD userData,DWORD dwParam1,DWORD dwParam2) {
	MidiAdapter* myInstance = (MidiAdapter*)userData;
	myInstance->MidiInCallback(hMidiIn,wMsg,dwParam1,dwParam2);
}

void MidiAdapter::MidiInCallback(HMIDIIN hMidiIn,UINT wMsg,DWORD dwParam1,DWORD dwParam2){
	switch(wMsg) {
	case MIM_OPEN:		// 入力デバイス オープン コールバック
		Logger::Println("[MidiAdapter]	MidiIn Open");
		break;
	case MIM_CLOSE:		// 入力デバイスをクローズ
		Logger::Println("[MidiAdapter]	MidiIn Close");
		break;
	case MIM_DATA:		// 入力デバイス データ コールバック
		OnMimData(dwParam1, dwParam2);
		break;
	case MIM_LONGDATA:	// 入力バッファのコールバック
		printf("[MidiAdapter] Exclusive\n");
		break;
	case MIM_ERROR:		// 入力デバイス エラー コールバック
		Logger::Println("[MidiAdapter]	MidiIn Device Error");
		break;
	case MIM_LONGERROR:	// 無効なシステム エクスクルーシブ メッセージに対するコールバック
		Logger::Println("[MidiAdapter]	MidiIn Invalid Exclusive Data");
		break;
	case MIM_MOREDATA:	// ???
		Logger::Println("[MidiAdapter]	MidiIn Unknown Callback Message");
		break;
	default:
		Logger::Println("[MidiAdapter]	MidiIn Unknown Callback Message");
		break;
	}
	return;
}

void MidiAdapter::OnMimData(DWORD dwData, DWORD dwTimeStamp)
{
	unsigned char st, dt1, dt2;	// 受信データ格納バッファ
	st = dwData     & 0xFF;	// ステータス バイト
	dt1= dwData>> 8 & 0xFF;	// データ バイト１
	dt2= dwData>>16 & 0xFF;	// データ バイト２
	if(st == 0xF8 || st == 0xFE)
		return;
	
	if(st>>4 == 0xF) {
		switch(st & 0xF) {
		case 0x1:
			printf("[MidiAdapter] MTC quarter frame (Type:%d Val:2d)\n", dt1>>4&0xF, dt1&0xF);
			break;
		case 0x2:
			printf("[MidiAdapter] Song position pointer (Beat:%5d)\n", dt1+(dt2<<7));
			break;
		case 0x3:
			printf("[MidiAdapter] Song select (Num:%3d)\n", dt1);
			break;
		case 0x6:
			printf("[MidiAdapter] Tune request\n");
			break;
		case 0x7:
			printf("[MidiAdapter] End of exclusive\n");
			break;
		case 0x8:
			printf("[MidiAdapter] Timing clock\n");
			break;
		case 0xA:
			printf("[MidiAdapter] Start\n");
			break;
		case 0xB:
			printf("[MidiAdapter] Continue\n");
			break;
		case 0xC:
			printf("[MidiAdapter] Stop\n");
			break;
		case 0xE:
			printf("[MidiAdapter] Active sensing\n");
			break;
		case 0xF:
			printf("[MidiAdapter] System reset\n");
			break;
		default:
			printf("[MidiAdapter] Error : Unknown system message\n");
			break;
		}
	} else {
		char keys[12][3] = {"C ","C#","D ","D#","E ","F ","F#","G ","G#","A ","A#","B "};
		switch(st>>4) {
		case 0x8:
			printf("[MidiAdapter] Note off* (Note:%s%d Vel:%3d)\n", keys[dt1%12],dt1/12-1, dt2);
			break;
		case 0x9:
			printf("[MidiAdapter] Note %s  (Note:%s%d Vel:%3d)\n", (dt2?"on ":"off"), keys[dt1%12], dt1/12-1, dt2);
			break;
		case 0xA:
			printf("[MidiAdapter] Polyphonic key pressure (Note:%s%d Prss:%3d)\n", keys[dt1%12], dt1/12-1, dt2);
			break;
		case 0xB:
			printf("[MidiAdapter] Control change (Num:%3d Val:%3d)\n", dt1, dt2);
			break;
		case 0xC:
			printf("[MidiAdapter] Program change (Num:%3d)\n", dt1);
			break;
		case 0xD:
			printf("[MidiAdapter] Channel pressure (Prss:%3d)\n", dt1);
			break;
		case 0xE:
			printf("[MidiAdapter] Pitch bend change (Val:%5d)\n", dt1+(dt2<<7)-8192);
			break;
		default:
			printf("[MidiAdapter] Error : Unknown channel message\n");
			break;
		}
	}
	return;
}
