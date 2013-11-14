#include "Database_ADT.h"
#include "IOstream.h"

extern bool GlobalError;

/*数据结点结构无参构造函数*/
DataNode::DataNode()
{
	next = NULL;
}

/*数据结点结构析构函数*/
DataNode::~DataNode() {
	for (int i = 0; i < itemNum; i++)
	{
		delete[] data[i];
	}
	delete[] data;
	delete[] name;
}

/*数据比较函数（暂仅支持char比较）
  以带比较的两个数据、及类型为参数
  返回-1为小于，0为等于，1为大于*/
int cmpData (char * a, char * b, int type)
{
	if (a == NULL || b == NULL)
	{
		GlobalError = false;
		printf("Error:数据比较函数传入参数为NULL\n");
		return -2;
	}
	switch(type)
	{
	case 2:
		/*2为字符串，按照字符串比较*/
		{
			if (strcmp(a, b) == 0)
				return 0;
			else if (strcmp(a, b) < 0)
				return -1;
			else
				return 1;
		}
		break;
	case 1:
		/*1为浮点数，按照浮点数比较*/
		{
			double num1 = atof(a), num2 = atof(b);
			if (fabs(num1 - num2) < eps)
				return 0;
			else if (num1 < num2)
				return -1;
			else
				return 1;
		}
		break;
	case 0:
		/*0为整数，按照整数比较*/
		{
			int num1 = atoi(a), num2 = atoi(b);
			if (num1 == num2)
				return 0;
			else if (num1 < num2)
				return -1;
			else
				return 1;
		}
		break;
	default:
		{
			/*类型不存在*/
			GlobalError = false;
			printf("数据类型%d不存在（0：整数，1：浮点数，2：字符串）\n", type);
			return -2;
		}
	}
}

/*在表中查找指定项名称索引值
  以表指针、项名称为参数，返回索引值
  返回对应项名在表中的索引值*/
int FindItemIndex(TableNode * a, char * item)
{
	if (a == NULL || item == NULL)
	{
		GlobalError = false;
		printf("Error:FindItemIndex函数参数为NULL\n");
		return -1;
	}
	for (int i = 0; i < a->itemNum; i++)
	{
		if (strcmp(a->itemName[i], item) == 0)
		{
			return i;
		}
	}
	GlobalError = false;
	printf("Error:表中未找到项%s\n", item);
	return -1;
}

/*将数据结点完整复制到新建指针中
  以新建的指针、数据源指针为参数*/
void copyNode(DataNode * New, DataNode * Source)
{
	if (New == NULL || Source == NULL)
	{
		GlobalError = false;
		printf("Error:copyNode函数参数为NULL\n");
		return;
	}
	New->name = new char[strlen(Source->name) + 1];
	strcpy(New->name, Source->name);
	New->itemNum = Source->itemNum;
	New->data = new char * [New->itemNum];
	for (int i = 0; i < New->itemNum; i++)
	{
		New->data[i] = new char[strlen(Source->data[i]) + 1];
		strcpy(New->data[i], Source->data[i]);
	}
	New->next = NULL;
}

/*根据数据构造数据结点
  以数据名称、数据结点主键值、数据值为参数
  返回构建成的数据结点指针*/
DataNode * makeNode (int num, char * mainData, char ** data)
{
	if (mainData == NULL || data == NULL)
	{
		GlobalError = false;
		printf("Error:makeNode参数为NULL\n");
		return NULL;
	}
	/*新建表*/
	DataNode * temp;
	temp = new DataNode;
	temp->itemNum = num;
	temp->next = NULL;
	temp->name = new char[strlen(mainData) + 1];
	strcpy(temp->name, mainData);//主键赋值
	temp->data = new char * [num];
	for (int i = 0; i < num; i++)
	{
		temp->data[i] = new char[strlen(data[i]) + 1];
		strcpy(temp->data[i], data[i]);//数据赋值
	}
	return temp;
}

/*两表或操作
  以两表指针为参量
  返回操作后的表*/
TableNode * Or(TableNode * a, TableNode * b)
{
	if (a == NULL || b == NULL)
	{
		GlobalError = false;
		printf("Error:Or函数传入参数为NULL\n");
		return NULL;
	}
	if (a->itemName == NULL || b->itemName == NULL)
	{
		GlobalError = false;
		printf("Error:Or函数传入参数未初始化\n");
		return NULL;
	}
	for (DataNode * iNode = a->head; iNode != NULL; iNode = iNode->next)
	{
		for (DataNode * jNode = b->head, * pre = NULL; jNode != NULL; )
		{
			/*将重复的结点从b中删除*/
			if (strcmp(iNode->name, jNode->name) == 0)
			{
				DataNode * temp = jNode->next;
				b->Delete(pre, jNode);
				if (!GlobalError)
					return NULL;
				jNode = temp;
			}
			else
			{
				pre = jNode;
				jNode = jNode ->next;
			}
		}
	}
	/*将两表连接起来*/
	for (DataNode * iNode = b->head; iNode != NULL; iNode = iNode->next)
	{
		DataNode * temp;
		temp = new DataNode;
		copyNode(temp, iNode);
		if (!GlobalError)
			return NULL;
		a->Insert(temp);
		if (!GlobalError)
			return NULL;
	}
	delete b;
	return a;
}

/*两表且操作
  以两表指针为参量
  返回操作后的表*/
