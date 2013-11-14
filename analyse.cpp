/*
 *
 * 字符解析：先把输入命令规范化，变成输入流，然后再逐词将其解析。
 *
 */
#include "analyse.h"
#include "IOstream.h"

/* 全局表指针,方便输出以及函数间参数交流 */
TableNode *Gtable;   

/* 规范化的输入流 */
istringstream input;

bool GlobalError  = true;

extern TableTree BinaryTree;


/* 错误信息输出函数 */
void Error(const char *str, int how)
{
	if (Gtable)
	{
		delete Gtable;
		Gtable = NULL;
	}
	
	if (how == 0)
	{
		cout << "ERROR: 无命令输入" << endl;
		cout << "usage : 请输入命令" << endl;
	}
	else if (how == 1)
	{
		cout << "ERROR: 命令 \"" << str << "\" 找不到" << endl;
		cout << "usage : 命令必须以 select/insert/delete 开始" << endl;
	}
	else if (how == 3)
	{
		cout << "ERROR: \"select\" 语句中必须有 \"from\"" << endl;
	}
	else if (how == 5)
	{
		cout << "ERROR: \"inner\" 后必须有 \"join\"" << endl;
	}
	else if (how == 6)
	{
		cout << "ERROR: \"join\" 后必须有 \"on\" 语句" << endl;
	}
	else if (how == 7)
	{
		cout << "ERROR: join 命令只支持 \"=\"" << endl;
	}
	else if (how == 8)
	{
		cout << "ERROR: join 命令需要内连接的表" << endl;
	}
	else if (how == 9)
	{
		cout << "ERROR: on 语句后面要有两个表" << endl;
	}
	else if (how == 10)
	{
		cout << "ERROR: \"order\" 后面必须有 \"by\"" << endl;
	}
	else if (how == 11)
	{
		cout << "ERROR: inner_join 命令错误结束" << endl;
	}
	else if (how == 12)
	{
		cout << "ERROR: where 命令错误结束" << endl;
	}
	else if (how == 13)
	{
		cout << "ERROR: as 不能作为 select 的第一个参数" << endl;
	}
	else if (how == 14)
	{
		cout << "ERROR: " << str << " 命令不能作为 order 的第一个参数" << endl;
	}
	else if (how == 15)
	{
		cout << "ERROR: where 的参数错误" << endl;
	}
	else if (how == 16)
	{
		cout << "ERROR: where 语句需要逻辑比较关系" << endl;
	}
	else if (how == 17)
	{
		cout << "ERROR: where 语句需要值" << endl;
	}
	else if (how == 18)
	{
		cout << "ERROR: update 命令后面要有 set" << endl;
	}
	else if (how == 19)
	{
		cout << "ERROR: update 命令不能用关系符 \"" << str << "\"运行" << endl;
	}
	else if (how == 20)
	{
		cout << "ERROR: update 命令中要有值" << endl;
	}
	else if (how == 21)
	{
		cout << "ERROR: delete 命令必须后面接着 from" << endl;
	}
	else if (how == 22)
	{
		cout << "ERROR: delete 命令缺失表名" << endl;
	}
	else if (how == 23)
	{
		cout << "ERROR: delete 命令错误结束" << endl;
	}
	else if (how == 24)
	{
		cout << "ERROR: insert 后面必须要有 into" << endl;
	}
	else if (how == 25)
	{
		cout << "ERROR: insert 命令运行需要表名" << endl;
	}
	else if (how == 26)
	{
		cout << "ERROR: insert 命令缺失值" << endl;
	}
	else if (how == 27)
	{
		cout << "ERROR: insert 命令中列的数量不等于值的数量" << endl;
	}
	else if (how == 28)
	{
		cout << "ERROR: from 子语句后面缺少表" << endl;
	}
	else if (how == 29)
	{
		cout << "ERROR: 没有任何表存在" << endl;
	}
	else if (how == 30)
	{
		cout << "ERROR: 引号不对称！" << endl;
	}
	else if (how == 31)
	{
		cout << "不存在要查询的表 "  << str << endl;
	}
}
void FindError(const char *str)
{
	if (BinaryTree.mainRoot == NULL)
		Error(NULL, 29);
	else
		Error(str, 31);
}

