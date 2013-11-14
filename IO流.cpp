// IO��.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "IOstream.h"
#include "analyse.h"

/*ȫ�ֱ�������*/
extern TableTree BinaryTree;
extern istringstream input;
extern bool GlobalError;

/*��������0,����������1,�ַ�������2*/
int type_select(string p)
{
	int i,flag1=0,flag2=0;
	for(i=0;i<p.length();i++)
	{
		if(p[i]<'0'||p[i]>'9')
		{
			flag1=1;
			if(p[i]='.'&&i!=0&&i!=p.length()-1)
				flag2++;
			else
				break;
		}
	}
	if(flag1==0)
		return 0;
	if(flag1==1&&flag2==1)
		return 1;
	else
		return 2;
}
void print(TableNode *ReturnNode)      //����Ϊ��
{
	int i,j,*offset;
	DataNode *p=ReturnNode->head;
	offset=new int[ReturnNode->itemNum+1]();
	if (ReturnNode->name != NULL)
		cout<<ReturnNode->name<<" ";
	cout<< "������" << ReturnNode->itemNum << ", ������" << ReturnNode->dataNum << endl;
	/*����ƫ�������������*/
	for(i=0;i<ReturnNode->itemNum;i++)
	{
		if(strlen(ReturnNode->itemName[i])>offset[i])
			offset[i]=strlen(ReturnNode->itemName[i]);
	}
	for(i=0;i<ReturnNode->dataNum;i++)
	{
		for(j=0;j<ReturnNode->itemNum;j++)
		{
			if(strlen(p->data[j])>offset[j])
			{
				offset[j]=strlen(p->data[j]);
			}
		}
		p=p->next;
	}
	p=ReturnNode->head;
	for(i=0;i<ReturnNode->itemNum;i++)
	{
		cout<<setiosflags(ios::left)<<setw(offset[i]+2)<<ReturnNode->itemName[i];
	}
	cout<<endl;
	for(i=0;i<ReturnNode->dataNum;i++)
	{
		for(j=0;j<ReturnNode->itemNum;j++)
		{
			cout<<setiosflags(ios::left)<<setw(offset[j]+2)<<p->data[j];
		}
		cout<<endl;
		p=p->next;
	}
	delete []offset;
}
/*���ļ��е�������*/
void from_text()
{
	string name;
	int i,j,m,n;
	ifstream infile,tmpin;
	TableNode *p;
	DataNode **q;
	versionmenu();
	cout<<"������Ҫ������ļ���"<<endl
		<<"���Ҫ�˳���������q"<<endl;
	cin>>name;
	if(name=="q"||name=="Q")
	{
		startpage();
		return;
	}
	infile.open(name.c_str());
	if(!infile)
	{
		cout<<"�Ҳ����浵�ļ�"<<endl;
		system("pause");
		return;
	}  
	while(true)
	{
		string headname,data;
		char *buf,**t1,**info,*maininfo;
		int *type;
		i=0,j=0;
		infile>>headname;
		if(headname=="END")
			break;
		/*������������������*/
		infile>>m>>n;
		/*�ұ�*/
        buf = new char[strlen(headname.c_str())+1]();
		strcpy(buf,headname.c_str());
		p=BinaryTree.FindTable(buf);
		delete []buf;
		/*�ж��Ƿ��Ӧ*/
		if(p!=NULL)
		{
			if(m!=p->itemNum)
			{
				cout<<"���������ͻ����ȷ���ļ�"<<endl;
				system("pause");
				return;
			}
		}
		t1=new char*[m+1]();
		info=new char *[m+1]();
		type=new int[i+8]();
		q=new DataNode*[n+1]();
		/*������ȡ*/
		for(i=0;i<m;i++)
		{
			infile>>data;
			buf=new char[strlen(data.c_str())+1]();
			strcpy(buf,data.c_str());
			t1[i]=buf;
			if(p!=NULL)
			{
				if(strcmp(t1[i],p->itemName[i])!=0)
				{
					cout<<"���������ͻ����ȷ���ļ�"<<endl;
					system("pause");
					return;
				}
			}
		}
		/*�����ж�*/
		for(i=0;i<n;i++)
		{
			for(j=0;j<m;j++)
			{
				infile>>data;
				if(j==0)
				{
					buf=new char[strlen(data.c_str())+1]();
					strcpy(buf,data.c_str());
					maininfo=buf;
				}
				else
				{
					buf=new char[strlen(data.c_str())+1]();
				}
				strcpy(buf,data.c_str());
				info[j]=buf;
				if(type_select(data)>type[j])
				{
					type[j]=type_select(data);
				}
			}
			q[i]=makeNode (m, maininfo, info);
		}
		buf = new char[strlen(headname.c_str())+1]();
		strcpy(buf,headname.c_str());
		if(p==NULL)
		{
			BinaryTree.CreatNewTable(buf,t1,type,m);
		}
		p=BinaryTree.FindTable(buf);
		for(i=0;i<n;i++)
		{
			p->Insert(q[i]);
		}
		delete []buf;
		//delete []t1;
		//delete []info;
		delete []type;
        delete []maininfo;
	}
	infile.close();
	cout<<"�ɹ�"<<endl;
	system("pause");
	return;
}
void to_text()
{
	string name,in1;
	ofstream outfile;
	TableNode *p=BinaryTree.mainRoot;
	DataNode *q;
	queue<TableNode *>aQueue;
	int j,i=1;
	versionmenu();
	cout<<"������Ҫ������ļ���"<<endl
		<<"���Ҫ�˳���������q"<<endl;
	cin>>name;
	if(name=="q"||name=="Q")
	{
		startpage();
		return;
	}
	else
	{
		outfile.open(name.c_str());
		/*�ļ�����������ε�������������������������*/
		aQueue.push(p);
		while(!aQueue.empty())
		{
			int *offset;
			offset=new int[p->itemNum+2]();
			p=aQueue.front();
			aQueue.pop();
			q=p->head;
			for(i=0;i<p->itemNum;i++)
			{
				if(strlen(p->itemName[i])>offset[i])
					offset[i]=strlen(p->itemName[i]);
			}
			for(i=0;i<p->dataNum;i++)
			{
				for(j=0;j<p->itemNum;j++)
				{
					if(strlen(q->data[j])>offset[j])
					{
						offset[j]=strlen(q->data[j]);
					}
				}
				q=q->next;
			}
			q=p->head;
			outfile<<p->name<<" "<<p->itemNum<<" "<<p->dataNum<<endl;
			for(i=0;i<p->itemNum;i++)
			{
				outfile<<setiosflags(ios::left)<<setw(offset[i]+2)<<p->itemName[i];
			}
			outfile<<endl;
			for(i=0;i<p->dataNum;i++)
			{
				for(j=0;j<p->itemNum;j++)
				{
					outfile<<setiosflags(ios::left)<<setw(offset[j]+2)<<q->data[j];
				}
				outfile<<endl;
				q=q->next;
			}
			if(p->left!=NULL)
			{
				aQueue.push(p->left);
			}
			if(p->right!=NULL)
			{
				aQueue.push(p->right);
			}
			delete []offset;
		}
		outfile<<"END"<<endl;
		outfile.close();
		cout<<"���"<<endl;
		system("pause");
		return;
	}
	return;
}

