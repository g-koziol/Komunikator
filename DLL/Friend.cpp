#include "stdafx.h"
Friend::Friend() : chatedit(NULL), name(""), talk(NULL)
{
}

Friend::Friend(string x) : name(x),talk(NULL)
{
}

Friend::~Friend()
{
	name.~string();
	if(talk!=NULL)
	{
		int i=0;
		while(talk[i]!=NULL)
		{
			delete talk[i];
			talk[i]=NULL;
			i++;
		}
	}
	
}