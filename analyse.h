#ifndef analyse_H_
#define analyse_H_

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <memory>
#include <stack>
#include "Database_ADT.h"
using namespace std;

#define debug
#ifdef debug
#define debug_cout cout
#else
#define debug_cout /##/
#endif

#define LEFT 6
#define END 7
#define MAXLINE 32


/*
 * ��װ���char**,��ȫ���ȶ�
 */
class Scharp
{
	public:
		int len;
		int maxlen;
		char **head;

		Scharp();
		~Scharp();

		void insert(const string &str);
		void changetop(const string &str);
		void output();
};

/*
 * ��װ������뺯��...trueΪ���falseΪ���ֻ���ֵ
 */
inline bool valueclear(string &str)
{
	if (str[0] == '\'' && str[str.length() - 1] == '\'')
	{
		str.erase(0, 1);
		str.erase(str.length() - 1, 1);
		for (unsigned int i = 0; i < str.length(); ++i)
		{
			if (str[i] == '\'')
				str[i] = ' ';
		}
	}
	else
	{
		for (int i = 0; i < str.length(); ++i)
		{
			if (!isdigit(str[i]))
				return true;
		}
	}
	return false;
}

/*
 * �����й淶�ʹ�������
 */
void route();
bool normalstring(string &command);
int char2int(char ch);
void Error(const char*str, int how);
void FindError(const char *);

/*
 * ÿ������ܵ����к���
 */
bool run_select();
void run_insert();
void run_delete();
void run_update();

/*
 * �����Ľ�������
 */
bool analyse_where(TableNode * &tablep, bool &order_by);
bool analyse_select(Scharp *selectrow, Scharp *newrow, bool &distinct, bool &as_);
bool analyse_from(Scharp *fromtable, bool &inner_join, bool &where_, bool &order_by);
bool analyse_inner(string &jointable, string &join1, string &join2, bool &where_, bool &order_by);
bool analyse_order(Scharp *orderrow, int *orderway);

#endif