TableNode * And(TableNode * a, TableNode * b)
{
	if (a == NULL || b == NULL)
	{
		GlobalError = false;
		printf("Error:And函数传入参数为NULL\n");
		return NULL;
	}
	if (a->itemName == NULL || b->itemName == NULL)
	{
		GlobalError = false;
		printf("Error:And函数传入参数未初始化\n");
		return NULL;
	}
	/*构建新表*/
	TableNode * newTable;
	newTable = new TableNode;
	newTable->Init(a);
	if (!GlobalError)
		return NULL;
	for (DataNode * iNode = a->head; iNode != NULL; iNode = iNode->next)
	{
		for (DataNode * jNode = b->head; jNode != NULL; jNode = jNode->next)
		{
			/*将两表中皆存在的项加入新表*/
			if (strcmp(iNode->name, jNode->name) == 0)
			{
				newTable->insertNode(iNode->name, iNode->data);
				if (!GlobalError)
					return NULL;
				break;
			}
		}
	}
	delete a;
	delete b;
	return newTable;
}

/*FROM操作
  以两表名称为参数
  返回合成表*/
TableNode * From(TableNode * a, TableNode * b)
{
	if (a == NULL)
	{
		GlobalError = false;
		printf("Error:From操作参数为NULL\n");
		return NULL;
	}
	TableNode * NewTable;
	NewTable = new TableNode;
	if (b == NULL)
	{
		/*如果b不存在为单表操作，复制原表*/
		NewTable->Init(a);
		if (!GlobalError)
			return NULL;
		for (DataNode * iNode = a->head; iNode != NULL; iNode = iNode->next)
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
	/*双表合成*/
	char ** item, * temp;
	item = new char * [a->itemNum + b->itemNum];
	NewTable->itemNum = a->itemNum + b->itemNum;
	NewTable->itemName = new char * [NewTable->itemNum];
	NewTable->type = new int[NewTable->itemNum];
	int i;
	for (i = 0; i < a->itemNum; i++)
	{
		NewTable->type[i] = a->type[i];
		/*a表项名称录入*/
		if (a->name != NULL)
		{
			temp = new char[strlen(a->name) + 1 + strlen(a->itemName[i])+ 1];
			strcpy(temp, a->name);
			strcat(temp, ".");
			strcat(temp, a->itemName[i]);
			NewTable->itemName[i] = new char[strlen(temp) + 1];
			strcpy(NewTable->itemName[i], temp);
			delete[] temp;
		}
		else
		{
			NewTable->itemName[i] = new char[strlen(a->itemName[i]) + 1];
			strcpy(NewTable->itemName[i], a->itemName[i]);
		}
	}
	for (i = 0; i < b->itemNum; i++)
	{
		NewTable->type[i + a->itemNum] = b->type[i];
		/*b表项名称录入*/
		if (b->name != NULL)
		{
			temp = new char[strlen(b->name) + 1 + strlen(b->itemName[i])+ 1];
			strcpy(temp, b->name);
			strcat(temp, ".");
			strcat(temp, b->itemName[i]);
			NewTable->itemName[i + a->itemNum] = new char[strlen(temp) + 1];
			strcpy(NewTable->itemName[i + a->itemNum], temp);
			delete[] temp;
		}
		else
		{
			NewTable->itemName[i + a->itemNum] = new char[strlen(b->itemName[i]) + 1];
			strcpy(NewTable->itemName[i + a->itemNum], b->itemName[i]);
		}
	}
	for (DataNode * iNode = a->head; iNode != NULL; iNode = iNode->next)
	{
		for (DataNode * jNode = b->head; jNode != NULL; jNode = jNode->next)
		{
			/*将对应数据结点合成录入*/
			for (i = 0; i < a->itemNum; i++)
			{
				item[i] = new char[strlen(iNode->data[i]) + 1];
				strcpy(item[i], iNode->data[i]);
			}
			for (i = 0; i < b->itemNum; i++)
			{
				item[i + a->itemNum] = new char[strlen(jNode->data[i]) + 1];
				strcpy(item[i + a->itemNum], jNode->data[i]);
			}
			NewTable->insertNode(item[0], item);
			if (!GlobalError)
				return NULL;
			for (i = 0; i < NewTable->itemNum; i++)
			{
				delete[] item[i];
			}
		}
	}
	delete[] item;
	return NewTable;
}

/*参数格式转换函数
  以表指针、项数、项名称、数据为参数
  将参数格式转化为目标表的格式返回*/
char ** ParaTrans(TableNode * a, int num, char ** item, char ** data)
{
	if (a == NULL || item == NULL || data == NULL)
	{
		GlobalError = false;
		printf("Error:ParaTrans传入参数为NULL\n");
		return NULL;
	}
	if (a->itemName == NULL)
	{
		GlobalError = false;
		printf("Error:ParaTrans传入参数未初始化\n");
		return NULL;
	}
	int * index;
	index = new int[num];
	for (int i = 0; i < num; i++)
	{
		/*寻找每个项名的对应索引值*/
		index[i] = FindItemIndex(a, item[i]);
		if (!GlobalError)
			return NULL;
	}
	char ** itemNew;
	itemNew = new char * [a->itemNum];
	for (int i = 0; i < a->itemNum; i++)
	{
		/*初始为'\0'*/
		itemNew[i] = new char[1];
		itemNew[i][0] = '\0';
	}
	for (int i = 0; i < num; i++)
	{
		/*复制数据到对应位置上*/
		strcat(itemNew[index[i]], data[i]);
	}
	/*返回格式修改过的参数*/
	return itemNew;
}