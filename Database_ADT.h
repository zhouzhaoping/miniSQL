#ifndef Database_ADT_H
#define Database_ADT_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
using namespace std;

#define eps 1e-10   //精度控制

/*数据存储结点*/
struct DataNode {
	char * name;//数据主键的名称
	int itemNum;//记录中项的数量
	char ** data;//记录中的数据，每个数据以字符串存储
	DataNode * next;//下一个数据节点的指针

	DataNode();//构造函数
	~DataNode();//析构函数
};

/*表类*/
class TableNode {
public:
	char * name;//表的名称
	DataNode * head, * tail;//表中第一项与最后一项的指针
	int dataNum;//表中记录的数目
	int itemNum;//表中数据项的数目
	char ** itemName;//每个项的名称
	int * type;//每个项的类型
	TableNode * left, * right;//下一个表、上一个表

	/*构造函数*/
	TableNode();//无参
	TableNode(char * tableName, char ** itemNameNew, int * Type, int num);//以表名、项名、项类型、项数为参数
	TableNode(TableNode * Source);//以另一个表为参数

	/*初始化*/
	void Init(TableNode * Source);//以另一个表为参数
	void init(int num, char ** item, int * type);//以项数量、项名、项类型为参数

	/*析构函数*/
	~TableNode();

	/*操作函数*/

	TableNode * Where(char * item, char ** value, int isItem, int num, int condiction, int tag);//condiction = 0 1 2 (tag = 0 < = >, tag = 1 >= != <=), -1为无条件

	void insertNode(char * mainData, char ** data);//插入数据节点（以数据为参数）
	void Insert(DataNode * aim);//插入数据节点（以数据节点为参数）

	void updateNode(TableNode * up, int num, char ** itemNameNew, char ** data);//更新数据（以待更新表和数据为参数）
	void Update(DataNode * aim, int num, int * itemIndex, char ** data);//更新数据（以待更新数据结点和数据为参数）

	void deleteNode(TableNode * del);//删除记录（以待删除表为参数）
	void Delete(DataNode * pre, DataNode * aim);//删除记录（以数据结点即前继结点为参数）

	void Distinct();//删除表中重复记录

	TableNode * Select(int num, char ** item);//选取目标项（以项数、项名为参数）

	TableNode * OrderBy(int num, char ** item, int * condiction);//排序（以项数、项名、每项条件为参数）

	TableNode * As(char ** item, int * opt, char ** data);//改名（以项名、更改方式、数据为参数）

	void Opt(int index, int opt, char * data);//（未用到？）
};

/*总类*/
class TableTree {
public:
	TableNode * mainRoot;//表根结点
	int TableNum;//表的数量

	TableTree();//构造函数

	void CreatNewTable(char * tableName, char ** itemNameNew, int * Type,  int num);//建立一个新表
	void InsertTable(TableNode * table);//插入表结点
	TableNode * FindTable(char * name);//查询表
};

int FindItemIndex(TableNode * a, char * item);//寻找项名称对应的索引值（以表指针、项名为参数）

void copyNode(DataNode * New, DataNode * Source);//将数据结点完整复制（以新数据结点指针、目标数据结点指针为参数）

TableNode * From(TableNode * a, TableNode * b);//FROM操作，b = NULL时为单表（以两表指针为参数，第二个指针为NULL时为单表）

TableNode * Or(TableNode * a, TableNode * b);//两表Or（以两表指针为参数）

TableNode * And(TableNode * a, TableNode * b);//两表And（以两表指针为参数）

int cmpData(char * a, char * b, int type);//数据比较函数（暂仅支持char比较）

DataNode * makeNode(int num, char * mainData, char ** data);//根据数据构建节点（以项数、主键、数据为参数）

char ** ParaTrans(TableNode * a, int num, char ** item, char ** data);//转换参数使格式与数据结点相同（以表指针、项数、项名称、数据为参数）

#endif