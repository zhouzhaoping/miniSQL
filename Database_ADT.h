#ifndef Database_ADT_H
#define Database_ADT_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
using namespace std;

#define eps 1e-10   //���ȿ���

/*���ݴ洢���*/
struct DataNode {
	char * name;//��������������
	int itemNum;//��¼���������
	char ** data;//��¼�е����ݣ�ÿ���������ַ����洢
	DataNode * next;//��һ�����ݽڵ��ָ��

	DataNode();//���캯��
	~DataNode();//��������
};

/*����*/
class TableNode {
public:
	char * name;//�������
	DataNode * head, * tail;//���е�һ�������һ���ָ��
	int dataNum;//���м�¼����Ŀ
	int itemNum;//�������������Ŀ
	char ** itemName;//ÿ���������
	int * type;//ÿ���������
	TableNode * left, * right;//��һ������һ����

	/*���캯��*/
	TableNode();//�޲�
	TableNode(char * tableName, char ** itemNameNew, int * Type, int num);//�Ա����������������͡�����Ϊ����
	TableNode(TableNode * Source);//����һ����Ϊ����

	/*��ʼ��*/
	void Init(TableNode * Source);//����һ����Ϊ����
	void init(int num, char ** item, int * type);//����������������������Ϊ����

	/*��������*/
	~TableNode();

	/*��������*/

	TableNode * Where(char * item, char ** value, int isItem, int num, int condiction, int tag);//condiction = 0 1 2 (tag = 0 < = >, tag = 1 >= != <=), -1Ϊ������

	void insertNode(char * mainData, char ** data);//�������ݽڵ㣨������Ϊ������
	void Insert(DataNode * aim);//�������ݽڵ㣨�����ݽڵ�Ϊ������

	void updateNode(TableNode * up, int num, char ** itemNameNew, char ** data);//�������ݣ��Դ����±������Ϊ������
	void Update(DataNode * aim, int num, int * itemIndex, char ** data);//�������ݣ��Դ��������ݽ�������Ϊ������

	void deleteNode(TableNode * del);//ɾ����¼���Դ�ɾ����Ϊ������
	void Delete(DataNode * pre, DataNode * aim);//ɾ����¼�������ݽ�㼴ǰ�̽��Ϊ������

	void Distinct();//ɾ�������ظ���¼

	TableNode * Select(int num, char ** item);//ѡȡĿ���������������Ϊ������

	TableNode * OrderBy(int num, char ** item, int * condiction);//������������������ÿ������Ϊ������

	TableNode * As(char ** item, int * opt, char ** data);//�����������������ķ�ʽ������Ϊ������

	void Opt(int index, int opt, char * data);//��δ�õ�����
};

/*����*/
class TableTree {
public:
	TableNode * mainRoot;//������
	int TableNum;//�������

	TableTree();//���캯��

	void CreatNewTable(char * tableName, char ** itemNameNew, int * Type,  int num);//����һ���±�
	void InsertTable(TableNode * table);//�������
	TableNode * FindTable(char * name);//��ѯ��
};

int FindItemIndex(TableNode * a, char * item);//Ѱ�������ƶ�Ӧ������ֵ���Ա�ָ�롢����Ϊ������

void copyNode(DataNode * New, DataNode * Source);//�����ݽ���������ƣ��������ݽ��ָ�롢Ŀ�����ݽ��ָ��Ϊ������

TableNode * From(TableNode * a, TableNode * b);//FROM������b = NULLʱΪ����������ָ��Ϊ�������ڶ���ָ��ΪNULLʱΪ����

TableNode * Or(TableNode * a, TableNode * b);//����Or��������ָ��Ϊ������

TableNode * And(TableNode * a, TableNode * b);//����And��������ָ��Ϊ������

int cmpData(char * a, char * b, int type);//���ݱȽϺ������ݽ�֧��char�Ƚϣ�

DataNode * makeNode(int num, char * mainData, char ** data);//�������ݹ����ڵ㣨������������������Ϊ������

char ** ParaTrans(TableNode * a, int num, char ** item, char ** data);//ת������ʹ��ʽ�����ݽ����ͬ���Ա�ָ�롢�����������ơ�����Ϊ������

#endif