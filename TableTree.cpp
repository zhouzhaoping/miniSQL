#include "Database_ADT.h"

extern bool GlobalError;

/*无参构造函数*/
TableTree::TableTree()
{
	mainRoot = NULL;
	TableNum = 0;
}

/*构建新表
  以表名、各项名、项数为参数*/
void TableTree::CreatNewTable(char * tableName, char ** itemNameNew, int * Type, int num)
{
	if (tableName == NULL || itemNameNew == NULL || Type == NULL)
	{
		GlobalError = false;
		printf("Error:建表传入参数为NULL\n");
		return;
	}
	/*创建新表并将其值赋给指针插入树中中*/
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

/*将表插入树中
  以待插入表为参数*/
void TableTree::InsertTable(TableNode * table)
{
	if (mainRoot == NULL)
	{
		/*如果跟不存在则插入根*/
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
				/*存在该名称的表*/
				GlobalError = false;
				printf("Error:表%s已存在\n", table->name);
				return;
			}
		}
	}
}

/*在树中搜索目标表
  以目标表名称为参数
  返回查找到的表的指针*/
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