#include "stdafx.h"

Server::Server(HWND a) : Data(a), nClient(0), data(NULL)
{	
	data=NULL;
}

Server::~Server(void)
{
	_endconnection(this->mainSocket);
	if(data!=NULL)
	{				
		int i=0;	
		while(data[i]!=NULL)
		{
			delete data[i];
			data[i]=NULL;
			i++;
		}
	}
}

void Server::start(void)
{//create working server, all messagess are shown at end of function
	try
	{
		if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
		{
			apendtoedit(EditScreen,(LPARAM)L"WSA Initialization failed!\r\n");
			WSACleanup();
		}
		else 
			apendtoedit(EditScreen,(LPARAM)L"Wlaczenie modulu sieciowego powiodlo sie!\r\n");	

		mainSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(mainSocket == INVALID_SOCKET)
		{
			throw ErrorSocket(EditScreen,WSAGetLastError());
			WSACleanup();
		}
		memset(&socketAddress, 0, sizeof(socketAddress));
		socketAddress.sin_family = AF_INET;
		socketAddress.sin_addr.s_addr = inet_addr(szLocalIP );
		socketAddress.sin_port = htons( port );

		if( bind( mainSocket,( SOCKADDR * ) (&socketAddress), sizeof( socketAddress ) ) == SOCKET_ERROR )
		{
			throw ErrorSocket(EditScreen,WSAGetLastError());
			_endconnection(mainSocket);
			return;
		}	
	}
	catch(ErrorSocket x)
	{		
		if(x.checknumber())
			return;		
		_endconnection(mainSocket);
		return;
	}
	catch(...)
	{
		_Error err(NULL);		
		err.general();
		return;
	}

}	

void Server::action(Message * msg)
{//select action depending to type of message
	if(msg==NULL)
		return;
	switch(msg->status)
	{
	case 0:
		_sendfrompeertopeer(msg);		
		break;
	case 1:
		{//send a message from the clients to the server or receive, depending on addresses, 0 is address of server
			terror=chartotchar(msg->from.c_str());
			addpipetab((LPARAM)terror);
			delete terror;
			terror=NULL;
			terror=chartotchar(msg->to.c_str());
			addpipetab((LPARAM)terror);
			delete terror;
			terror=NULL;
			terror=chartotchar(msg->data.c_str());
			addpipetab((LPARAM)terror);
			delete terror;
			terror=NULL;
			terror=chartotchar(msg->time.c_str());
			addpipetab((LPARAM)terror);
			delete terror;
			terror=NULL;
		}
		break;
	case 3://data for synchronization?
		break;
	case 5:
		{//send contacts to user pointed by msg->to
			User * dst=finddstUser(msg->to,data);
			if(dst!=NULL) //if destiantion user is find on the list in memory send contacts from server
				_sendcontactsfromserver(data,dst->socketClient);
			else
			{			
				Message * tmp=new Message("","STOP","","",5);
				_send(tmp,dst->socketClient); //if not stop operation
				delete tmp;
			}
		}
		break;
	}
}

User ** Server::addUsertodata(User * Client, SOCKET Clientsocket)
{
	int i=0;	
	while(data[i+1]!=NULL)
	{
		if(data[i]->socketClient==Clientsocket)
			break;
		i++;
	}
	(*data[i]).name.insert(0,(const char *)Client->name.c_str());
	data[i]->number.insert(0,(const char *)Client->number.c_str());
	data[i]->password.insert(0,(const char *)Client->password.c_str());
	data[i]->fileName.insert(0,(const char *)Client->fileName.c_str());
	data[i]->status=1;

	return data;

}

void Server::_sendfrompeertopeer(Message * msg)
{
	string to=msg->to;
	char * buf=new char[100];
	int i=0;
	User * dst;
	dst=finddstUser(to,data);
	if(dst!=NULL)
		_send(msg,dst->socketClient);
	else
	{
		msg->status=1;
		msg->data="Disconnected";
		_send(msg,finddstUser(msg->from,data)->socketClient);
	}

}

void Server::deletesocket(int i)
{
	if(i>=0) 
	{
		delete data[i];
		if(data[i+1]==NULL)//jesli to jest jedyny element	
			data[i]=NULL;			
		else//jesli cos jest dalej
		{			
			data[i]=data[i+1];
			if(data[i]!=NULL) //jesli sa dalej dane
			{
				i++;
				while(data[i]!=NULL)
				{
					data[i]=data[i+1];
					i++;
				}
			}
		}
	}

}

int Server::findclosedsocket(void)
{
	char a;
	int i=0, nr=0;
	while(data[i]!=NULL)
	{
		nr=recv(data[i]->socketClient ,&a,1, MSG_PEEK);
		if(nr==0)
		{
			closesocket(data[i]->socketClient);
			shutdown(data[i]->socketClient,SD_BOTH);
			return i;
		}
		i++;
	}
	return i;
}

