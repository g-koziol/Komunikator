#include "stdafx.h"

Data::Data(HWND a): Connection(a)
{
	time=new time_t;
}

Data::~Data(void)
{
	delete time;
}

User * Data::finddstUser(string name, User ** data)
{
	int i=0;
	while(data[i]!=NULL)
	{
		if(data[i]->name==name)
			return data[i];
		else i++;
	}
	return NULL;
}

char * Data::castedchartochar(char * src)
{//change casted char string to char string
	char * a=new char[256];
	int i=0;
	int j=0;
	while((src[j]!=0) || (src[j+1]!=0))	{
		a[i]=src[j];
		j+=2;
		i++;
	}
	a[i]=0;	
	return a;
}

void Data::addMessagetolist(Friend * frnd,Message * msg)
{//add message to list connected as talk with contact
	int i=0;
	if(frnd==NULL)
	{
		frnd=new class Friend;
		frnd->talk=NULL;
	}
	if(frnd->talk==NULL)
	{
		frnd->talk=new Message *;
		frnd->talk[0]=NULL;
	}
	else
	{
		while(frnd->talk[i]!=NULL)
			i++;
	}
	//set NULL pointer for the last element of the list
	frnd->talk[i]=new Message(msg->from,msg->to,msg->data,msg->time,msg->status); //copy received message to new element of the list
	frnd->talk[i+1]=NULL; 
}

void Data::showMessages(HWND Edit,Friend * frnd )
{//show messagess in sent Edit control from sent pointer to friend
	int i=0;
	string text="";
	if(frnd==NULL)
		return;
	else if(frnd->talk==NULL)
		return;
	else if(frnd->talk[i]==NULL)
		return;

	while(frnd->talk[i]!=NULL)
	{
		if(frnd->talk[i]->visible==true) //if message is displayed continue to next
		{
			i++;
			continue;
		}
		text=frnd->talk[i]->from + " - " + frnd->talk[i]->time + "\r\n" + frnd->talk[i]->data + "\r\n"; //make text prepared to send to edit control
		frnd->talk[i]->visible=true; //set parametr of message as displayed
		TCHAR * str=chartotchar(text.c_str());
		apendtoedit(Edit,(LPARAM)str); //add to tab of messagess, before conversion to TCHAR
		i++;
	}
	readpipetab(Edit); //read prepared messages
}

void Data::hideMessagess(Friend * frnd)
{//set parametr visible to false for each message of chat with sent friend
	int i=0;
	if(frnd->talk==NULL) //if there is no chat go back
		return;
	while(frnd->talk[i]!=NULL)
	{
		frnd->talk[i]->visible=false;
		i++;
	}
}

Friend * Data::_getcontactfromserver(Message * msg)
{//getting information about contacts from server
	Friend * frnd=new Friend;

	if(msg->to=="STOP")
		return NULL;
	frnd->name=msg->from;
	return frnd;
}

void Data::_sendcontactsfromserver(User ** data, SOCKET dst)
{//function which send actual list of the connected users to one user by sent socket
	Message * a=new Message;
	int i=0, _name=0, _to=sizeof("STOP")+1, _status=sizeof(int), status=5;

	while(data[i]!=NULL) //send all contacts until end
	{
		*a=Message(data[i]->name,"","","",5);
		_send(a,dst);
		i++;
	}
	*a=Message("","STOP","","",5); //send message aout end of sending contacts
	_send(a,dst);
	delete a;
}

void Data::_sendUserdatatoserver(User * User,SOCKET dst)
{//send user data to server, first send size of each item of class, and then a data
	char * buffer=new char[1000];
	char * old=buffer;
	WORD _name=User->name.size()+1,_number=User->number.size()+1,_password=User->password.size()+1,_fileName=User->fileName.size()+1;

	memset(buffer,0,1000);
	memcpy(buffer,&_name,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(buffer,&_number,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(buffer,&_password,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(buffer,&_fileName,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(buffer,User->name.c_str(),_name);
	buffer+=_name;
	memcpy(buffer,User->number.c_str(),_number);
	buffer+=_number;
	memcpy(buffer,User->password.c_str(),_password);
	buffer+=_password;
	memcpy(buffer,User->fileName.c_str(),_fileName);
	buffer+=_fileName;
	//change the pointer to begin and send data
	buffer=old;
	send(dst,buffer,1000,0);
	delete buffer; //delete send data
}

User * Data::_reciveUserdatafromUser(SOCKET src, User ** data)
{//get contact info for user to make own list of avaliable contacts
	User * a=new User;
	char * buffer=new char[1000];
	char * work=new char[1000];
	char * old=buffer;

	memset(buffer,0,1000);
	//check if recived any data
	int inDataLength=-1;
	inDataLength=recv(src,buffer,1000,0);
	if(inDataLength==-1)
	{
		delete a;
		delete old;
		delete work;
		return NULL;
	}
	//create information about sizes of recived class
	WORD _name,_number,_password,_fileName;
	memcpy(&_name,buffer,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(&_number,buffer,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(&_password,buffer,sizeof(WORD));
	buffer+=sizeof(WORD);
	memcpy(&_fileName,buffer,sizeof(WORD));
	buffer+=sizeof(WORD);
	//get the infrmation about class data
	memcpy(work,buffer,_name);
	buffer+=_name;
	a->name=(string)work;
	memcpy(work,buffer,_number);
	buffer+=_number;
	a->number=(string)work;
	memcpy(work,buffer,_password);
	buffer+=_password;
	a->fileName=(string)work;
	buffer+=1;	
	//if there is any error in sended data delete data and return NULL
	if(_name=='\0' && _number=='\0')
	{
		delete a;
		delete old;
		delete work;
		return NULL;
	}
	//delete no more needed variable and return data as message
	delete work;	
	delete old;
	if(finddstUser(a->name,data)!=NULL)
	{
		delete a;
		return NULL;
	}
	return a;
}

