/*
 *
 * �ַ��������Ȱ���������淶���������������Ȼ������ʽ��������
 *
 */
#include "analyse.h"
#include "IOstream.h"

/* ȫ�ֱ�ָ��,��������Լ�������������� */
TableNode *Gtable;   

/* �淶���������� */
istringstream input;

bool GlobalError  = true;

extern TableTree BinaryTree;


/* ������Ϣ������� */
void Error(const char *str, int how)
{
	if (Gtable)
	{
		delete Gtable;
		Gtable = NULL;
	}
	
	if (how == 0)
	{
		cout << "ERROR: ����������" << endl;
		cout << "usage : ����������" << endl;
	}
	else if (how == 1)
	{
		cout << "ERROR: ���� \"" << str << "\" �Ҳ���" << endl;
		cout << "usage : ��������� select/insert/delete ��ʼ" << endl;
	}
	else if (how == 3)
	{
		cout << "ERROR: \"select\" ����б����� \"from\"" << endl;
	}
	else if (how == 5)
	{
		cout << "ERROR: \"inner\" ������� \"join\"" << endl;
	}
	else if (how == 6)
	{
		cout << "ERROR: \"join\" ������� \"on\" ���" << endl;
	}
	else if (how == 7)
	{
		cout << "ERROR: join ����ֻ֧�� \"=\"" << endl;
	}
	else if (how == 8)
	{
		cout << "ERROR: join ������Ҫ�����ӵı�" << endl;
	}
	else if (how == 9)
	{
		cout << "ERROR: on ������Ҫ��������" << endl;
	}
	else if (how == 10)
	{
		cout << "ERROR: \"order\" ��������� \"by\"" << endl;
	}
	else if (how == 11)
	{
		cout << "ERROR: inner_join ����������" << endl;
	}
	else if (how == 12)
	{
		cout << "ERROR: where ����������" << endl;
	}
	else if (how == 13)
	{
		cout << "ERROR: as ������Ϊ select �ĵ�һ������" << endl;
	}
	else if (how == 14)
	{
		cout << "ERROR: " << str << " �������Ϊ order �ĵ�һ������" << endl;
	}
	else if (how == 15)
	{
		cout << "ERROR: where �Ĳ�������" << endl;
	}
	else if (how == 16)
	{
		cout << "ERROR: where �����Ҫ�߼��ȽϹ�ϵ" << endl;
	}
	else if (how == 17)
	{
		cout << "ERROR: where �����Ҫֵ" << endl;
	}
	else if (how == 18)
	{
		cout << "ERROR: update �������Ҫ�� set" << endl;
	}
	else if (how == 19)
	{
		cout << "ERROR: update ������ù�ϵ�� \"" << str << "\"����" << endl;
	}
	else if (how == 20)
	{
		cout << "ERROR: update ������Ҫ��ֵ" << endl;
	}
	else if (how == 21)
	{
		cout << "ERROR: delete ������������� from" << endl;
	}
	else if (how == 22)
	{
		cout << "ERROR: delete ����ȱʧ����" << endl;
	}
	else if (how == 23)
	{
		cout << "ERROR: delete ����������" << endl;
	}
	else if (how == 24)
	{
		cout << "ERROR: insert �������Ҫ�� into" << endl;
	}
	else if (how == 25)
	{
		cout << "ERROR: insert ����������Ҫ����" << endl;
	}
	else if (how == 26)
	{
		cout << "ERROR: insert ����ȱʧֵ" << endl;
	}
	else if (how == 27)
	{
		cout << "ERROR: insert �������е�����������ֵ������" << endl;
	}
	else if (how == 28)
	{
		cout << "ERROR: from ��������ȱ�ٱ�" << endl;
	}
	else if (how == 29)
	{
		cout << "ERROR: û���κα����" << endl;
	}
	else if (how == 30)
	{
		cout << "ERROR: ���Ų��Գƣ�" << endl;
	}
	else if (how == 31)
	{
		cout << "������Ҫ��ѯ�ı� "  << str << endl;
	}
}
void FindError(const char *str)
{
	if (BinaryTree.mainRoot == NULL)
		Error(NULL, 29);
	else
		Error(str, 31);
}