/* 对主命令进行分析 */
void route()
{
	string command;
	input >> command;
	if (!_stricmp(command.c_str(), "SELECT"))
	{
		if (run_select())
		{
			cout << "查询结果为：" << endl;
			print(Gtable);
			delete Gtable;
			Gtable = NULL;
		}
	}
	else if (!_stricmp(command.c_str(), "INSERT"))
	{
		run_insert();
	}
	else if (!_stricmp(command.c_str(), "UPDATE"))
	{
		run_update();
	}
	else if (!_stricmp(command.c_str(), "DELETE"))
	{
		run_delete();
	}
	else if (!_stricmp(command.c_str(), ";"))
		Error(NULL, 0);
	else
		Error(command.c_str(), 1);
}

/* 对命令进行规范化, 给一个命令（以分号结尾）,返回一个规范化的命令 */
bool normalstring(string &command)
{
	unsigned int pos = 0, newpos;
	command.append(" ;");
	while ((newpos = command.find_first_of("!()<>=\n,\'", pos, 9)) != string::npos)
	{
		if (command[newpos] == '\n' || command[newpos] == ',')
			command[newpos] = ' ';
		else if (command[newpos] == '\'')//找两个相邻的引号
		{
			if (newpos >= 1 && command[newpos - 1] != ' ')
				command.insert(newpos++, 1, ' ');
			++newpos;
			//左引号
			while (command[newpos] != '\'')
			{
				if (command[newpos] == ' ')
					command[newpos] = '\'';
				else if(command[newpos] == ';')
				{
					Error(NULL, 30);
					return false;
				}
				++newpos;
			}
			//对称的引号
			++newpos;
			if (command[newpos] != ' ')
				command.insert(newpos, 1, ' ');
		}
		else
		{
			if (newpos > 0 && command[newpos - 1] != ' ')
				command.insert(newpos++, 1, ' ');
			if (command[newpos] == '<' || command[newpos] == '>' || command[newpos] == '!')
			{
				if (command[newpos + 1] != ' ')
				{
					if (command[newpos + 1] == '=')
					{
						if (command[newpos + 2] != ' ')
						{
							newpos += 2;
							command.insert(newpos, 1, ' ');
						}
						else
							++newpos;
					}
					else
						command.insert(++newpos, 1, ' ');
				}
			}
			else if (command[newpos] == '(' || command[newpos] == ')' || command[newpos] == '=')
			{
				if (command[newpos + 1] != ' ')
					command.insert(++newpos, 1, ' ');
			}
		}
		pos = newpos + 1;
	}

	return true;
}

/* 逻辑关系变成数字 */
int char2int(const string &ch)
{
	if (!strcmp(ch.c_str(), "<"))
		return 0;
	else if (!strcmp(ch.c_str(), "=") || !_stricmp(ch.c_str(), "IN"))//in操作不知道做的对不对
		return 1;
	else if (!strcmp(ch.c_str(), ">"))
		return 2;
	else if (!strcmp(ch.c_str(), ">="))
		return 3;
	else if (!strcmp(ch.c_str(), "!="))
		return 4;
	else if (!strcmp(ch.c_str(), "<="))
		return 5;
	else if (!strcmp(ch.c_str(), "("))
		return LEFT;
	else if (!strcmp(ch.c_str(), ")") || !strcmp(ch.c_str(), ";"))
		return END;
	else if (!_stricmp(ch.c_str(), "LIKE"))//where中支持模糊查询，暂时还是不要做吧
		return 8;
	else
		return -1;//错误处理
}

/*
 * analyse_select: 解析select字句的函数,输入直到找到from
 * 仅被run_select函数调用
 * 读入成功返回true，读入错误返回false
 */
