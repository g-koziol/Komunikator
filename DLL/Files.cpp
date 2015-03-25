#include "stdafx.h"

Files::Files(HWND a): Pipe(a)
{
	//file= new (fstream*);		
}

Files::~Files()
{
	//delete file;
}

/*
User ** Files::_opens() 
{
	DWORD ftype=GetFileAttributesA("Data/");
	if(!(ftype & FILE_ATTRIBUTE_DIRECTORY))
	{
		addpipetab((LPARAM)L"Katalog z danymi nie istnieje");
		return NULL;
	}

	string first_file="list";
	string name="Data/" + first_file + ".txt";
	fstream a;
	a.open(name.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
	addpipetab((LPARAM)L"Otwieranie plikow z lista uzytkwnikow...");
	if(a.fail())
	{
		a.open(name.c_str(), std::fstream::trunc);
		if(a.fail())
		{		
			addpipetab((LPARAM)L"Blad przy otwieraniu pliku z lista");
			a.close();
			return NULL;
		}

	}
	string * tab[100];
	char * buffer=new (char [100]);
	int i=0;
	//from main file read all numbers of Users
	while(!a.eof())
	{
		a.getline(buffer,100);
		tab[i]=new string[100];
		*(tab[i])=(string)buffer;
		i++;
	}
	tab[i]=NULL;
	names=tab;
	delete buffer;

	User ** data=new User *;
	i=0;
	while(tab[i]!=NULL)
	{
		data[i]=new User;
		data[i]->name=*tab[i];
		data[i]->contacts=NULL;
		data[i]->fileName="";
		data[i]->number="";
		data[i]->socketClient=INVALID_SOCKET;
		data[i]->status=0;
		i++;
	}
	data[i]=NULL;
	//_openseach(&a);

	return data;

}

void Files::_openseach(fstream * l)
{
	/*
	i=0;
	//open each file and make pointer on it
	while(tab[i]!=NULL){
	string address="Data/Users/" + *tab[i] +"/"+ *tab[i] + ".txt";
	file[i]=new fstream;
	file[i]->open(address.c_str(),std::fstream::in | std::fstream::out | std::fstream::app );		
	i++;
	}
	file[i]=NULL;
	delete buffer;
}

void Files::_closes(void)
{
	int i=0;
	while (file[i]!=NULL)
	{
		file[i]->close();
		delete file[i];
		i++;
	}
	delete file[i];
}

void Files::_openc(void)
{
	DWORD ftype=GetFileAttributesA("..\/Data");
	if(ftype == INVALID_FILE_ATTRIBUTES){
	addpipetab((LPARAM)L"Katalog z danymi nie istnieje",0);
	return;
	}
	if(!(ftype&FILE_ATTRIBUTE_DIRECTORY)){
	addpipetab((LPARAM)L"Katalog z danymi nie istnieje",0);
	return;
	}
	string first_file="contacts";
	string second_file="Userinfo";
	string name="Data/" + first_file + ".txt";
	string name2="Data/" +second_file + ".txt";
	fstream a, b;
	a.open(name.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
	b.open(name2.c_str(), std::fstream::in | std::fstream::out | std::fstream::app); 
	addpipetab((LPARAM)L"Otwieranie plikow z danymi uzytkownika...");
	if(a.fail())
	{
		a.open(name.c_str(), std::fstream::trunc);
		if(a.fail())
		{		
			addpipetab((LPARAM)L"Blad przy otwieraniu pliku z lista");
			a.close();
			return;
		}	
	}

	string * tab[100];
	char * buffer=new (char [100]);
	int i=0;
	//from main file read all numbers of Users
	while(!a.eof())
	{
		a.getline(buffer,100);
		tab[i]=new string[100];
		*(tab[i])=(string)buffer;
		i++;
	}
	tab[i]=NULL;
	names=tab;
	delete buffer;

	_openceach(tab);


}

void Files::_openceach(string ** tab)
{
	int i=0;
	string name="";
	while(tab[i]!=NULL)
	{
		file[i]=new fstream;
		name="Data/" + (*tab[i]) + ".txt";
		(*file[i]).open(name.c_str(), std::fstream::in | std::fstream::out);
		if((*file[i]).fail())
		{
			string b="BBBBLAAAD";
		}
		i++;
	}
	file[i]=NULL;
}

void Files::_closec(void)
{
	int i=0;
	while(file[i]!=NULL)
	{
		(*file[i]).close();
		delete file[i];
		i++;
	}
	delete file[i];
}

void Files::safec(User * User)
{
	fstream f, fl;
	string name="";
	fl.open("Data/list.txt", std::fstream::out);
	int i=0,j;
	while(User->contacts[i]!=NULL){
	name="Data\/" + User->contacts[i]->number + ".txt";
	f.open(User->contacts[i]->number.c_str(),std::fstream::out);
	fl << User->contacts[i]->number.c_str() << "\n";
	f << User->contacts[i]->name <<  "\n" << User->contacts[i]->number << "\n";
	j=0;
	while(User->contacts[i]->talk[j]!=NULL){
	f << User->contacts[i]->talk[j]->time << "\n";
	f << User->contacts[i]->talk[j]->from << "\n";
	f << User->contacts[i]->talk[j]->to << "\n";
	f << User->contacts[i]->talk[j]->data<< "\n";
	j++;
	}
	f.close();
	i++;
	}
	fl.close();
}

*/