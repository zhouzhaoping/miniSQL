#include "Database_ADT.h"
#include "IOstream.h"

extern bool GlobalError;

/*表类无参构造函数*/
TableNode::TableNode()
{
	name = NULL;
	itemName = NULL;
	type = NULL;
	head = tail = NULL;
	left = right = NULL;
	dataNum = itemNum = 0;
}

/*表类含参构造函数
  以表名称、项名称、项数量为参数*/
TableNode::TableNode(char * tableName, char ** itemNameNew, int * Type, int num)
{
	if (tableName == NULL || itemNameNew == NULL || Type == NULL)
	{
		GlobalError = false;
		printf("Error:TableNode构造函数参数为NULL\n");
		return;
	}
	name = new char[strlen(tableName) + 1];
	strcpy(name, tableName);//初始表名
	itemName = new char * [num];
	type = new int[num];
	for (int i = 0; i < num; i++)
	{
		/*初始每个项名*/
		itemName[i] = new char[strlen(itemNameNew[i]) + 1];
		strcpy(itemName[i], itemNameNew[i]);
		type[i] = Type[i];
	}
	itemNum = num;
	dataNum = 0;
	head = tail = NULL;
	left = right = NULL;
}

/*表类含参构造函数
  以另一个表指针为参数*/
TableNode::TableNode(TableNode * Source) {
	name = NULL;
	head = tail = NULL;
	dataNum = 0;
	Init(Source);//初始化
	if (!GlobalError)
		return;
	for (DataNode * iNode = Source->head; iNode != NULL; iNode = iNode->next)
	{
		DataNode * temp;
		temp = new DataNode;
		temp->next = NULL;
		copyNode(temp, iNode);//复制数据结点
		if (!GlobalError)
			return;
		/*将复制的结点插入*/
		dataNum++;
		if (head == NULL)
		{
			head = tail = temp;
		}
		else
		{
			tail->next = temp;	
			tail = temp;
		}
	}
}

/*表类初始化函数
  以另一个表为参数*/
void TableNode::Init(TableNode * Source) {
	if (Source == NULL)
	{
		GlobalError = false;
		printf("Error:Table初始化参数为NULL？\n");
		return;
	}
	if (Source->itemName == NULL || Source->type == NULL)
	{
		GlobalError = false;
		printf("Error:Table初始化参数Source未初始化\n");
		return;
	}
	/*用另一个表的参数初始化*/
	init(Source->itemNum, Source->itemName, Source->type);
	if (!GlobalError)
		return;
}

/*表类初始化函数
  以项数、项名、类型为参数*/
void TableNode::init(int num, char ** item, int * Type) {
	if (item == NULL || Type == NULL)
	{
		GlobalError = false;
		printf("Error:初始化传入参数为NULL\n");
		return;
	}
	itemNum = num;
	itemName = new char * [num];
	type = new int[num];
	for (int i = 0; i < num; i++)
	{
		/*复制项名和类型*/
		itemName[i] = new char[strlen(item[i]) + 1];
		strcpy(itemName[i], item[i]);
		type[i] = Type[i];
	}
}

/*析构函数*/
TableNode::~TableNode()
{
	while(head != NULL)
	{
		/*删除数据结点*/
		Delete(NULL, head);
		if (!GlobalError)
			return;
	}
	for (int i = 0; i < itemNum; i++)
	{
		delete[] itemName[i];
	}
	delete[] itemName;
	delete[] type;
	delete[] name;
}

/*查询表中符合条件的记录（行）
  以比较的项名称，比较值，比较关系为参数
  返回符合条件记录组成的新表*/