bool analyse_select(Scharp *selectrow, Scharp *newrow, bool &distinct, bool &as_)
{
	string temp;
	input >> temp;
	if (!_stricmp(temp.c_str(), "DISTINCT"))
	{
		distinct = true;
		input >> temp;
	}

	if (!strcmp(temp.c_str(), "*"))
	{
		input >> temp;
		if (_stricmp(temp.c_str(), "FROM"))
		{
			Error(NULL, 3);
			return false;
		}
	}
	else
	{
		while (1)
		{
			if (temp[0] == ';')
			{
				Error(NULL, 3);
				return false;
			}
			else if (!_stricmp(temp.c_str(), "FROM"))
				break;
			else if (!_stricmp(temp.c_str(), "AS"))
			{
				if (selectrow->len == 0)
				{
					Error(NULL, 13);
					return false;
				}
				else
				{
					as_ = true;
					input >> temp;
					newrow->changetop(temp);
				}
			}
			else
			{
				selectrow->insert(temp);
				newrow->insert(temp);
			}
			input >> temp;
		}
	}
	return true;
}
/*
 * analyse_from: 解析from字句的函数,输入直到找到下一个关键字
 * 仅被run_select函数调用
 * 读入成功返回true，读入错误返回false
 */
bool analyse_from(Scharp *fromtable, bool &inner_join, bool &where_, bool &order_by)
{
	string temp;
	while (input >> temp)
	{
		if (char2int(temp) == END)
			break;
		else if (!_stricmp(temp.c_str(), "INNER"))
		{
			//检查inner join语句的完整性
			input >> temp;
			if (!_stricmp(temp.c_str(), "JOIN"))
			{
				inner_join = true;
				break;
			}
			Error(NULL, 5);
			return false;
		}
		else if (!_stricmp(temp.c_str(), "WHERE"))
		{
			where_ = true;
			break;
		}
		else if (!_stricmp(temp.c_str(), "ORDER"))
		{
			//检查ORDER语句的完整性
			input >> temp;
			if (!_stricmp(temp.c_str(), "BY"))
			{
				order_by = true;
				break;
			}
			Error(NULL, 10);
			return false;
		}
		else
			fromtable->insert(temp);
	}
	if (fromtable->len == 0)
	{
		Error(NULL, 28);
		return false;
	}
	return true;
}
/*
 * analyse_inner：解析inner_join字句的函数
 * 仅被run_select调用
 * 输入错误返回false，输入正确返回true
 */
bool analyse_inner(string &jointable, string &join1, string &join2, bool &where_, bool &order_by)
{
	string temp;
	//inner_join 的table
	input >> jointable;
	if (char2int(jointable) == END)
	{
		Error(NULL, 8);
		return false;
	}
	//检查on
	input >> temp;
	if (_stricmp(temp.c_str(), "ON"))
	{
		Error(NULL, 6);
		return false;
	}
	//解析on
	input >> join1;
	if (char2int(join1) == END)
	{
		Error(NULL, 9);
		return false;
	}
	input >> temp;
	if (_stricmp(temp.c_str(), "="))//on应该只有=吧？
	{
		Error(NULL, 7);
		return false;
	}
	input >> join2;
	if (char2int(join2) == END)
	{
		Error(NULL, 9);
		return false;
	} 
	//tail.....
	input >> temp;
	if (char2int(temp) == END)
		return true;
	else if (!_stricmp(temp.c_str(), "WHERE"))
		where_ = true;
	else if (!_stricmp(temp.c_str(), "ORDER"))
	{
		//检查ORDER语句的完整性
		input >> temp;
		if (!_stricmp(temp.c_str(), "BY"))
			order_by = true;
		else
		{
			Error(NULL, 10);
			return false;
		}
	}
	else
	{
		Error(temp.c_str(), 1);
		return false;
	}
	return true;
}
/*
 * analyse_where: 解析where子句的函数
 * 在select和delete、update语句中可能要用到
 * 支持嵌套/and/or运算
 * 输入错误返回false，输入正确返回true
 * 栈运算的方式：操作符=、<、>、<=、>=，值即为表，并且支持括号嵌套（以下内容需要重新改写！）
 */
