#include "Database_ADT.h"
#include "IOstream.h"

extern bool GlobalError;

/*�����޲ι��캯��*/
TableNode::TableNode()
{
	name = NULL;
	itemName = NULL;
	type = NULL;
	head = tail = NULL;
	left = right = NULL;
	dataNum = itemNum = 0;
}

/*���ຬ�ι��캯��
  �Ա����ơ������ơ�������Ϊ����*/
TableNode::TableNode(char * tableName, char ** itemNameNew, int * Type, int num)
{
	if (tableName == NULL || itemNameNew == NULL || Type == NULL)
	{
		GlobalError = false;
		printf("Error:TableNode���캯������ΪNULL\n");
		return;
	}
	name = new char[strlen(tableName) + 1];
	strcpy(name, tableName);//��ʼ����
	itemName = new char * [num];
	type = new int[num];
	for (int i = 0; i < num; i++)
	{
		/*��ʼÿ������*/
		itemName[i] = new char[strlen(itemNameNew[i]) + 1];
		strcpy(itemName[i], itemNameNew[i]);
		type[i] = Type[i];
	}
	itemNum = num;
	dataNum = 0;
	head = tail = NULL;
	left = right = NULL;
}

/*���ຬ�ι��캯��
  ����һ����ָ��Ϊ����*/