TableNode * TableNode::Where(char * item, char ** value, int isItem, int num, int condiction, int tag) {
	if (item == NULL || value == NULL)
	{
		GlobalError = false;
		printf("Error:Where函数传入参数为NULL\n");
		return NULL;
	}
	else if (isItem > 1 || isItem < 0)
	{
		GlobalError = false;
		printf("Error:Where函数传入不合法isItem参数\n");
		return NULL;
	}
	else if (num < 0)
	{
		GlobalError = false;
		printf("Error:Where函数传入不合法num参数\n");
		return NULL;
	}
	else if (condiction < -1 || condiction > 2)
	{
		GlobalError = false;
		printf("Error:Where函数传入不合法condiction参数\n");
		return NULL;
	}
	else if (tag < 0 || tag > 1)
	{
		GlobalError = false;
		printf("Error:Where函数传入不合法tag参数\n");
		return NULL;
	}
	TableNode * NewTable;
	NewTable = new TableNode;
	NewTable->Init(this);//初始化
	if (!GlobalError)
		return NULL;
	if (condiction < 0)
	{
		/*无条件时复制原表*/
		for (DataNode * iNode = head; iNode != NULL; iNode = iNode->next)
		{
			DataNode * temp;
			temp = new DataNode;
			copyNode(temp, iNode);
			if (!GlobalError)
				return NULL;
			NewTable->Insert(temp);
			if (!GlobalError)
				return NULL;
		}
		return NewTable;
	}
	int index = FindItemIndex(this, item), indexV;//寻找目标项索引值
	if (!GlobalError)
		return NULL;
	if (isItem == 1)
	{
		indexV = FindItemIndex(this, value[0]);
		if (!GlobalError)
			return NULL;
	}
	if (tag == 0)
	{
		/*第一组条件值（< = >）*/
		for (DataNode * iNode = head; iNode != NULL; iNode = iNode->next)
		{
			/*将符合条件的结点复制并插入新表*/
			if (isItem == 1)
			{
				for (int i = 0; i < num; i++)
				{
					
					if (cmpData(iNode->data[index], iNode->data[indexV], type[index]) + 1 == condiction)
					{
						DataNode * temp;
						temp = new DataNode;
						copyNode(temp, iNode);
						if (!GlobalError)
							return NULL;
						NewTable->Insert(temp);
						if (!GlobalError)
							return NULL;
						break;
					}
					if (!GlobalError)
						return NULL;
				}
			}
			else
			{
				for (int i = 0; i < num; i++)
				{
					if (cmpData(iNode->data[index], value[i], type[index]) + 1 == condiction)
					{
						DataNode * temp;
						temp = new DataNode;
						copyNode(temp, iNode);
						if (!GlobalError)
							return NULL;
						NewTable->Insert(temp);
						if (!GlobalError)
							return NULL;
						break;
					}
					if (!GlobalError)
						return NULL;
				}
			}
		}
	}
	else
	{
		/*第二组条件值（>= != <=）*/
		for (DataNode * iNode = head; iNode != NULL; iNode = iNode->next)
		{
			/*将符合条件的结点复制并插入新表*/
			if (isItem == 1)
			{
				for (int i = 0; i < num; i++)
				{
					if (cmpData(iNode->data[index], iNode->data[indexV], type[index]) + 1 != condiction)
					{
						DataNode * temp;
						temp = new DataNode;
						copyNode(temp, iNode);
						if (!GlobalError)
							return NULL;
						NewTable->Insert(temp);
						if (!GlobalError)
							return NULL;
						break;
					}
					if (!GlobalError)
						return NULL;
				}
			}
			else
			{
				for (int i = 0; i < num; i++)
				{
					if (cmpData(iNode->data[index], value[i], type[index]) + 1 != condiction)
					{
						DataNode * temp;
						temp = new DataNode;
						copyNode(temp, iNode);
						if (!GlobalError)
							return NULL;
						NewTable->Insert(temp);
						if (!GlobalError)
							return NULL;
						break;
					}
					if (!GlobalError)
						return NULL;
				}
			}
		}
	}
	return NewTable;
}

/*向表中插入结点
  以数据主键、数据值首指针为参数*/
void TableNode::insertNode(char * mainData, char ** data)
{
	Insert(makeNode(itemNum, mainData, data));//将数据make成结点插入表中
	if (!GlobalError)
		return;
}

/*向表中插入结点
  以要插入的数据结点为参数*/
void TableNode::Insert(DataNode * aim)
{
	dataNum++;
	if (head == NULL)
	{
		head = tail = aim;
	}
	else
	{
		tail->next = aim;
		tail = aim;
	}
}

/*删除结点函数
  以待删除的记录组成的表为参数*/
void TableNode::deleteNode(TableNode * del)
{
	if (del == NULL)
	{
		GlobalError = false;
		printf("Error:deleteNode操作传入参数为NULL\n");
		return;
	}
	if (del->itemName == NULL || del->type == NULL)
	{
		GlobalError = false;
		printf("Error:deleteNode操作传入未初始化参数\n");
		return;
	}
	for (DataNode * D = del->head, * iNode, * pre; D != NULL; )
	{
		for (iNode = head, pre = NULL; iNode != NULL; )
		{
			/*将在待删除表中的项删除*/
			if (cmpData(D->name, iNode->name, type[0]) == 0)
			{
				Delete(pre, iNode);
				if (!GlobalError)
					return;
				D = D->next;
				break;
			}
			else
			{
				pre = iNode;
				iNode = iNode->next;
			}
			if (!GlobalError)
				return;
		}
		if (iNode == NULL)
		{
			GlobalError = false;
			printf("Error:数据结点%s不存在，无法删除\n", D->name);
			return;
		}
	}
}

/*删除结点函数
  以需删除的结点指针、前一项数据结点指针为参数*/
