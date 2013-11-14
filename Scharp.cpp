#include "analyse.h"

Scharp::Scharp()
{
	len = 0;
	head = new char *[32];
	maxlen = 32;
}

Scharp::~Scharp()
{
	for (int i = 0; i < len; ++i)
		delete []head[i];
	delete []head;
}

void Scharp::insert(const string &str)
{
	if (len == maxlen)
	{
		maxlen <<= 1;
		char **newhead = new char *[maxlen];
		for (int i = 0; i < len; ++i)
		{
			newhead[i] = new char [strlen(head[i]) + 1];
			strcpy(newhead[i], head[i]);
			delete []head[i];
		}
		delete []head;
		head = newhead;
	}
	head[len] = new char [str.length() + 1];
	strcpy(head[len], str.c_str());
	++len;
}


void Scharp::changetop(const string &str)
{
	if (str.length() > strlen(head[len - 1]))
	{
		delete []head[len - 1];
		head[len - 1] = new char [str.length() + 1];
	}
	strcpy(head[len - 1], str.c_str());
}


void Scharp::output()
{
	debug_cout << "len = " << len << endl; 
	for (int i = 0; i < len; ++i)
	{
		debug_cout << i << " : " << head[i] << endl;
	}
}