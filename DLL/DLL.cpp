// DLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
/*
---------------------------------------------------------------------
All definition of classess in separate files!
-----------------------------------------------------------------------
*/

void deletepipe(LPARAM ** tab)
{
	int i=0;
	while(*(tab[i])!=(LPARAM)L"\0")
	{
		delete tab[i];
		i++;
	}	
	delete tab[i];
	delete tab;
}