void TableNode::Delete(DataNode * pre, DataNode * aim)
{
	if (aim == NULL)
	{
		GlobalError = false;
		printf("Error:Delete函数传入参数为NULL\n");
		return;
	}
	dataNum--;
	if (tail == head)
		tail = head = NULL;
	else if (pre == NULL)
		head = head->next;
	else if (aim == tail)
	{
		pre->next = NULL;
		tail = pre;
	}
	else
		pre->next = aim->next;
	delete aim;//delete结点
}

/*更新数据结点
  以待更新的记录组成的表、更新项数、更新项、更新值为参数*/
void TableNode::updateNode(TableNode * up, int num, char ** itemNameNew, char ** data)
{
	if (up == NULL || itemNameNew == NULL || data == NULL)
	{
		GlobalError = false;
		printf("Error:updateNode函数传入参数为NULL\n");
		return;
	}
	if (up->itemName == NULL || up->type == NULL)
	{
		GlobalError = false;
		printf("Error:updateNode函数传入参数未初始化\n");
		return;
	}
	int iNew = 0, * index;
	DataNode * U = up->head, * temp;
	index = new int[num];
	/*求所有待更新项的索引值*/
	for (int i = 0; i < num; i++)
	{
		index[iNew++] = FindItemIndex(this, itemNameNew[i]);
		if (!GlobalError)
			return;
	}
	/*将在待更新表中的记录更新*/
	for (DataNode * U = up->head, * iNode, * pre; U != NULL; )
	{
		for (iNode = head, pre = NULL; iNode != NULL; )
		{
			/*将在待更新表中的项更新*/
			if (cmpData(U->name, iNode->name, type[0]) == 0)
			{
				Update(iNode, num, index, data);
				if (!GlobalError)
					return;
				U = U->next;
				break;
			}
			else
			{
				pre = iNode;
				iNode = iNode->next;
			}
			if (!GlobalError)
				return;
		}
		if (iNode == NULL)
		{
			GlobalError = false;
			printf("Error:数据结点%s不存在，无法更新\n", U->name);
			return;
		}
	}
}

/*更新数据结点
  以需更新的结点、更新项数、更新项、更新值为参数*/
void TableNode::Update(DataNode * aim, int num, int * itemIndex, char ** data)
{
	if (aim == NULL || itemIndex == NULL || data == NULL)
	{
		GlobalError = false;
		printf("Error:Update函数传入参数为NULL\n");
		return;
	}
	if (aim->data == NULL)
	{
		GlobalError = false;
		printf("Error:Update函数传入参数未初始化\n");
	}
	for (int i = 0; i < num; i++)
	{
		/*更新数据*/
		delete[] aim->data[itemIndex[i]];
		aim->data[itemIndex[i]] = new char[strlen(data[i]) + 1];
		strcpy(aim->data[itemIndex[i]], data[i]);
		if (itemIndex[i] == 0)
		{
			delete[] aim->name;
			aim->name = new char[strlen(data[i]) + 1];
			strcpy(aim->name, data[i]);
		}
	}
}

/*消除表中重复项*/
void TableNode::Distinct()
{
	for (DataNode * iNode = head; iNode != NULL; iNode = iNode->next)
	{
		for (DataNode * jNode = iNode->next, * pre = iNode; jNode != NULL; )
		{
			int i;
			for (i = 0; i < itemNum; i++)
			{
				/*比较两项中所有项*/
				if (cmpData(iNode->data[i], jNode->data[i], type[i]) != 0)
				{
					break;
				}
				if (!GlobalError)
					return;
			}
			if (i == itemNum)
			{
				/*所有项相等时删除一个*/
				DataNode * temp = jNode->next;
				Delete(pre, jNode);
				if (!GlobalError)
					return;
				jNode = temp;
			}
			else
			{
				 pre = jNode;
				 jNode = jNode->next;
			}
		}
	}
}

/*排序函数
  以项数、项名、正降序为参数
  返回排序后表*/
