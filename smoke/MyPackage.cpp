
#include "MyPackage.h"

MyPackage* MyPackage::myInstance=NULL;

Package* CALLBACK MyPackage::CreateInstance(){
	if(myInstance==NULL){
		myInstance=new MyPackage;
	}
	return myInstance;
}
void CALLBACK MyPackage::DestroyInstance(){
	if(myInstance!=NULL){
		SAFE_DELETE(myInstance)
	}
}

void CALLBACK MyPackage::GetName(char* buf,int max){
	sprintf_s(buf,max,"Smoke");
}