#include "Database_ADT.h"
#include "IOstream.h"

extern bool GlobalError;

/*���ݽ��ṹ�޲ι��캯��*/
DataNode::DataNode()
{
	next = NULL;
}

/*���ݽ��ṹ��������*/
DataNode::~DataNode() {
	for (int i = 0; i < itemNum; i++)
	{
		delete[] data[i];
	}
	delete[] data;
	delete[] name;
}

/*���ݱȽϺ������ݽ�֧��char�Ƚϣ�
  �Դ��Ƚϵ��������ݡ�������Ϊ����
  ����-1ΪС�ڣ�0Ϊ���ڣ�1Ϊ����*/
int cmpData (char * a, char * b, int type)
{
	if (a == NULL || b == NULL)
	{
		GlobalError = false;
		printf("Error:���ݱȽϺ����������ΪNULL\n");
		return -2;
	}
	switch(type)
	{
	case 2:
		/*2Ϊ�ַ����������ַ����Ƚ�*/
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
		/*1Ϊ�����������ո������Ƚ�*/
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
		/*0Ϊ���������������Ƚ�*/
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
			/*���Ͳ�����*/
			GlobalError = false;
			printf("��������%d�����ڣ�0��������1����������2���ַ�����\n", type);
			return -2;
		}
	}
}

/*�ڱ��в���ָ������������ֵ
  �Ա�ָ�롢������Ϊ��������������ֵ
  ���ض�Ӧ�����ڱ��е�����ֵ*/
int FindItemIndex(TableNode * a, char * item)
{
	if (a == NULL || item == NULL)
	{
		GlobalError = false;
		printf("Error:FindItemIndex��������ΪNULL\n");
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
	printf("Error:����δ�ҵ���%s\n", item);
	return -1;
}

/*�����ݽ���������Ƶ��½�ָ����
  ���½���ָ�롢����Դָ��Ϊ����*/
void copyNode(DataNode * New, DataNode * Source)
{
	if (New == NULL || Source == NULL)
	{
		GlobalError = false;
		printf("Error:copyNode��������ΪNULL\n");
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

/*�������ݹ������ݽ��
  ���������ơ����ݽ������ֵ������ֵΪ����
  ���ع����ɵ����ݽ��ָ��*/
DataNode * makeNode (int num, char * mainData, char ** data)
{
	if (mainData == NULL || data == NULL)
	{
		GlobalError = false;
		printf("Error:makeNode����ΪNULL\n");
		return NULL;
	}
	/*�½���*/
	DataNode * temp;
	temp = new DataNode;
	temp->itemNum = num;
	temp->next = NULL;
	temp->name = new char[strlen(mainData) + 1];
	strcpy(temp->name, mainData);//������ֵ
	temp->data = new char * [num];
	for (int i = 0; i < num; i++)
	{
		temp->data[i] = new char[strlen(data[i]) + 1];
		strcpy(temp->data[i], data[i]);//���ݸ�ֵ
	}
	return temp;
}

/*��������
  ������ָ��Ϊ����
  ���ز�����ı�*/
TableNode * Or(TableNode * a, TableNode * b)
{
	if (a == NULL || b == NULL)
	{
		GlobalError = false;
		printf("Error:Or�����������ΪNULL\n");
		return NULL;
	}
	if (a->itemName == NULL || b->itemName == NULL)
	{
		GlobalError = false;
		printf("Error:Or�����������δ��ʼ��\n");
		return NULL;
	}
	for (DataNode * iNode = a->head; iNode != NULL; iNode = iNode->next)
	{
		for (DataNode * jNode = b->head, * pre = NULL; jNode != NULL; )
		{
			/*���ظ��Ľ���b��ɾ��*/
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
	/*��������������*/
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

/*�����Ҳ���
  ������ָ��Ϊ����
  ���ز�����ı�*/
TableNode * And(TableNode * a, TableNode * b)
{
	if (a == NULL || b == NULL)
	{
		GlobalError = false;
		printf("Error:And�����������ΪNULL\n");
		return NULL;
	}
	if (a->itemName == NULL || b->itemName == NULL)
	{
		GlobalError = false;
		printf("Error:And�����������δ��ʼ��\n");
		return NULL;
	}
	/*�����±�*/
	TableNode * newTable;
	newTable = new TableNode;
	newTable->Init(a);
	if (!GlobalError)
		return NULL;
	for (DataNode * iNode = a->head; iNode != NULL; iNode = iNode->next)
	{
		for (DataNode * jNode = b->head; jNode != NULL; jNode = jNode->next)
		{
			/*�������нԴ��ڵ�������±�*/
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

/*FROM����
  ����������Ϊ����
  ���غϳɱ�*/
TableNode * From(TableNode * a, TableNode * b)
{
	if (a == NULL)
	{
		GlobalError = false;
		printf("Error:From��������ΪNULL\n");
		return NULL;
	}
	TableNode * NewTable;
	NewTable = new TableNode;
	if (b == NULL)
	{
		/*���b������Ϊ�������������ԭ��*/
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
	/*˫��ϳ�*/
	char ** item, * temp;
	item = new char * [a->itemNum + b->itemNum];
	NewTable->itemNum = a->itemNum + b->itemNum;
	NewTable->itemName = new char * [NewTable->itemNum];
	NewTable->type = new int[NewTable->itemNum];
	int i;
	for (i = 0; i < a->itemNum; i++)
	{
		NewTable->type[i] = a->type[i];
		/*a��������¼��*/
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
		/*b��������¼��*/
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
			/*����Ӧ���ݽ��ϳ�¼��*/
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

/*������ʽת������
  �Ա�ָ�롢�����������ơ�����Ϊ����
  ��������ʽת��ΪĿ���ĸ�ʽ����*/
char ** ParaTrans(TableNode * a, int num, char ** item, char ** data)
{
	if (a == NULL || item == NULL || data == NULL)
	{
		GlobalError = false;
		printf("Error:ParaTrans�������ΪNULL\n");
		return NULL;
	}
	if (a->itemName == NULL)
	{
		GlobalError = false;
		printf("Error:ParaTrans�������δ��ʼ��\n");
		return NULL;
	}
	int * index;
	index = new int[num];
	for (int i = 0; i < num; i++)
	{
		/*Ѱ��ÿ�������Ķ�Ӧ����ֵ*/
		index[i] = FindItemIndex(a, item[i]);
		if (!GlobalError)
			return NULL;
	}
	char ** itemNew;
	itemNew = new char * [a->itemNum];
	for (int i = 0; i < a->itemNum; i++)
	{
		/*��ʼΪ'\0'*/
		itemNew[i] = new char[1];
		itemNew[i][0] = '\0';
	}
	for (int i = 0; i < num; i++)
	{
		/*�������ݵ���Ӧλ����*/
		strcat(itemNew[index[i]], data[i]);
	}
	/*���ظ�ʽ�޸Ĺ��Ĳ���*/
	return itemNew;
}