/* ����������з��� */
void route()
{
	string command;
	input >> command;
	if (!_stricmp(command.c_str(), "SELECT"))
	{
		if (run_select())
		{
			cout << "��ѯ���Ϊ��" << endl;
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

/* ��������й淶��, ��һ������ԷֺŽ�β��,����һ���淶�������� */
bool normalstring(string &command)
{
	unsigned int pos = 0, newpos;
	command.append(" ;");
	while ((newpos = command.find_first_of("!()<>=\n,\'", pos, 9)) != string::npos)
	{
		if (command[newpos] == '\n' || command[newpos] == ',')
			command[newpos] = ' ';
		else if (command[newpos] == '\'')//���������ڵ�����
		{
			if (newpos >= 1 && command[newpos - 1] != ' ')
				command.insert(newpos++, 1, ' ');
			++newpos;
			//������
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
			//�ԳƵ�����
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

/* �߼���ϵ������� */
int char2int(const string &ch)
{
	if (!strcmp(ch.c_str(), "<"))
		return 0;
	else if (!strcmp(ch.c_str(), "=") || !_stricmp(ch.c_str(), "IN"))//in������֪�����ĶԲ���
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
	else if (!_stricmp(ch.c_str(), "LIKE"))//where��֧��ģ����ѯ����ʱ���ǲ�Ҫ����
		return 8;
	else
		return -1;//������
}

/*
 * analyse_select: ����select�־�ĺ���,����ֱ���ҵ�from
 * ����run_select��������
 * ����ɹ�����true��������󷵻�false
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
 * analyse_from: ����from�־�ĺ���,����ֱ���ҵ���һ���ؼ���
 * ����run_select��������
 * ����ɹ�����true��������󷵻�false
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
			//���inner join����������
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
			//���ORDER����������
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
 * analyse_inner������inner_join�־�ĺ���
 * ����run_select����
 * ������󷵻�false��������ȷ����true
 */
bool analyse_inner(string &jointable, string &join1, string &join2, bool &where_, bool &order_by)
{
	string temp;
	//inner_join ��table
	input >> jointable;
	if (char2int(jointable) == END)
	{
		Error(NULL, 8);
		return false;
	}
	//���on
	input >> temp;
	if (_stricmp(temp.c_str(), "ON"))
	{
		Error(NULL, 6);
		return false;
	}
	//����on
	input >> join1;
	if (char2int(join1) == END)
	{
		Error(NULL, 9);
		return false;
	}
	input >> temp;
	if (_stricmp(temp.c_str(), "="))//onӦ��ֻ��=�ɣ�
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
		//���ORDER����������
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
 * analyse_where: ����where�Ӿ�ĺ���
 * ��select��delete��update����п���Ҫ�õ�
 * ֧��Ƕ��/and/or����
 * ������󷵻�false��������ȷ����true
 * ջ����ķ�ʽ��������=��<��>��<=��>=��ֵ��Ϊ������֧������Ƕ�ף�����������Ҫ���¸�д����
 */
bool analyse_where(TableNode* &tablep, bool &order_by)
{
	string temp, row;
	int relative;

	stack <TableNode*> Argv;   //����ջ
	stack <int> opt;     //�����ջ0:(),1:or,2:and
	
	while (1)
	{
		Scharp value;
		int isitem = 0;
		//row
		input >> temp;
		//����
		if (!_stricmp(temp.c_str(), "ORDER"))
		{
			//���ORDER����������
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
			//����������
			bool flag = false;
			while (!opt.empty())
			{
				//ȥ��������
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
				//TODO:����
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
			//û���ҵ������š�����Ӧ�ý����ˡ���
			if (flag == false)
				break;
		}
		//�����
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
				//TODO:����
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
		//�������
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
				//Ƕ��ֻ֧��select
				if (_stricmp(temp.c_str(), "SELECT"))
				{
					Error(NULL, 16);
					return false;
				}
				else if (run_select() == true)
				{
					//TODO:��ջ���ĵ�һ�����Ψһ��Ԫ��ȫ��������value��
					//ѭ��value.insert��Gtable��ѭ������
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
			//TODO�����ɱ����뵽ջArgv��
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
		//����
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

	//ȡ���
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
 * analyse_order: ����order�Ӿ�ĺ���
 * ����run_select����
 * ������󷵻�false��������ȷ����true
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
 * run_select�����������в�ѯ�ĺ���
 * ����select/from�����﷨�ϱ�����ֵģ���������ѡ�����
 * ����˳��select��from��inner join��on��where��order by
 * ִ��˳��from��inner join on��where��order by��select
 */
bool run_select()
{
	bool inner_join = false;
	bool where_ = false;
	bool order_by = false;
	bool distinct = false;
	bool as_ = false;

	/* select�Ӿ�:����ֱ���ҵ�from */
	Scharp *selectrow = new Scharp, *newrow = new Scharp;//[0, rowsum)
	
	if (analyse_select(selectrow, newrow, distinct, as_) == false)
		return false;
	
	
	/* from�Ӿ䣺����ֱ���ҵ���һ���ؼ��� */
	Scharp *fromtable = new Scharp;//[0, tablesum)
	if (analyse_from(fromtable, inner_join, where_, order_by) == false)
		return false;


	TableNode * VT, * tempT, *tempT1, *tempT2;//�����б���������
	
	//TODO:1�����
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


	/* inner join�Ӿ䣺������inner_join������֮���ټ��β�� */
	string jointable;
	string join1, join2;
	if (inner_join)
	{
		if (analyse_inner(jointable, join1, join2, where_, order_by) == false)
			return false;

		//TODO:2��ɸѡ
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


	/* where�Ӿ� */
	if (where_)
	{
		//TODO:3��whereɸѡ
		if (analyse_where(VT, order_by) == false)
			return false;
	}

	//TODO:4��select���������±�VT = ��VT��selectrow��
	if (selectrow->len != 0)
	{
		tempT = VT;
		VT = VT->Select(selectrow->len, selectrow->head);
		if (GlobalError == false)return false;
		delete tempT;
	}
	

	/* order�Ӿ� */
	Scharp *orderrow = new Scharp;//[0, ordersum)
	int orderway[MAXLINE];//TODO:-1ΪĬ������1Ϊ����������������޳���³���԰�����������ô��
	if (order_by)
	{
		memset(orderway, -1, sizeof(orderway));//��ô��
		if (analyse_order(orderrow, orderway) == false)
			return false;

		//TODO:5������
		tempT = VT;
		VT = VT->OrderBy(orderrow->len, orderrow->head, orderway);
		if (GlobalError == false)return false;
		delete tempT;
	}

	//TODO:6��AS����
	if (as_)
	{
		VT->As(newrow->head, NULL, NULL);
		if (GlobalError == false)return false;
	}
	//��������

	if (distinct)
	{
		//�����distinct��ȥ����ͬ����
		VT->Distinct();
		if (GlobalError == false)return false;
	}

	/* ɾ����ʱ�ַ��� */
	delete newrow;
	delete selectrow;
	delete fromtable;
	delete orderrow;
	
	//�������ȫ��ջ
	Gtable = VT;
	return true;
}
/*
 * run_insert�����������в���ĺ���
 * ����insertinto/values�����﷨�ϱ�����ֵģ���������ѡ�����
 * ����˳��into��value
 * ִ��˳��ֱ�Ӷ���Ͳ�����
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

	/* ��������� */
	string table;
	input >> table;
	if (char2int(table) == END)
	{
		Error(NULL, 25);
		return;
	}

	Scharp *rowname = new Scharp, *value = new Scharp;
	
	/* �����е����� */
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

	
	/* ����value */
	if(_stricmp(temp.c_str(), "VALUES"))
	{
		Error(NULL, 26);
		return;
	}
	else 
	{
		//������
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

		//�������ƥ��
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
	
	/* ���в��� */
	
	//TODO:
	VT->insertNode(rowname->head[0], ParaTrans(VT, rowname->len, rowname->head, value->head));
	
	delete rowname;
	delete value;
	if (GlobalError == false)
		cout << "����ʧ��" <<endl;
	else
		cout << "����ɹ�" << endl;
}
/*
 * run_delete������������ɾ���ĺ���
 * ����deletefrom/where�����﷨�ϱ�����ֵģ���������ѡ�����
 * ����˳��deletefrom ������֡�where ����
 * ִ��˳��from��where
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


	//TODO:1������һ����ʱ�Ŀ�����
	TableNode *VT, *tempT;
	if ((tempT = BinaryTree.FindTable((char *)table.c_str())) == NULL)
	{
		FindError(table.c_str());
		cout << "ɾ��ʧ��" << endl;
		return;
	}
	VT = From(tempT, NULL);
	if (GlobalError == false)
	{
		cout << "ɾ��ʧ��" << endl;
		return ;
	}

	/* ����Ƿ���where��� */
	input >> temp;
	if (!_stricmp(temp.c_str(), "WHERE"))//û��where����ɾ��ȫ�����Ǳ�������
	{
		//2��whereɸѡ
		bool non;
		if (analyse_where(VT, non) == false)
			return;
	}
	else if (char2int(temp.c_str()) != END)
	{
		Error(NULL, 23);
		return;
	}
	

	/* ����delete */

	//TODO:2������deleteɾ��table����VT��ͬ����Ŀ
	TableNode * TB = BinaryTree.FindTable((char *)table.c_str());
	if (TB == NULL)
	{
		FindError(table.c_str());
		cout << "ɾ��ʧ��" << endl;
		return;
	}
	TB->deleteNode(VT);
	if (GlobalError == false)
	{
		cout << "ɾ��ʧ��" << endl;
		return;
	}
	delete VT;
	cout << "ɾ���ɹ�" << endl;
}
/*
 * run_update�����������и��µĺ���
 * ����update/set/where�����﷨�ϱ�����ֵģ���������ѡ�����
 * ����˳��update��set��where
 * ִ��˳��where��set
 */
void run_update()
{
	string temp;
	string table;
	
	/* ���������� */
	input >> table;
	if (char2int(table) == END)
		return;
	
	//TODO:1�������±�
	TableNode *VT, *tempT;
	tempT = BinaryTree.FindTable((char *)table.c_str());
	if (tempT == NULL)
	{
		FindError(table.c_str());
		cout << "����ʧ��" << endl;
		return;
	}
	VT = From(tempT, NULL);
	if (GlobalError == false)
	{
		cout << "����ʧ��" << endl;
		return;
	}

	/* ����Ƿ����update�ĸ�ʽ */
	input >> temp;
	if (_stricmp(temp.c_str(), "SET"))
	{
		Error(NULL, 18);
		return;
	}
	
	bool where_ = false;
	Scharp *rowname = new Scharp, *value = new Scharp;
	/* ���������ƺ���ֵ */
	while (1)
	{
		//��������
		input >> temp;
		if (char2int(temp) == END)
			break;
		else if (!_stricmp(temp.c_str(), "WHERE"))
		{
			where_ = true;
			break;
		}
		rowname->insert(temp);
		//����Ⱥ�
		input >> temp;
		if (strcmp(temp.c_str(), "="))
		{
			Error(temp.c_str(), 19);
			return;
		}
		//����ֵ
		input >> temp;
		if (char2int(temp) == END)
		{
			Error(NULL, 20);
			return;
		}
		valueclear(temp);
		value->insert(temp);
	}


	/* ����where��� */ 
	if (where_)
	{
		//2��whereɸѡ
		bool non;
		if (analyse_where(VT, non) == false)
			return;
	}
	

	/* ��ʼ���� */
    //TODO������
	TableNode * TB = BinaryTree.FindTable((char *)table.c_str());
	if (TB == NULL)
	{
		FindError(table.c_str());
		cout << "����ʧ��" << endl;
		return;
	}
	TB->updateNode(VT, rowname->len, rowname->head, value->head);
	if (GlobalError == false)
	{
		cout << "����ʧ��" << endl;
		return;
	}
	
	delete rowname;
	delete value;
	delete VT;
	cout << "���³ɹ�" << endl;
}