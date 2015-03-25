#include "stdafx.h"

ErrorSocket::ErrorSocket(HWND hwnd,int nerr) : _Error(hwnd), number(nerr), tstr(NULL), buf(NULL)
{	
	
}

ErrorSocket::~ErrorSocket(void)
{
	delete tstr;
	delete buf;
}

bool ErrorSocket::checknumber() //add more definitions if you want additional info
{
	wstring wstr=L"";
	string str="Blad WinSock: ";
	str+=to_string(number);
	str+="\r\n";
	if(number==WSAENOTCONN) //in case of fail connection
		str+="Program nie laczy sie z serwerem.\nSprawdz adres IP";
	else if(number==10037)
		str+="Program nie laczy sie z serwerem.\nSprawdz adres IP";
	else if(number==WSAEFAULT ) //in case of error which don't influence on program
		return true;
	else if(number==WSAEISCONN)
		return true;
	else
		str+="Sprawdz w dokumentacji\r\n";
	wstr=wstring(str.begin(),str.end());
	starterror();
	writetofile(str);
	writetohwnd((TCHAR *)wstr.c_str());
	return false;
}