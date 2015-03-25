#include "stdafx.h"

void MemoryDump::writememory()
{
	writetofile("Blad nastapil przy uzywaniu wskaznika do:");
	if((*file).fail())
		return;
	if(msg!=NULL)
	{
		writemessages(msg);
	}
	else if(frnd!=NULL)
	{
		writefriends(frnd);
	}
	else if(users!=NULL)
	{
		writeusers(users);
	}
}

void MemoryDump::writemessages(Message ** msg)
{
	int i=0;
	if(msg!=NULL)
		while(msg[i]!=NULL)
		{
			Dump <Message *> tmp(msg[i]);
			(*file) << "\t\tMessage: " ;
			(*file) << tmp;
			(*file) << endl;
			i++;
		}
}
void MemoryDump::writefriends(Friend ** frnd)
{
	int i=0;
	if(frnd!=NULL)
		while(frnd[i]!=NULL)
		{
			Dump <Friend *> tmp(frnd[i]);
			(*file) << "\tContact: " ;
			(*file) << tmp;
			(*file) << endl;
			writemessages(frnd[i]->talk);
			i++;
		}
}

void MemoryDump::writeusers(User ** users)
{
	int i=0;
	if(users!=NULL)
		while(users[i]!=NULL)
		{
			Dump <User *> tmp(users[i]);
			(*file) << "User: ";
			(*file) << tmp;
			(*file) << endl;
			writefriends(users[i]->contacts);
			i++;
		}
}
