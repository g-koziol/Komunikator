
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#pragma comment(lib, "Ws2_32.lib")

#define _CRT_SECURE_NO_WARNINGS
#define LOCAL_IP "127.0.0.1"
void deletepipe(LPARAM ** tab);
using namespace std;


///////////////////////////////////////////////////
//template
template <class TYPE>
class Dump
{
	string name;
public:
	Dump(TYPE tmp): name(tmp->name){};
	friend void operator<<(ofstream & stream,Dump & dumpy)
	{
		stream << dumpy.name;
	}
};

/////////////////////////////////////////////////////
//execptions classess
class DLL_API _Error
{
private:
	HWND tmphwnd;	
	time_t time;
	
public:
	ofstream * file;

	_Error(const _Error &);
	_Error(HWND);
	_Error(HWND,TCHAR *);
	_Error(string);	
	~_Error();

	inline bool starterror(void);
	void writetofile(string);
	void writetohwnd(TCHAR *);
	void general(void);
	void SetTmphwnd(HWND h)
	{
		tmphwnd=h;
	}
};

class DLL_API ErrorSocket
	: public _Error
{
	char * buf;
	int number;
	TCHAR * tstr;
public:
	ErrorSocket(HWND,int);
	~ErrorSocket(void);

	void SetNumber(int x){ number=x; }	
	bool checknumber();
};

class DLL_API MemoryDump
	 : public _Error
{
	class Message ** msg;
	class Friend ** frnd;
	class User ** users;
	HWND hwnd;
	
	void writemessages(class Message **);
	void writefriends(class Friend **);
	void writeusers(class User **);
public:
	MemoryDump(class Message ** m) : msg(m), frnd(NULL), users(NULL), _Error(NULL) {};
	MemoryDump(class Friend ** f) : msg(NULL), frnd(f), users(NULL), _Error(NULL) {};
	MemoryDump(class User ** u) : msg(NULL), frnd(NULL), users(u), _Error(NULL) {} ;

	void writememory();
};
////////////////////////////////////////////classess used as structs

class DLL_API Message
{
	
public:
	string from;
	string to;
	string data;
	string time;
	int status;
	bool visible;

	string name;

	Message();
	Message(string a,string b, string c,string d, int i);
	~Message();
};

class DLL_API Friend
{
public:
	string name;
	class Message ** talk;
	HWND chatedit;

	Friend();
	Friend(string);
	~Friend();
};
class DLL_API User
{
public:
	string name; 
	string number;
	string password;
	string fileName;
	SOCKET socketClient;
	SOCKADDR_IN socketaddrClient;
	int status;
	class Friend ** contacts;

	User();
	~User();
};

///////////////////////////////////////////////////////main classess

class DLL_API Pipe 
{
public:
	char * error;
	int nError;
	TCHAR * terror;
	HWND EditScreen;
	HWND ** tabhwndtalk;
	HWND contactshwnd;
	LPARAM ** _msg;
	char * teksttime;
	tm * ptr;
	time_t czas;
	static const int constid=9999;

	Pipe(HWND a);
	~Pipe();
	TCHAR * chartotchar(const char *);
	void addpipetab(LPARAM text);
	void addtabtopipetab(LPARAM ** tab);
	void readpipetab(HWND handle);
	void apendtoedit(HWND handle, LPARAM text);
	void showMessage(LPARAM text);
	void deletehwndtalkpointers(HWND **);
	void endmanytalkdialogbox(void);
	void addtalktohwndtab(HWND hwnd);
	void setnewvalueinhwndtab(HWND hDlg,HWND tmp);

};

class DLL_API Files :
	public Pipe
{
public:
	fstream ** file;
	string ** names;

	Files(HWND a);
	~Files(void);
	/*
	User ** _opens();//otwarcie plików dla serwera
	void _openc();//otwracie plików dla klienta
	void _openceach(string ** tab);//open each file for Client
	void _openseach(fstream * l);//open each file for Server
	void safes(void); //save files for Server
	void safec(User *); //save files for Client
	void _closes(void); //close files for Server
	void _closec(void); //close files for Client
	*/
};

class DLL_API Connection : public Files
{
public:
	SOCKET mainSocket;
	WSADATA wsaData;
	SOCKADDR_IN socketAddress;
	SOCKET newsocket;
	DWORD addr;
	int nResult;
	int port;
	char * szHostName;
	char * szLocalIP;

	Connection(HWND a);
	~Connection(void);
	void _endconnection(SOCKET);
	void _send(Message *, SOCKET);	
	Message * _recive(SOCKET);	
	string setip(BYTE a,BYTE b,BYTE c,BYTE d);
};

class DLL_API Data : 
	public Connection
{
public:

	HINSTANCE hInst;
	time_t * time;
	string stime;
	Data(HWND a);
	~Data(void);	
	char * castedchartochar(char *);
	void addMessagetolist(Friend * ,Message *);
	void showMessages(HWND Edit,Friend * frnd );
	void hideMessagess(Friend * frnd);
	User * finddstUser(string,class User **);
	void _sendUserdatatoserver(class User *,SOCKET);
	User * _reciveUserdatafromUser(SOCKET,class User **);
	Friend * _getcontactfromserver(Message *);
	void _sendcontactsfromserver(User ** data, SOCKET dst);
};


class DLL_API Client : 
	public Data
{
protected:
	string number;
	bool newMessage;	

public:
	bool endedgettingcontacts;
	HWND actualedit;
	TCHAR * buffer;
	LPARAM state;
	HWND boxmodeless;
	string actual;
	int actualint;
	class User * User;	

	Client(HWND a);
	~Client(void);
	bool _connect (HWND &,int);
	void selectFriend(void);
	void action(Message *, HWND);
	bool stateofnewMessage()
	{ 
		return newMessage; 
	}
	void newstateofnewMessage(bool a)
	{ 
		newMessage=a; 
	}
	class Friend * findFriend(class Friend **,string name);
	void initialize(void);
	void clear(void);
	void startgettingcontactsfromserver(void);
	Message _recv(void);
};

class DLL_API Server :
	public Data
{
private:
	int nClient;
public:
	User ** data;

	Server(HWND a);
	~Server(void);
	void _connect(void);
	void action(class Message *);
	void start();
	User ** addUsertodata(class User *, SOCKET); //dodaje informacje nowego podlaczonego uzytkownika do tablicy wskaznikow na User
	void _sendfrompeertopeer(class Message *);	
	void deletesocket(int);
	int findclosedsocket(void);

};