#pragma once
#include "stdafx.h"
#include "Klient.h"
#define WM_SOCKET 200

using namespace std;
Client client(NULL);	


ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND hWnd,UINT Message,WPARAM wParam,LPARAM lParam);
INT_PTR CALLBACK	Info(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Settings(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Contacts(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Talk(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Menu(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPTSTR lpCmdLine,_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	// TODO: Place code here.

	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MENU1));


	// Main Message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}


	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{//register od window class
	WNDCLASSEX wClass;

	ZeroMemory(&wClass,sizeof(WNDCLASSEX));
	wClass.cbClsExtra=NULL;
	wClass.cbSize=sizeof(WNDCLASSEX);
	wClass.cbWndExtra=NULL;
	wClass.hbrBackground=(HBRUSH)COLOR_WINDOW;
	wClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	wClass.hIcon=NULL;
	wClass.hIconSm=NULL;
	wClass.hInstance=hInstance;
	wClass.lpfnWndProc=WndProc;
	wClass.lpszClassName=L"WIN";
	wClass.lpszMenuName=MAKEINTRESOURCE(IDC_MENU);
	wClass.style=CS_HREDRAW|CS_VREDRAW;

	return RegisterClassEx(&wClass);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	client.hInst = hInstance; // Store instance handle in class variable

	hWnd = CreateWindowEx(NULL,
		L"WIN",
		L"Jarjar Klient",
		WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX| WS_THICKFRAME),
		200,
		200,
		640,
		800,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	//A window is not needed to be show
	//ShowWindow(hWnd, nCmdShow);
	//UpdateWindow(hWnd);
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
	case WM_CREATE :
		{
			DialogBox(client.hInst, MAKEINTRESOURCE(IDD_MAIN), hWnd, Menu);
		}
		break;	
	}

	return DefWindowProc(hWnd,msg,wParam,lParam);
}

INT_PTR CALLBACK Info(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{//dialogbox with info about program
	UNREFERENCED_PARAMETER(lParam); //makro for surpress the W4 warnings
	switch (Message)
	{
	case WM_INITDIALOG:
		SetWindowText(GetDlgItem(hDlg,IDC_INFO_STATIC),(LPCWSTR)L"Komunikator JarJar\nKlient\nby Grzegorz Kozioł\n\n\nProsze w ustawieniach wpisac adres IP serwera.\nNastępnie podać swój numer/nick \ni podłączyć się przyciskiem uruchom.\nPojawi się lista użytkowników aktualnie dostepnych.\nKliknąć podwójnie myszka na wskazany nick i wyswietli sie okienko rozmowy\nZycze milego uzytkowania!");
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if(LOWORD(wParam) == IDC_INFO_OK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Settings(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{//dialog box with option to change IP
	LPARAM state=0;	
	DWORD addr=0;
	HWND hIP=GetDlgItem(hDlg,IDC_SETTINGS_IPADDRESS); //returns a handle to a dialogbox
	UNREFERENCED_PARAMETER(lParam);
	switch (Message)
	{
	case WM_INITDIALOG:
		{
			SendMessage(hIP,IPM_SETADDRESS,NULL,MAKEIPADDRESS(client.socketAddress.sin_addr.S_un.S_un_b.s_b1,client.socketAddress.sin_addr.S_un.S_un_b.s_b2,client.socketAddress.sin_addr.S_un.S_un_b.s_b3,client.socketAddress.sin_addr.S_un.S_un_b.s_b4));
			return (INT_PTR)FALSE;
		}
	case WM_COMMAND:
		switch(LOWORD(wParam))  //when user clicks the apply button
		{
		case IDC_SETTINGS_APPLY:
			{
				SendMessage(hIP,IPM_GETADDRESS,NULL,(LPARAM)&addr);	 //get IP from the IP control and set as a new IP in program and exit			
				string a=client.setip(FIRST_IPADDRESS((LPARAM) addr),SECOND_IPADDRESS((LPARAM) addr),THIRD_IPADDRESS((LPARAM) addr),FOURTH_IPADDRESS((LPARAM) addr));
				strcpy(client.szLocalIP,(const char *)a.c_str());
				client.socketAddress.sin_addr.s_addr =inet_addr(client.szLocalIP);

				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
		case IDC_SETTINGS_CANCEL: //exit from dialogbox
		case ID_EXIT:
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
		}
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Contacts(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (Message)
	{
	case WM_INITDIALOG:
		{
			client.contactshwnd=hDlg;
			client.startgettingcontactsfromserver();
			TCHAR * tab=NULL;
			if(client.User->contacts[0]==NULL)	//if there is not any contact
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			HWND hList = GetDlgItem(hDlg, IDC_CONTACTS_LIST);
			int i=0,j;
			string position=""; //variable for name of contact
			while(client.User->contacts[i]!=NULL) //loop for adding contacts connected with server
			{
				j=-1;
				if(client.User->contacts[i]->name==client.User->name) //don't add user to list
				{
					i++;
					continue;
				}
				position=client.User->contacts[i]->name;
				tab=client.chartotchar(position.c_str()); 
				SendMessage(hList,LB_FINDSTRING,(WPARAM)&j,(LPARAM)tab); //find position with send string
				delete tab;
				tab=NULL;
				if((WPARAM)&j==LB_ERR)
				{
					i++;
					continue;
				}
				tab=client.chartotchar(position.c_str());
				int pos=(int)SendMessage(hList,LB_ADDSTRING,0,(LPARAM)tab);//add position
				SendMessage(hList,LB_SETITEMDATA,pos,(LPARAM) i);
				delete tab;
				i++;
			}
			SetFocus(hList); //keep keybord focus on dialogbox
			return (INT_PTR)FALSE;	
		}
	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDC_CONTACTS_OK:
			case IDC_CONTACTS_CANCEL:
			case IDCANCEL:
			case ID_EXIT:
			case client.constid:
				client.endmanytalkdialogbox();	
				EndDialog(hDlg, LOWORD(wParam));
				client.contactshwnd=NULL;
				return (INT_PTR)FALSE;
			}
		}
	case IDC_LISTA:
		{
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				{//changed selection on the list
					HWND hList = GetDlgItem(hDlg,IDC_CONTACTS_LIST);
					int lbItem =(int)SendMessage(hList, LB_GETCURSEL,0 ,0);
					int i = (int)SendMessage(hList, LB_GETITEMDATA, lbItem, 0);	
					client.actualint=i;
					return (INT_PTR)FALSE;
				}
				break;
			case LBN_DBLCLK:
				{//user double-clicked on the position
					
					client.actual=client.User->contacts[client.actualint]->name;
					client.boxmodeless=CreateDialog(client.hInst, MAKEINTRESOURCE(IDC_TALK), hDlg, Talk); //create non-blocking dialogbox to chat with another user
					ShowWindow(client.boxmodeless,SW_SHOW);
					
					(INT_PTR)FALSE;
				}
				break;
			}
		}
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Talk(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{//DialogBox for exchange messages
	UNREFERENCED_PARAMETER(lParam);
	HWND name=GetDlgItem(hDlg,IDC_TALK_NAME), chat=GetDlgItem(hDlg,IDC_TALK_EDIT), sendbox=GetDlgItem(hDlg,IDC_TALK_SENDEDIT);//catching handlers
	Friend * frnd=client.findFriend(client.User->contacts,client.actual); //setting actual friend
	client.EditScreen=chat;//setting actual chat controlnew
	switch (Message)
	{
	case WM_INITDIALOG:
		{	
			
			client.hideMessagess(frnd);
			client.newstateofnewMessage(true);
			TCHAR * tab=client.chartotchar(client.actual.c_str());//change string to TCHAR *
			SendMessage(name,WM_SETTEXT,NULL,(LPARAM)tab); //Set name of second user			
			frnd->chatedit=chat;
			client.addtalktohwndtab(hDlg);
			delete tab;
			return (INT_PTR)FALSE;
		}
		break;
	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDC_TALK_CANCEL:
			case IDCANCEL:
			case ID_EXIT:				
			case client.constid:
				{
					EndDialog(hDlg, LOWORD(wParam));
					ShowWindow(client.boxmodeless,SW_HIDE);	
					client.hideMessagess(frnd);//set hide option messagess to make possibility to display them all again with open chat window
					client.newstateofnewMessage(true);
					frnd->chatedit=NULL;
					client.setnewvalueinhwndtab(hDlg,NULL);									
					return (INT_PTR)TRUE;
				}
				break;
			case IDC_TALK_SEND:
				{
					GetWindowText(sendbox,(LPWSTR)client.buffer,GetWindowTextLength(sendbox)+1);	
					char * data=client.castedchartochar((char *)client.buffer);
					if(GetWindowTextLength(sendbox)!=0)
					{
						*client.time=time(client.time);	//get time from computer
						class Message * msg=new class Message(client.User->name,client.actual,(string)data,ctime(client.time),0);
						client._send(msg,client.User->socketClient); //send message to server
						client.addMessagetolist(client.findFriend(client.User->contacts,msg->to),msg); //add message to list in memory
						delete msg;
						SendMessage(sendbox,WM_SETTEXT,NULL,(LPARAM)L""); //set empty text in edit control
						client.showMessages(chat,frnd); //show messagess in chat edit control
					}
					delete data;
				}
				break;			
			}
	default:		
		if(client.stateofnewMessage())
			client.showMessages(chat,frnd);		//show undisplayed messagessclient.menu
		}
		return (INT_PTR)FALSE;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Menu(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (Message)
	{
	case WM_INITDIALOG:		
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch((WPARAM)wParam)
		{
		case IDC_MAIN_CONTACTS:
			{	
				
				ShowWindow(hDlg,SW_HIDE);
				DialogBox(client.hInst, MAKEINTRESOURCE(IDC_CONTACTS), hDlg, Contacts); //create dialog box with loaded contacts
				ShowWindow(hDlg,SW_SHOW);
			}
			break;
		case IDC_MAIN_RUN:
			{	
				GetWindowText(GetDlgItem(hDlg,IDC_MAIN_NAME),(LPWSTR)client.buffer,GetWindowTextLength(GetDlgItem(hDlg,IDC_MAIN_NAME))+1);
				if(GetWindowTextLength(GetDlgItem(hDlg,IDC_MAIN_NAME))!='\0')
				{
					if(client._connect(hDlg,WM_SOCKET)==false) //if connection is not set
						break;
					char * data=client.castedchartochar((char *)client.buffer);
					client.User->name=(string)data;
					delete data;
					client._sendUserdatatoserver(client.User,client.User->socketClient); //send info of User to server
					//change apperance of dialogbox
					ShowWindow(GetDlgItem(hDlg,IDC_MAIN_STOP),SW_SHOW);
					ShowWindow(GetDlgItem(hDlg,IDC_MAIN_CONTACTS),SW_SHOW);
					ShowWindow(GetDlgItem(hDlg,IDC_MAIN_RUN),SW_HIDE);	
				}
				else
				{
					MessageBox(GetDlgItem(hDlg,IDC_MAIN_NAME),(LPCWSTR)L"Podaj jakas nazwe!",(LPCWSTR)L"Blad",MB_ICONERROR);
					break;
				}
			}
			break;
		case IDC_MAIN_STOP:
			{ //when user press button "STOP"
				ShowWindow(GetDlgItem(hDlg,IDC_MAIN_STOP),SW_HIDE);
				ShowWindow(GetDlgItem(hDlg,IDC_MAIN_RUN),SW_SHOW);
				ShowWindow(GetDlgItem(hDlg,IDC_MAIN_CONTACTS),SW_HIDE);
				//close connection and socket
				client.clear();
				client.initialize(); //once the socket is closed, all procedures must be done again
			}
			break;
		case ID_INFO:
			{	//when user press menu INFO			
				client.boxmodeless = CreateDialog(client.hInst, MAKEINTRESOURCE(IDC_INFO), hDlg, Info);
				ShowWindow(client.boxmodeless,SW_SHOW);
			}
			break;
		case ID_USTAWIENIA:
			{
				DialogBox(client.hInst, MAKEINTRESOURCE(IDC_SETTINGS), hDlg, Settings); //show dialobox with settings
			}
			break;
		case IDCANCEL:
		case ID_EXIT:
			PostQuitMessage(WM_QUIT);	
			return (INT_PTR)TRUE;		
		}

	case WM_SOCKET:
		switch(WSAGETSELECTEVENT(lParam))
		{
		case FD_READ:
			{ //get message from internet
				class Message * msg=NULL;
				msg=client._recive(client.User->socketClient);
				if(msg!=NULL)
				{
					client.action(msg,client.actualedit);
					if(msg->from=="User_exists")
					{
						ShowWindow(GetDlgItem(hDlg,IDC_MAIN_STOP),SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,IDC_MAIN_RUN),SW_SHOW);
						ShowWindow(GetDlgItem(hDlg,IDC_MAIN_CONTACTS),SW_HIDE);
						//close connection and socket
						client.clear();
						client.initialize(); //once the socket is closed, all procedures must be done again
					}
				}
				delete msg;
				msg=NULL;
				client.readpipetab(client.actualedit);
			}
			break;
		case FD_CLOSE:
			{	//close connection											
				SendMessage(client.contactshwnd,WM_COMMAND,client.constid,NULL);
				_Error er(NULL,L"Stracono placzenie z serwerem");
				SendMessage(hDlg,WM_COMMAND,IDC_MAIN_STOP,NULL);	
			}
			break;
		}
		return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}