bool analyse_where(TableNode* &tablep, bool &order_by)
{
	string temp, row;
	int relative;

	stack <TableNode*> Argv;   //参数栈
	stack <int> opt;     //运算符栈0:(),1:or,2:and
	
	while (1)
	{
		Scharp value;
		int isitem = 0;
		//row
		input >> temp;
		//结束
		if (!_stricmp(temp.c_str(), "ORDER"))
		{
			//检查ORDER语句的完整性
			input >> temp;
			if (!_stricmp(temp.c_str(), "BY"))
			{
				order_by = true;
				break;
			}
			Error(NULL, 10);
			return false;
		}
		else if (temp[0] == '(')
			opt.push(0);
		else if (char2int(temp.c_str()) == END)
		{
			//运算完所有
			bool flag = false;
			while (!opt.empty())
			{
				//去掉左括号
				if (opt.top() == 0)
				{
					opt.pop();
					flag = true;
					break;
				}
				TableNode *T1 = new TableNode(Argv.top());
				if (GlobalError == false)return false;
				Argv.pop();
				TableNode *T2 = new TableNode(Argv.top());
				if (GlobalError == false)return false;
				Argv.pop();
				//TODO:运算
				if (opt.top() == 1)
				{
					TableNode *T3 = Or(T1, T2);
					if (GlobalError == false)return false;
					Argv.push(T3);
				}
				else
				{
					TableNode *T3 = And(T1, T2);
					if (GlobalError == false)return false;
					Argv.push(T3);
				}
				opt.pop();
			}
			//没有找到左括号。运算应该结束了。。
			if (flag == false)
				break;
		}
		//运算符
		else if (!_stricmp(temp.c_str(), "AND") || !_stricmp(temp.c_str(), "OR"))
		{
			int newopt = 1;
			if (!_stricmp(temp.c_str(), "AND"))
				newopt = 2;
			while (!opt.empty() && opt.top() != 0 && opt.top() >= newopt)
			{
				TableNode *T1 = new TableNode(Argv.top());
				if (GlobalError == false)return false;
				Argv.pop();
				TableNode *T2 = new TableNode(Argv.top());
				if (GlobalError == false)return false;
				Argv.pop();
				//TODO:运算
				if (opt.top() == 1)
				{
					TableNode *T3 = Or(T1, T2);
					if (GlobalError == false)return false;
					Argv.push(T3);
				}
				else
				{
					TableNode *T3 = And(T1, T2);
					if (GlobalError == false)return false;
					Argv.push(T3);
				}
				opt.pop();
			}
			opt.push(newopt);
		}
		//运算对象
		else
		{
			//row
			row = temp;
			//ralative
			input >> temp;
			if (char2int(temp) == END)
			{
				Error(NULL, 15);
				return false;
			}
			relative = char2int(temp);
			if (relative == -1 || relative > 5)
			{
				Error(NULL, 15);
				return false;
			}
			//value
			input >> temp;
			if (char2int(temp) == END)
			{
				Error(NULL, 17);
				return false;
			}
			else if (char2int(temp) == LEFT)
			{
				input >> temp;
				//嵌套只支持select
				if (_stricmp(temp.c_str(), "SELECT"))
				{
					Error(NULL, 16);
					return false;
				}
				else if (run_select() == true)
				{
					//TODO:将栈顶的第一个表的唯一列元素全部拷贝到value中
					//循环value.insert从Gtable中循环拷贝
					for (DataNode * iNode = Gtable->head; iNode != NULL; iNode = iNode->next)
					{
						value.insert(string(iNode->data[0]));
					}
					delete Gtable;
					Gtable = NULL;
				}
				else
					return false;
			}
			else
			{
				if (valueclear(temp))
					isitem = 1;
				value.insert(temp);
			}
			//TODO：生成表，放入到栈Argv中
			TableNode *T3 = tablep->Where((char *)row.c_str(), value.head, isitem, value.len, relative % 3, relative / 3);
			if (GlobalError == false)return false;
			Argv.push(T3);
		}
	}

	while (!opt.empty())
	{
		
		TableNode *T1 = new TableNode(Argv.top());
		if (GlobalError == false)return false;
		Argv.pop();
		TableNode *T2 = new TableNode(Argv.top());
		if (GlobalError == false)return false;
		Argv.pop();
		//运算
		if (opt.top() == 1)
		{
			TableNode *T3 = Or(T1, T2);
			if (GlobalError == false)return false;
			Argv.push(T3);
		}
		else
		{
			TableNode *T3 = And(T1, T2);
			if (GlobalError == false)return false;
			Argv.push(T3);
		}
		opt.pop();
	}

	//取结果
	if (Gtable)
		delete Gtable;
	TableNode *T1 = new TableNode;
	T1->Init(Argv.top());
	if (GlobalError == false)return false;
	for (DataNode * iNode = Argv.top()->head; iNode != NULL; iNode = iNode->next) {
		DataNode * temp;
		temp = new DataNode;
		copyNode(temp, iNode);
		if (GlobalError == false)return false;
		T1->Insert(temp);
		if (GlobalError == false)return false;
	}
	Gtable = T1;
	
	delete tablep;
	tablep = T1;
	return true;
}
/*
 * analyse_order: 解析order子句的函数
 * 仅被run_select调用
 * 输入错误返回false，输入正确返回true
 */
