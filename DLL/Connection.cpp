#include "stdafx.h"

Connection::Connection(HWND a) : Files(a),nResult(0), port(27015), addr(0),szHostName(NULL),szLocalIP(NULL)
{
	szHostName=new char[255];
	szLocalIP=new char[256];
	strcpy(szLocalIP,"127.0.0.1");
}

Connection::~Connection(void)
{
	delete[] szHostName;
	delete[] szLocalIP;
}

void Connection::_send(Message * msg, SOCKET dst)                          
{//send given message to destination given by socket
	if(msg==NULL)
	{//if sent message is NULL
		Message * a =new Message();
		msg=a;
	}
	//prepare variablew
	char * buffer=new char[1000];
	char * old=buffer;
	WORD _f=msg->from.size()+1,_t=msg->to.size()+1,_d=msg->data.size()+1,_ti=msg->time.size()+1, _s=sizeof(msg->status)+1;
	//prepare size of next elements of class
	memset(buffer,0,1000);
	memcpy(buffer,&_f,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(buffer,&_t,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(buffer,&_d,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(buffer,&_ti,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(buffer,&_s,sizeof(WORD));
	buffer+=sizeof(WORD);
	//prepare data
	memcpy(buffer,msg->from.c_str(),_f);
	buffer+=_f;
	memcpy(buffer,msg->to.c_str(),_t);
	buffer+=_t;
	memcpy(buffer,msg->data.c_str(),_d);
	buffer+=_d;
	memcpy(buffer,msg->time.c_str(),_ti);
	buffer+=_ti;;
	memcpy(buffer,&(msg->status),sizeof(msg->status));

	buffer=old;
	send(dst,buffer,1000,0); //send prepared data
	delete buffer;
}


Message * Connection::_recive(SOCKET src)
{//recive data which was sent
	Message * a=new Message;
	char * buffer=new char[1000];
	char * old=buffer;
	char * work=new char[1000];
	memset(buffer,0,1000);
	//check if data is correct
	WORD inDataLength=recv(src,buffer,1000,0);
	if(inDataLength==-1 || inDataLength==0xffff)
	{
		delete a;
		delete old;
		delete work;
		return NULL;
	}
	//get data from buffer
	WORD _f,_t,_d,_ti, _s;
	memcpy(&_f,buffer,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(&_t,buffer,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(&_d,buffer,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(&_ti,buffer,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(&_s,buffer,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(work,buffer,_f);
	buffer+=_f;
	a->from=(string)work;
	memcpy(work,buffer,_t);
	buffer+=_t;
	a->to=(string)work;
	memcpy(work,buffer,_d);
	buffer+=_d;
	a->data=(string)work;
	memcpy(work,buffer,_ti);
	buffer+=_ti;
	a->time=(string)work;
	memcpy(&(a->status),buffer,_s);
	delete old;
	delete work;
	return a; //return data
}

string Connection::setip(BYTE a,BYTE b,BYTE c,BYTE d)
{ //made ip as string, every part of ip is send in different parts as BYTEs
	string ip="";
	char work[256];
	//contract strings into one
	ip=_itoa((int)a,work,10);
	ip+=".";
	ip+=_itoa((int)b,work,10);
	ip+=".";
	ip+=_itoa((int)c,work,10);
	ip+=".";
	ip+=_itoa((int)d,work,10);
	ip+="\0";

	return ip;
}

void Connection::_endconnection(SOCKET a)
{//end connection for given socket
	closesocket(a);
	shutdown(a,SD_BOTH);
	a=INVALID_SOCKET;
	WSACleanup();
}


