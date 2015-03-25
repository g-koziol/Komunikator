#include "stdafx.h"

Client::Client(HWND a): Data(a), boxmodeless(NULL)
{	
	initialize();
}

Client::~Client(void)
{
	clear();	//cleaning up
}

bool Client::_connect (HWND & hWnd, int wmsocket)
{//connect with server, sent address to handler and number of socket
	try{//all procedures needed to connect with server

		int a=0;
		if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)						
			throw ErrorSocket(NULL,WSAGetLastError());
		User->socketClient=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(User->socketClient  == INVALID_SOCKET)		
			throw ErrorSocket(NULL,WSAGetLastError());
		memset(&User->socketaddrClient, 0, sizeof(User->socketaddrClient));
		User->socketaddrClient.sin_family = AF_INET;
		User->socketaddrClient.sin_addr.s_addr =inet_addr(szLocalIP);
		User->socketaddrClient.sin_port = htons( port );
		if(WSAAsyncSelect(User->socketClient, hWnd, wmsocket, (FD_CLOSE|FD_ACCEPT|FD_READ)))
			throw ErrorSocket(NULL,WSAGetLastError());
		int tmp;
		while(1)
		{
			tmp=connect(User->socketClient,(LPSOCKADDR)(&(User->socketaddrClient)),sizeof(User->socketaddrClient));
			if(tmp==SOCKET_ERROR)
			{
				a=WSAGetLastError();
				if(a!=WSAEWOULDBLOCK)	
					throw ErrorSocket(NULL,WSAGetLastError());
				if(a==WSAEISCONN)
					break;
			}			
		}
		if(recv(User->socketClient,NULL,1,MSG_PEEK)==-1)
			throw ErrorSocket(NULL,WSAGetLastError());

	}
	catch(ErrorSocket x)
	{		
		if(x.checknumber())
			return true;		
		_endconnection(User->socketClient);
		return false;
	}
	catch(...)
	{
		_Error err(NULL);
		err.general();
		return false;
	}
	return true;
}



void Client::action(Message * msg,HWND actual)
{
	switch(msg->status)
	{
	case 0://Message is recived	
		{
			startgettingcontactsfromserver();
			addMessagetolist(findFriend(User->contacts,msg->from),msg);
			newMessage=true;
		}
		break;
	case 1://raport from server
		{			
			if(msg->from=="User_exists")
				MessageBox(actual,(LPCWSTR)L"Uzytkownik o tej nazwie juz istnieje, wybierz inna nazwe",(LPCWSTR)L"MB_ICONERROR",MB_ICONERROR);
			if(msg->data=="Disconnected")
			{
				apendtoedit(findFriend(User->contacts,msg->to)->chatedit,(LPARAM)L"Uzytkownik nie jest podlaczony! Kolejne wiadomosci do niego nie zostana dostarczone,chyba ze podlaczy sie na nowo. Cofnij sie do glownego menu i sprawdz ponownie czy jest na liscie uzytkownikow!\r\n");
			}
		}
		break;
	case 5://message with contact information
		{
			if(User->contacts==NULL)
			{
				User->contacts=new class Friend *;	
				User->contacts[0]=NULL;
			}
			if(msg->to=="STOP")
			{
				endedgettingcontacts=true;
				break;
			}
			int i=0;
			if(User->name==msg->from)
				return;
			while(User->contacts[i]!=NULL)
			{	
				if(User->contacts[i]->name==msg->from)
					return;
				i++;
			}
			User->contacts[i]=_getcontactfromserver(msg);
			User->contacts[i+1]=NULL;

		}			
		break;
	}
}

Friend * Client::findFriend(Friend** tab,string name)
{//find friend on the list in memmory
	int i=0;
	if(tab!=NULL)
		while(tab[i]!=NULL)
		{
			if(tab[i]->name==name) //if the looking and existig contact match return pointer 
				return tab[i];
			i++;
		}
	return NULL; //if there is no such friend
}

void Client::initialize(void)
{//set the parameter to default
	endedgettingcontacts=FALSE; 
	newMessage=false;
	actual="";
	actualint=0; 
	buffer=new TCHAR[1024];
	User=new class User;	
}

void Client::clear(void)
{//clear the memory after class
	int i=0, j=0,k=0;
	if(User!=NULL)
	{
		_endconnection(User->socketClient);
		delete User;
		User=NULL;
	}
	if(buffer!=NULL)
	{
		delete buffer;
		buffer=NULL;
	}
}

void Client::startgettingcontactsfromserver(void)
{
	class Message * start=new class Message("",User->name,"START","",5); //message send to start transfer contatcs from server
	_send(start,User->socketClient);
	delete start;
	endedgettingcontacts=FALSE;
	class Message * msg=NULL;
	while(endedgettingcontacts!=TRUE) //loop for control tranfer data and chatch the end of operation
	{
		msg=_recive(User->socketClient);
		if(msg!=NULL)
			action(msg,NULL); //check type of message
		delete msg;
		msg=NULL;				
	}
}

