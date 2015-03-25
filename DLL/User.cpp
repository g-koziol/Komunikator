#include "stdafx.h"


User::User()
{
	name=string();
	number=string();
	password=string();
	fileName=string();
	contacts=NULL;
	socketClient=INVALID_SOCKET;
	socketaddrClient.sin_addr.s_addr = inet_addr( LOCAL_IP );
}

User::~User()
{
	int i=0;
	if(this!=NULL)
	{
		if(contacts!=NULL)
		{//delete contacts
			while(contacts[i]!=NULL)
			{
				delete contacts[i];
				contacts[i]=NULL;
				i++;
			}
		}
	}
}