TableNode::TableNode(TableNode * Source) {
	name = NULL;
	head = tail = NULL;
	dataNum = 0;
	Init(Source);//��ʼ��
	if (!GlobalError)
		return;
	for (DataNode * iNode = Source->head; iNode != NULL; iNode = iNode->next)
	{
		DataNode * temp;
		temp = new DataNode;
		temp->next = NULL;
		copyNode(temp, iNode);//�������ݽ��
		if (!GlobalError)
			return;
		/*�����ƵĽ�����*/
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

/*�����ʼ������
  ����һ����Ϊ����*/
void TableNode::Init(TableNode * Source) {
	if (Source == NULL)
	{
		GlobalError = false;
		printf("Error:Table��ʼ������ΪNULL��\n");
		return;
	}
	if (Source->itemName == NULL || Source->type == NULL)
	{
		GlobalError = false;
		printf("Error:Table��ʼ������Sourceδ��ʼ��\n");
		return;
	}
	/*����һ����Ĳ�����ʼ��*/
	init(Source->itemNum, Source->itemName, Source->type);
	if (!GlobalError)
		return;
}

/*�����ʼ������
  ������������������Ϊ����*/
void TableNode::init(int num, char ** item, int * Type) {
	if (item == NULL || Type == NULL)
	{
		GlobalError = false;
		printf("Error:��ʼ���������ΪNULL\n");
		return;
	}
	itemNum = num;
	itemName = new char * [num];
	type = new int[num];
	for (int i = 0; i < num; i++)
	{
		/*��������������*/
		itemName[i] = new char[strlen(item[i]) + 1];
		strcpy(itemName[i], item[i]);
		type[i] = Type[i];
	}
}

/*��������*/
TableNode::~TableNode()
{
	while(head != NULL)
	{
		/*ɾ�����ݽ��*/
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

/*��ѯ���з��������ļ�¼���У�
  �ԱȽϵ������ƣ��Ƚ�ֵ���ȽϹ�ϵΪ����
  ���ط���������¼��ɵ��±�*/
TableNode * TableNode::Where(char * item, char ** value, int isItem, int num, int condiction, int tag) {
	if (item == NULL || value == NULL)
	{
		GlobalError = false;
		printf("Error:Where�����������ΪNULL\n");
		return NULL;
	}
	else if (isItem > 1 || isItem < 0)
	{
		GlobalError = false;
		printf("Error:Where�������벻�Ϸ�isItem����\n");
		return NULL;
	}
	else if (num < 0)
	{
		GlobalError = false;
		printf("Error:Where�������벻�Ϸ�num����\n");
		return NULL;
	}
	else if (condiction < -1 || condiction > 2)
	{
		GlobalError = false;
		printf("Error:Where�������벻�Ϸ�condiction����\n");
		return NULL;
	}
	else if (tag < 0 || tag > 1)
	{
		GlobalError = false;
		printf("Error:Where�������벻�Ϸ�tag����\n");
		return NULL;
	}
	TableNode * NewTable;
	NewTable = new TableNode;
	NewTable->Init(this);//��ʼ��
	if (!GlobalError)
		return NULL;
	if (condiction < 0)
	{
		/*������ʱ����ԭ��*/
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
	int index = FindItemIndex(this, item), indexV;//Ѱ��Ŀ��������ֵ
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
		/*��һ������ֵ��< = >��*/
		for (DataNode * iNode = head; iNode != NULL; iNode = iNode->next)
		{
			/*�����������Ľ�㸴�Ʋ������±�*/
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
		/*�ڶ�������ֵ��>= != <=��*/
		for (DataNode * iNode = head; iNode != NULL; iNode = iNode->next)
		{
			/*�����������Ľ�㸴�Ʋ������±�*/
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

/*����в�����
  ����������������ֵ��ָ��Ϊ����*/
void TableNode::insertNode(char * mainData, char ** data)
{
	Insert(makeNode(itemNum, mainData, data));//������make�ɽ��������
	if (!GlobalError)
		return;
}

/*����в�����
  ��Ҫ��������ݽ��Ϊ����*/
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

/*ɾ����㺯��
  �Դ�ɾ���ļ�¼��ɵı�Ϊ����*/
void TableNode::deleteNode(TableNode * del)
{
	if (del == NULL)
	{
		GlobalError = false;
		printf("Error:deleteNode�����������ΪNULL\n");
		return;
	}
	if (del->itemName == NULL || del->type == NULL)
	{
		GlobalError = false;
		printf("Error:deleteNode��������δ��ʼ������\n");
		return;
	}
	for (DataNode * D = del->head, * iNode, * pre; D != NULL; )
	{
		for (iNode = head, pre = NULL; iNode != NULL; )
		{
			/*���ڴ�ɾ�����е���ɾ��*/
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
			printf("Error:���ݽ��%s�����ڣ��޷�ɾ��\n", D->name);
			return;
		}
	}
}

/*ɾ����㺯��
  ����ɾ���Ľ��ָ�롢ǰһ�����ݽ��ָ��Ϊ����*/
void TableNode::Delete(DataNode * pre, DataNode * aim)
{
	if (aim == NULL)
	{
		GlobalError = false;
		printf("Error:Delete�����������ΪNULL\n");
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
	delete aim;//delete���
}

/*�������ݽ��
  �Դ����µļ�¼��ɵı��������������������ֵΪ����*/
void TableNode::updateNode(TableNode * up, int num, char ** itemNameNew, char ** data)
{
	if (up == NULL || itemNameNew == NULL || data == NULL)
	{
		GlobalError = false;
		printf("Error:updateNode�����������ΪNULL\n");
		return;
	}
	if (up->itemName == NULL || up->type == NULL)
	{
		GlobalError = false;
		printf("Error:updateNode�����������δ��ʼ��\n");
		return;
	}
	int iNew = 0, * index;
	DataNode * U = up->head, * temp;
	index = new int[num];
	/*�����д������������ֵ*/
	for (int i = 0; i < num; i++)
	{
		index[iNew++] = FindItemIndex(this, itemNameNew[i]);
		if (!GlobalError)
			return;
	}
	/*���ڴ����±��еļ�¼����*/
	for (DataNode * U = up->head, * iNode, * pre; U != NULL; )
	{
		for (iNode = head, pre = NULL; iNode != NULL; )
		{
			/*���ڴ����±��е������*/
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
			printf("Error:���ݽ��%s�����ڣ��޷�����\n", U->name);
			return;
		}
	}
}

/*�������ݽ��
  ������µĽ�㡢�������������������ֵΪ����*/
void TableNode::Update(DataNode * aim, int num, int * itemIndex, char ** data)
{
	if (aim == NULL || itemIndex == NULL || data == NULL)
	{
		GlobalError = false;
		printf("Error:Update�����������ΪNULL\n");
		return;
	}
	if (aim->data == NULL)
	{
		GlobalError = false;
		printf("Error:Update�����������δ��ʼ��\n");
	}
	for (int i = 0; i < num; i++)
	{
		/*��������*/
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

/*���������ظ���*/
void TableNode::Distinct()
{
	for (DataNode * iNode = head; iNode != NULL; iNode = iNode->next)
	{
		for (DataNode * jNode = iNode->next, * pre = iNode; jNode != NULL; )
		{
			int i;
			for (i = 0; i < itemNum; i++)
			{
				/*�Ƚ�������������*/
				if (cmpData(iNode->data[i], jNode->data[i], type[i]) != 0)
				{
					break;
				}
				if (!GlobalError)
					return;
			}
			if (i == itemNum)
			{
				/*���������ʱɾ��һ��*/
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

/*������
  ��������������������Ϊ����
  ����������*/
TableNode * TableNode::OrderBy(int num, char ** item, int * condiction)
{
	if (item == NULL || condiction == NULL)
	{
		GlobalError = false;
		printf("Error:OrderBy�����������ΪNULL\n");
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
		/*Ѱ��ÿ������*/
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
				/*�������Ƚ�*/
				if (cmpData(jNode->data[index[i]], aim->data[index[i]], type[index[i]]) == condiction[i])
				{
					min = 1;
					break;
				}
				if (!GlobalError)
					return NULL;
				else if (cmpData(jNode->data[index[i]], aim->data[index[i]], type[index[i]]) == -condiction[i])
				{
					/*�������*/
					break;
				}
				if (!GlobalError)
					return NULL;
			}
			if (i != num && min)
			{
				/*������Сֵ*/
				rPre = pre;
				aim = jNode;
			}
		}
		if (aim == head)
		{
			pre = NULL;
		}
		/*����ѡ����Ľ��*/
		temp = new DataNode;
		copyNode(temp, aim);
		if (!GlobalError)
			return NULL;
		/*��ԭ����ɾ��*/
		Delete(rPre, aim);
		if (!GlobalError)
			return NULL;
		/*�����±�*/
		NewTable->Insert(temp);
		if (!GlobalError)
			return NULL;
	}
	return NewTable;
}

/*��������ѡ����
  ��ѡ�����������������Ϊ����
  ����ѡ���ı��ָ��*/
TableNode * TableNode::Select(int num, char ** itemNameNew)
{
	if (itemNameNew == NULL)
	{
		GlobalError = false;
		printf("Error:Select�������ΪNULL\n");
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
		/*��������������ֵ*/
		index[i] = FindItemIndex(this, itemNameNew[i]);
		if (!GlobalError)
			return NULL;
		/*�����±���������*/
		Type[i] = type[index[i]];
	}
	temp->init(num, itemNameNew, Type);
	if (!GlobalError)
		return NULL;
	for (DataNode * iNode = head, * pre = NULL; iNode != NULL; iNode = iNode->next)
	{
		/*��ÿ�����ݽ��������ѡ������½��*/
		for (int i = 0; i < num; i++)
		{
			item[i] = new char[strlen(iNode->data[index[i]]) + 1];
			strcpy(item[i], iNode->data[index[i]]);
		}
		/*�������±�*/
		temp->insertNode(item[0], item);
		if (!GlobalError)
			return NULL;
		for (int i = 0; i < num; i++)
		{
			delete[] item[i];
		}
	}
	delete[] item;
	return temp;//�����±�
}

/*����������������
  ����������ʽ������Ϊ����
  ������������ı�*/
TableNode * TableNode::As(char ** item, int * opt, char ** data)
{
	if (item == NULL)
	{
		GlobalError = false;
		printf("Error:As��������δ��ʼ����\n");
		return NULL;
	}
	for (int i = 0; i < itemNum; i++)
	{
		if (strcmp(itemName[i], item[i]) != 0)
		{
			/*�����������������*/
			delete[] itemName[i];
			itemName[i] = new char[strlen(item[i]) + 1];
			strcpy(itemName[i], item[i]);
		}
		if (opt != NULL)
		{
			/*δ�õ���*/
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

/*δ�õ���*/
void TableNode::Opt(int index, int opt, char * data) {
	if (data == NULL)
	{
		GlobalError = false;
		printf("Error:�����������Ϊ��\n");
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
				printf("Error:��������������ʹ���\n");
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
					printf("Error:���������%d\n", opt);
					return;
				}
			}
			gcvt(num1, length, iNode->data[index]);
		}
	}
}