TableNode * TableNode::OrderBy(int num, char ** item, int * condiction)
{
	if (item == NULL || condiction == NULL)
	{
		GlobalError = false;
		printf("Error:OrderBy函数传入参数为NULL\n");
		return NULL;
	}
	TableNode * NewTable;
	NewTable = new TableNode;
	NewTable->Init(this);
	if (!GlobalError)
		return NULL;
	int * index;
	index = new int[num];
	for (int i = 0; i < num; i++)
	{
		/*寻找每项索引*/
		index[i] = FindItemIndex(this, item[i]);
		if (!GlobalError)
			return NULL;
	}
	for (DataNode * temp, * aim, * pre, * rPre; head != NULL; )
	{
		pre = aim = head;
		rPre = NULL;
		for (DataNode * jNode = head->next; jNode != NULL; pre = jNode, jNode = jNode->next)
		{
			int i;
			bool min = 0;
			for (i = 0; i < num; i++)
			{
				/*两两项间比较*/
				if (cmpData(jNode->data[index[i]], aim->data[index[i]], type[index[i]]) == condiction[i])
				{
					min = 1;
					break;
				}
				if (!GlobalError)
					return NULL;
				else if (cmpData(jNode->data[index[i]], aim->data[index[i]], type[index[i]]) == -condiction[i])
				{
					/*有相等项*/
					break;
				}
				if (!GlobalError)
					return NULL;
			}
			if (i != num && min)
			{
				/*更新最小值*/
				rPre = pre;
				aim = jNode;
			}
		}
		if (aim == head)
		{
			pre = NULL;
		}
		/*复制选择出的结点*/
		temp = new DataNode;
		copyNode(temp, aim);
		if (!GlobalError)
			return NULL;
		/*从原表中删除*/
		Delete(rPre, aim);
		if (!GlobalError)
			return NULL;
		/*插入新表*/
		NewTable->Insert(temp);
		if (!GlobalError)
			return NULL;
	}
	return NewTable;
}

/*根据条件选出表
  以选出项的数量、项名称为参数
  返回选出的表的指针*/
TableNode * TableNode::Select(int num, char ** itemNameNew)
{
	if (itemNameNew == NULL)
	{
		GlobalError = false;
		printf("Error:Select传入参数为NULL\n");
		return NULL;
	}
	TableNode * temp;
	temp = new TableNode;
	char ** item;
	item = new char * [num];
	int * index, * Type;
	index = new int[num];
	Type = new int[num];
	for (int i = 0; i < num; i++)
	{
		/*计算项名称索引值*/
		index[i] = FindItemIndex(this, itemNameNew[i]);
		if (!GlobalError)
			return NULL;
		/*更新新表中项名称*/
		Type[i] = type[index[i]];
	}
	temp->init(num, itemNameNew, Type);
	if (!GlobalError)
		return NULL;
	for (DataNode * iNode = head, * pre = NULL; iNode != NULL; iNode = iNode->next)
	{
		/*对每个数据结点用其中选中项构成新结点*/
		for (int i = 0; i < num; i++)
		{
			item[i] = new char[strlen(iNode->data[index[i]]) + 1];
			strcpy(item[i], iNode->data[index[i]]);
		}
		/*并插入新表*/
		temp->insertNode(item[0], item);
		if (!GlobalError)
			return NULL;
		for (int i = 0; i < num; i++)
		{
			delete[] item[i];
		}
	}
	delete[] item;
	return temp;//返回新表
}

/*表中项重命名函数
  以项名、方式、数据为参数
  返回重命名后的表*/
TableNode * TableNode::As(char ** item, int * opt, char ** data)
{
	if (item == NULL)
	{
		GlobalError = false;
		printf("Error:As函数传入未出始参数\n");
		return NULL;
	}
	for (int i = 0; i < itemNum; i++)
	{
		if (strcmp(itemName[i], item[i]) != 0)
		{
			/*如果项名不相等则更新*/
			delete[] itemName[i];
			itemName[i] = new char[strlen(item[i]) + 1];
			strcpy(itemName[i], item[i]);
		}
		if (opt != NULL)
		{
			/*未用到？*/
			if (opt[i] != -1)
			{
				int index = FindItemIndex(this, item[i]);
				if (!GlobalError)
					return NULL;
				Opt(index, opt[i], data[i]);
				if (!GlobalError)
					return NULL;
			}
		}
	}
	return this;
}

/*未用到？*/
void TableNode::Opt(int index, int opt, char * data) {
	if (data == NULL)
	{
		GlobalError = false;
		printf("Error:运算操作参数为空\n");
		return;
	}
	double num1, num2;
	int length;
	for (DataNode * iNode = head; iNode != NULL; iNode = iNode->next)
	{
		if (opt == 0)
		{
			delete[] iNode->data[index];
			iNode->data[index] = new char[strlen(data) + 1];
			strcpy(iNode->data[index], data);
		}
		else
		{
			if (type[index] == 0)
			{
				GlobalError = false;
				printf("Error:运算操作参数类型错误\n");
				return;
			}
			num1 = atof(iNode->data[index]);
			num2 = atof(data);
			switch(opt)
			{
			case 1: num1 += num2; break;
			case 2: num1 -= num2; break;
			case 3: num1 *= num2; break;
			case 4: num1 /= num2; break;
			default:
				{
					GlobalError = false;
					printf("Error:错误运算符%d\n", opt);
					return;
				}
			}
			gcvt(num1, length, iNode->data[index]);
		}
	}
}