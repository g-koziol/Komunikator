#include "stdafx.h"
Message::Message(): 
	from("00000000"), 
	to("00000000"), 
	data("WIADOMOSC TESTOWA"), 
	time("00/00/0000"), 
	status(0), 
	visible(false)
{
	;
}

Message::Message(string a,string b, string c,string d, int i):
	from(a),
	to(b),
	data(c),
	time(d),
	status(i), 
	visible(false)
{
	name=from + " " + to + " " + data + " ";
	if(visible)
		name+="true ";
	else name+="false ";
	name+=time;
}

Message::~Message()
{
	;
}