bool analyse_order(Scharp *orderrow, int *orderway)
{
	string temp;
	while (input >> temp)
	{
		if (char2int(temp) == END)
			break;
		else if (!_stricmp(temp.c_str(), "DESC"))
		{
			if (orderrow->len == 0)
			{
				Error(temp.c_str(), 14);
				return false;
			}
			orderway[orderrow->len - 1] = 1;
		}
		else if (!_stricmp(temp.c_str(), "ASC"))
		{
			if (orderrow->len == 0)
			{
				Error(temp.c_str(), 14);
				return false;
			}
		}
		else
			orderrow->insert(temp);
	}
	return true;
}

/*
 * run_select：解析并运行查询的函数
 * 其中select/from是在语法上必须出现的，其他语句可选则出现
 * 解析顺序：select、from、inner join、on、where、order by
 * 执行顺序：from、inner join on、where、order by、select
 */
bool run_select()
{
	bool inner_join = false;
	bool where_ = false;
	bool order_by = false;
	bool distinct = false;
	bool as_ = false;

	/* select子句:输入直到找到from */
	Scharp *selectrow = new Scharp, *newrow = new Scharp;//[0, rowsum)
	
	if (analyse_select(selectrow, newrow, distinct, as_) == false)
		return false;
	
	
	/* from子句：输入直到找到下一个关键字 */
	Scharp *fromtable = new Scharp;//[0, tablesum)
	if (analyse_from(fromtable, inner_join, where_, order_by) == false)
		return false;


	TableNode * VT, * tempT, *tempT1, *tempT2;//运行中必须的虚拟表
	
	//TODO:1、叉乘
	if (fromtable->len > 1)
	{
		if ((tempT1 = BinaryTree.FindTable((fromtable->head)[0])) == NULL)
		{
			FindError(fromtable->head[0]);
			return false;
		}
		if ((tempT2 = BinaryTree.FindTable((fromtable->head)[1])) == NULL)
		{
			FindError(fromtable->head[1]);
			return false;
		}
		VT = From(tempT1, tempT2);
		if (GlobalError == false)return false;
		TableNode* tempT = VT;
		for (int i = 2; i < fromtable->len; ++i)
		{
			if ((tempT1 = BinaryTree.FindTable((fromtable->head)[i])) == NULL)
			{
				FindError(fromtable->head[i]);
				return false;
			}
			VT = From(tempT, tempT1);
			if (GlobalError == false)return false;
			delete tempT;
			tempT = VT;
		}
	}
	else if (inner_join == false)
	{
		if ((tempT = BinaryTree.FindTable((fromtable->head)[0])) == NULL)
		{
			FindError(fromtable->head[0]);
			return false;
		}
		VT = From(tempT, NULL);
		if (GlobalError == false)return false;
	}


	/* inner join子句：输入完inner_join的命令之后再检查尾部 */
	string jointable;
	string join1, join2;
	if (inner_join)
	{
		if (analyse_inner(jointable, join1, join2, where_, order_by) == false)
			return false;

		//TODO:2、筛选
		Scharp ss;
		ss.insert(join2);
		if ((tempT1 = BinaryTree.FindTable(fromtable->head[0])) == NULL)
		{
			FindError(fromtable->head[0]);
			return false;
		}
		if ((tempT2 = BinaryTree.FindTable((char *)jointable.c_str())) == NULL)
		{
			FindError(jointable.c_str());
			return false;
		}
		VT = From(tempT1, tempT2);
		if (GlobalError == false)return false;
		tempT = VT;
		VT = VT->Where((char *)join1.c_str(), ss.head, 1, 1, 1, 0);
		if (GlobalError == false)return false;
		delete tempT;
		
	}


	/* where子句 */
	if (where_)
	{
		//TODO:3、where筛选
		if (analyse_where(VT, order_by) == false)
			return false;
	}

	//TODO:4、select切列生成新表VT = （VT，selectrow）
	if (selectrow->len != 0)
	{
		tempT = VT;
		VT = VT->Select(selectrow->len, selectrow->head);
		if (GlobalError == false)return false;
		delete tempT;
	}
	

	/* order子句 */
	Scharp *orderrow = new Scharp;//[0, ordersum)
	int orderway[MAXLINE];//TODO:-1为默认升序，1为降序？这个不具有无限长的鲁棒性啊。。。。怎么办
	if (order_by)
	{
		memset(orderway, -1, sizeof(orderway));//能么？
		if (analyse_order(orderrow, orderway) == false)
			return false;

		//TODO:5、排序
		tempT = VT;
		VT = VT->OrderBy(orderrow->len, orderrow->head, orderway);
		if (GlobalError == false)return false;
		delete tempT;
	}

	//TODO:6、AS操作
	if (as_)
	{
		VT->As(newrow->head, NULL, NULL);
		if (GlobalError == false)return false;
	}
	//改名结束

	if (distinct)
	{
		//如果有distinct再去除相同的行
		VT->Distinct();
		if (GlobalError == false)return false;
	}

	/* 删除临时字符串 */
	delete newrow;
	delete selectrow;
	delete fromtable;
	delete orderrow;
	
	//结果放入全局栈
	Gtable = VT;
	return true;
}
/*
 * run_insert：解析并运行插入的函数
 * 其中insertinto/values是在语法上必须出现的，其他语句可选则出现
 * 解析顺序：into、value
 * 执行顺序：直接读完就插入了
 */