void errorin(int x)
{
	switch(x)
	{
	case 1:
		cout<<"##### Wrong Input #####"<<endl;
		break;
	default:
		break;
	}
}
void startpage()
{
	//TableNode *ReturnNode;
	while(true)
	{
		int i=0;
		string in1,in2;
		system("cls");
		versionmenu();
		errorin(i);
		i=0;
	    selectpage();
	    cin>>in1;
		if(in1=="1")
		{
			system("cls");
			from_text();      //���뺯��
			continue;
		}
		//������
		else if(in1=="2")
		{
			system("cls");
			cout<<"������ָ��(����\'QUIT;\'�˳������˵�)" << endl;
			cout << "ÿ��ָ���Էֺ�\';\'����" << endl;
			while(true)
			{
				GlobalError = true;
				cout << "������ָ��:" << endl;
				string command;
				while (cin.peek() == '\n' || cin.peek() == ' ')
					cin.get();
				getline(cin, command, ';');
				if (!strncmp(command.c_str(), "QUIT", 4))
					break;
				if (normalstring(command) == true)
				{	
					input.clear();
					input.str(command);
					route();
				}
			}
			continue;
		}
		else if(in1=="3")
		{
			system("cls");
			to_text();       //��������
			continue;
		}
		else if(in1=="4")
		{
			cout<<"ȷ����(Y/N)"<<endl;
			cin>>in2;
			if(in2=="Y"||in2=="y")
				return ;         //ֱ���˳�
			else
			{
				system("cls");
				continue;
			}
		}
		else
		{
			i=1;
			system("cls");
		}
	}
}
void versionmenu()
{
	cout<<"*************************************************"<<endl
	    <<"*                 С�����ݿ�                    *"<<endl
		<<"*             �汾�ţ�V1.0���հ�                *"<<endl
		<<"*         С���Ա����׿ ����ƽ �Ÿ���          *"<<endl
	    <<"*************************************************"<<endl;
}
void selectpage()
{
	cout<<"������ѡ��ǰ�ı��ִ�ж�Ӧ�Ĳ�����"<<endl
		<<"1.���ļ��е������ݼ���"<<endl
		<<"2.ִ��������룻"<<endl
		<<"3.���ļ��е������ݼ���"<<endl
		<<"4.�˳���ϵͳ��"<<endl;
}