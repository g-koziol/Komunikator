#include "stdafx.h"
Pipe::Pipe(HWND a) :error(new char[100]), nError(0), terror(NULL), EditScreen(a), contactshwnd(NULL), _msg(NULL), tabhwndtalk(NULL)
{
}

Pipe::~Pipe()
{	
	if(_msg!=NULL)
	{
		int i=0;
		while(_msg[i]!=NULL)
		{
			delete _msg[i];
			i++;
		}	
		delete _msg[i];
		delete[] _msg;
	}
	deletehwndtalkpointers(tabhwndtalk);
	delete error;

}

void Pipe::addpipetab(LPARAM text)
{//add text to list of LPARAM variables to add at end of query

	LPARAM ** tab=_msg;
	int i=0;

	if(tab==NULL)
		return;

	if(tab[0]==NULL)//if there is no element
	{
		tab[0]=new LPARAM;
		*tab[0]=text;
		tab[1]=new LPARAM;
		*(tab[1])=(LPARAM)L"\0";
		return;
	}
	else while(tab[i]!=NULL)	//go to to the end
		i++;

	tab[i+1]=NULL; //add new and set last NULL
	*(tab[i])=text;
	return;
}

void Pipe::readpipetab(HWND handle)
{//send prapared tab to the handle to display all tab

	LPARAM ** tab=_msg;
	if(tab!=NULL)
	{
		int i=0;
		while(tab[i]!=NULL) //display while not end
		{	
			apendtoedit(handle,*(tab[i]));
			apendtoedit(handle,(LPARAM)L"\r\n");
			delete tab[i];
			i++;
		}
		_msg[0]=NULL;
	}
}

void Pipe::apendtoedit(HWND handle, LPARAM text)
{//apend to text already displayed in edit control
	int ndx = GetWindowTextLength (handle); //find end
	SetFocus (handle);
	SendMessage (handle, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx); ///set the place to paste
	SendMessage (handle, EM_REPLACESEL, 0, text); //paste text
}

void Pipe::addtabtopipetab(LPARAM ** diftab)
{//add preapred tab to already existed tab witch LPARAM elements
	LPARAM ** tab=_msg;
	int i=0,j=0;

	while(tab[i]!=NULL)	
		i++;

	while(diftab[j]!=NULL)//copy elements
	{
		tab[i]=new LPARAM;
		*tab[i]=*diftab[j];
		delete diftab[j]; //delete not need element		
		i++; j++;
	}
	tab[i+1]=NULL;
}

TCHAR * Pipe::chartotchar(const char * tab)
{//convert given char to TCHAR * (created because of Unicode code)
	TCHAR * tab2= new (TCHAR [1000]);
	int i=0;
	while(tab[i]!='\0')
	{		
		tab2[i]=tab[i];
		i++;
	}
	tab2[i]='\0';
	return tab2;	
}

void Pipe::deletehwndtalkpointers(HWND ** tabhwndtalk)
{
	if(tabhwndtalk!=NULL)
	{
		int i=0;
		while((tabhwndtalk[i])!=NULL)
		{
			delete tabhwndtalk[i];
			tabhwndtalk=NULL;
			i++;
		}
		tabhwndtalk=NULL;
	}

}

void Pipe::endmanytalkdialogbox(void)
{
	if(tabhwndtalk!=NULL)
	{
		int i=0;
		while(tabhwndtalk[i]!=NULL)
		{
			SendMessage(*tabhwndtalk[i],WM_COMMAND,constid,NULL);
			i++;
		}
		deletehwndtalkpointers(tabhwndtalk);
	}
}

void Pipe::addtalktohwndtab(HWND hwnd)
{	
	if(tabhwndtalk==NULL)
	{
		tabhwndtalk=new HWND *;
		tabhwndtalk[0]=NULL;
	}
	int i=0;
	while(tabhwndtalk[i]!=NULL)
		i++;
	tabhwndtalk[i]=new HWND;
	*tabhwndtalk[i]=hwnd;
	tabhwndtalk[i+1]=NULL;

}

void Pipe::setnewvalueinhwndtab(HWND hDlg,HWND tmp)
{
	if(tabhwndtalk!=NULL)
	{
		int i=0;
		while(*tabhwndtalk[i]!=hDlg)
		{
			if(*tabhwndtalk[i]==NULL)
				return;
			i++;
		}
		if(tmp==NULL)
		{
			delete tabhwndtalk[i];
			tabhwndtalk[i]=NULL;
		}
	}
}