void run_insert()
{
	string temp;
	input >> temp;
	if (_stricmp(temp.c_str(), "INTO"))
	{
		Error(NULL, 24);
		return;
	}

	/* 读表的名字 */
	string table;
	input >> table;
	if (char2int(table) == END)
	{
		Error(NULL, 25);
		return;
	}

	Scharp *rowname = new Scharp, *value = new Scharp;
	
	/* 读入列的名字 */
	input >> temp;
	if (char2int(temp) == END)
	{
		Error(NULL, 26);
		return;
	}
	else if (char2int(temp) == LEFT)
	{
		while (1)
		{
			input >> temp;
			if (!strcmp(temp.c_str(), ")"))
				break;
			else if (!strcmp(temp.c_str(), ";"))
			{
				Error(NULL, 26);
				return;
			}
			else
				rowname->insert(temp);
		}
		input >> temp;
	}

	
	/* 读入value */
	if(_stricmp(temp.c_str(), "VALUES"))
	{
		Error(NULL, 26);
		return;
	}
	else 
	{
		//左括号
		input >> temp;
		if (char2int(temp) != LEFT)
		{
			Error(NULL, 26);
			return;
		}
		
		while (1)
		{
			input >> temp;
			if (!strcmp(temp.c_str(), ")"))
				break;
			else if (!strcmp(temp.c_str(), ";"))
			{
				Error(NULL, 26);
				return;
			}
			else
			{
				valueclear(temp);
				value->insert(temp);
			}
		}

		//检查数量匹配
		if (rowname->len != 0 && rowname->len != value->len)
		{
			Error(NULL, 27);
			return;
		}
	}
	
	TableNode *VT = BinaryTree.FindTable((char *)table.c_str());
	if (VT == NULL)
	{
		FindError(table.c_str());
		return;
	}
	
	if (rowname->len == 0)
	{
		for (int i = 0; i < VT->itemNum; i++)
		{
			rowname->insert(string(VT->itemName[i]));
		}
	}
	
	/* 进行插入 */
	
	//TODO:
	VT->insertNode(rowname->head[0], ParaTrans(VT, rowname->len, rowname->head, value->head));
	
	delete rowname;
	delete value;
	if (GlobalError == false)
		cout << "插入失败" <<endl;
	else
		cout << "插入成功" << endl;
}
/*
 * run_delete：解析并运行删除的函数
 * 其中deletefrom/where是在语法上必须出现的，其他语句可选则出现
 * 解析顺序：deletefrom 表的名字、where 条件
 * 执行顺序：from、where
 */
