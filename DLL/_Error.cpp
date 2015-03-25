#include "stdafx.h"


_Error::_Error(const _Error & object)
{
	this->time=object.time;
	this->tmphwnd=object.tmphwnd;
	this->file=object.file;
}

_Error::_Error(HWND hwnd) : tmphwnd(hwnd),file(NULL)
{	
	time=::time(&time);	
}		

_Error::_Error(HWND hwnd, TCHAR * tstr) : tmphwnd(hwnd),file(NULL)
{
	time=::time(&time);
	writetohwnd(tstr);
	wstring ws=wstring(tstr);
	string a(ws.begin(),ws.end());
	writetofile(a);
}

_Error::_Error(string str) : tmphwnd(NULL),file(NULL)
{
	time=::time(&time);
	writetofile(str);
}

_Error::~_Error()
{
	if(file!=NULL)
	{
		(*file).close();
		delete file;
		file=NULL;
	}
}

void _Error::writetofile(string str)
{
	if(starterror())
		(*file) << str.c_str() << endl;
}

void _Error::writetohwnd(TCHAR * tstr)
{
	if(tmphwnd!=NULL)
	{
		if(tstr!=NULL)
		{
			int ndx = GetWindowTextLength (tmphwnd); //find end
			SetFocus (tmphwnd);
			SendMessage (tmphwnd, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx); ///set the place to paste
			SendMessage (tmphwnd, EM_REPLACESEL, 0, (LPARAM)tstr);
		}
	}
	if(tstr!=NULL)
	{
		MessageBox(NULL,(PCWSTR)tstr,(PCWSTR)L"Blad",MB_ICONERROR);
	}
}

inline bool _Error::starterror(void)
{
	if(file==NULL)
		file=new ofstream("error.txt",std::fstream::out | std::fstream::app | std::fstream::in);
	if(file)
	{
		(*file) << ctime(&time)  << "Uruchomiono zrzut dla bledow" << endl;	
		return true;
	}	
	return false;
}

void _Error::general(void)
{
	writetofile("Blad ogolny");
	MessageBox(NULL,(LPCWSTR)L"Nieznany blad", (LPCWSTR)L"Blad",MB_ICONERROR);
}





