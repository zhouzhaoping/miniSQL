#include "Database_ADT.h"

extern bool GlobalError;

/*�޲ι��캯��*/
TableTree::TableTree()
{
	mainRoot = NULL;
	TableNum = 0;
}

/*�����±�
  �Ա�����������������Ϊ����*/
void TableTree::CreatNewTable(char * tableName, char ** itemNameNew, int * Type, int num)
{
	if (tableName == NULL || itemNameNew == NULL || Type == NULL)
	{
		GlobalError = false;
		printf("Error:���������ΪNULL\n");
		return;
	}
	/*�����±�����ֵ����ָ�����������*/
	TableNum++;
	TableNode * newTable;
	newTable = new TableNode;
	newTable->name = new char[strlen(tableName) + 1];
	strcpy(newTable->name, tableName);
	newTable->init(num, itemNameNew, Type);
	if (!GlobalError)
		return;
	InsertTable(newTable);
	if (!GlobalError)
		return;
}

/*�����������
  �Դ������Ϊ����*/
void TableTree::InsertTable(TableNode * table)
{
	if (mainRoot == NULL)
	{
		/*�����������������*/
		mainRoot = table;
		return;
	}
	else
	{
		TableNode * temp = mainRoot;
		while(1)
		{
			if (strcmp(temp->name, table->name) < 0)
			{
				if (temp->left == NULL)
				{
					temp->left = table;
					return;
				}
				temp = temp->left;
			}
			else if (strcmp(temp->name, table->name) > 0)
			{
				if (temp->right == NULL)
				{
					temp->right = table;
					return;
				}
				temp = temp->right;
			}
			else
			{
				/*���ڸ����Ƶı�*/
				GlobalError = false;
				printf("Error:��%s�Ѵ���\n", table->name);
				return;
			}
		}
	}
}

/*����������Ŀ���
  ��Ŀ�������Ϊ����
  ���ز��ҵ��ı��ָ��*/
TableNode * TableTree::FindTable(char * tableName)
{
	if (mainRoot == NULL)
	{
		//GlobalError = false;
		return NULL;
	}
	if (strcmp(mainRoot->name, tableName) == 0)
	{
		return mainRoot;
	}
	else
	{
		TableNode * temp = mainRoot;
		while (strcmp(temp->name, tableName) != 0)
		{
			if (strcmp(temp->name, tableName) < 0)
				temp = temp->left;
			else
				temp = temp->right;
			if (temp == NULL)
			{
				//GlobalError = false;
				return NULL;
			}
		}
		return temp;
	}
}