void run_delete()
{
	string temp;
	input >> temp;
	if (_stricmp(temp.c_str(), "FROM"))
	{
		Error(NULL, 21);
		return;
	}

	string table;
	input >> table;
	if (char2int(table) == END)
	{
		Error(NULL, 22);
		return;
	}


	//TODO:1、生成一个暂时的拷贝表
	TableNode *VT, *tempT;
	if ((tempT = BinaryTree.FindTable((char *)table.c_str())) == NULL)
	{
		FindError(table.c_str());
		cout << "删除失败" << endl;
		return;
	}
	VT = From(tempT, NULL);
	if (GlobalError == false)
	{
		cout << "删除失败" << endl;
		return ;
	}

	/* 检查是否有where语句 */
	input >> temp;
	if (!_stricmp(temp.c_str(), "WHERE"))//没有where就是删除全表，但是保留表名
	{
		//2、where筛选
		bool non;
		if (analyse_where(VT, non) == false)
			return;
	}
	else if (char2int(temp.c_str()) != END)
	{
		Error(NULL, 23);
		return;
	}
	

	/* 运行delete */

	//TODO:2、调用delete删除table中与VT相同的条目
	TableNode * TB = BinaryTree.FindTable((char *)table.c_str());
	if (TB == NULL)
	{
		FindError(table.c_str());
		cout << "删除失败" << endl;
		return;
	}
	TB->deleteNode(VT);
	if (GlobalError == false)
	{
		cout << "删除失败" << endl;
		return;
	}
	delete VT;
	cout << "删除成功" << endl;
}
/*
 * run_update：解析并运行更新的函数
 * 其中update/set/where是在语法上必须出现的，其他语句可选则出现
 * 解析顺序：update、set、where
 * 执行顺序：where、set
 */
void run_update()
{
	string temp;
	string table;
	
	/* 读入表的名字 */
	input >> table;
	if (char2int(table) == END)
		return;
	
	//TODO:1、生成新表
	TableNode *VT, *tempT;
	tempT = BinaryTree.FindTable((char *)table.c_str());
	if (tempT == NULL)
	{
		FindError(table.c_str());
		cout << "更新失败" << endl;
		return;
	}
	VT = From(tempT, NULL);
	if (GlobalError == false)
	{
		cout << "更新失败" << endl;
		return;
	}

	/* 检查是否符合update的格式 */
	input >> temp;
	if (_stricmp(temp.c_str(), "SET"))
	{
		Error(NULL, 18);
		return;
	}
	
	bool where_ = false;
	Scharp *rowname = new Scharp, *value = new Scharp;
	/* 读入列名称和新值 */
	while (1)
	{
		//读入列名
		input >> temp;
		if (char2int(temp) == END)
			break;
		else if (!_stricmp(temp.c_str(), "WHERE"))
		{
			where_ = true;
			break;
		}
		rowname->insert(temp);
		//读入等号
		input >> temp;
		if (strcmp(temp.c_str(), "="))
		{
			Error(temp.c_str(), 19);
			return;
		}
		//读入值
		input >> temp;
		if (char2int(temp) == END)
		{
			Error(NULL, 20);
			return;
		}
		valueclear(temp);
		value->insert(temp);
	}


	/* 解析where语句 */ 
	if (where_)
	{
		//2、where筛选
		bool non;
		if (analyse_where(VT, non) == false)
			return;
	}
	

	/* 开始更新 */
    //TODO：更新
	TableNode * TB = BinaryTree.FindTable((char *)table.c_str());
	if (TB == NULL)
	{
		FindError(table.c_str());
		cout << "更新失败" << endl;
		return;
	}
	TB->updateNode(VT, rowname->len, rowname->head, value->head);
	if (GlobalError == false)
	{
		cout << "更新失败" << endl;
		return;
	}
	
	delete rowname;
	delete value;
	delete VT;
	cout << "更新成功" << endl;
}