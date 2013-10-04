
#pragma once

#include "Common.h"

#define MAX_TOUCH 50

class TouchCursor{
private:
	int x;
	int y;
	float ve;
	float ac;
	long life;
	bool active;
	int id;
public:
	TouchCursor(int _id,int _x,int _y){
		id=_id;
		x=_x;
		y=_y;
		ve=0.0f;
		ac=0.0f;
		life=0;
		active=true;
	}
	TouchCursor(){
		id=-1;
		x=0;
		y=0;
		ve=0.0f;
		ac=0.0f;
		life=0;
		active=false;
	}
	virtual ~TouchCursor(){}
	virtual void Init(){
		id=-1;
		x=0;
		y=0;
		ve=0.0f;
		ac=0.0f;
		life=0;
		active=false;
	}
	int GetX(){return x;};
	int GetY(){return y;};
	int GetId(){return id;};
	float GetVelocity(){return ve;};
	float GetAccel(){return ac;};
	long GetLife(){return life;};
	void SetX(int _x){x=_x;};
	void SetY(int _y){y=_y;};
	void SetId(int _id){id=_id;};
	void SetActive(bool _active){active=_active;};
	void SetVelocity(float v){ve=v;};
	void SetAccel(float a){ac=a;};
	void SetLife(DWORD l){life=l;};
	bool IsActive(){return active;};
};

class InputApi{
private:
public:
	virtual void Lock()=0;
	virtual void UnLock()=0;

	virtual int GetCursorNum()=0;
	virtual void GetCursorList(vector<TouchCursor>*)=0;
	virtual void GetCursorList(TouchCursor*,int*)=0;
};