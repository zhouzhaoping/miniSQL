// IO流.cpp : 定义控制台应用程序的入口点。
//
#include "IOstream.h"
#include "analyse.h"

/*全局变量定义*/
extern TableTree BinaryTree;
extern istringstream input;
extern bool GlobalError;

/*整数返回0,浮点数返回1,字符串返回2*/
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
void print(TableNode *ReturnNode)      //参数为表
{
	int i,j,*offset;
	DataNode *p=ReturnNode->head;
	offset=new int[ReturnNode->itemNum+1]();
	if (ReturnNode->name != NULL)
		cout<<ReturnNode->name<<" ";
	cout<< "行数：" << ReturnNode->itemNum << ", 列数：" << ReturnNode->dataNum << endl;
	/*计算偏移量，便于输出*/
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
/*从文件中导入数据*/
void from_text()
{
	string name;
	int i,j,m,n;
	ifstream infile,tmpin;
	TableNode *p;
	DataNode **q;
	versionmenu();
	cout<<"请输入要导入的文件名"<<endl
		<<"如果要退出，请输入q"<<endl;
	cin>>name;
	if(name=="q"||name=="Q")
	{
		startpage();
		return;
	}
	infile.open(name.c_str());
	if(!infile)
	{
		cout<<"找不到存档文件"<<endl;
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
		/*读入数据列数和行数*/
		infile>>m>>n;
		/*找表*/
        buf = new char[strlen(headname.c_str())+1]();
		strcpy(buf,headname.c_str());
		p=BinaryTree.FindTable(buf);
		delete []buf;
		/*判断是否对应*/
		if(p!=NULL)
		{
			if(m!=p->itemNum)
			{
				cout<<"与已有项冲突，请确认文件"<<endl;
				system("pause");
				return;
			}
		}
		t1=new char*[m+1]();
		info=new char *[m+1]();
		type=new int[i+8]();
		q=new DataNode*[n+1]();
		/*项名读取*/
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
					cout<<"与已有项冲突，请确认文件"<<endl;
					system("pause");
					return;
				}
			}
		}
		/*类型判断*/
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
	cout<<"成功"<<endl;
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
	cout<<"请输入要输出的文件名"<<endl
		<<"如果要退出，请输入q"<<endl;
	cin>>name;
	if(name=="q"||name=="Q")
	{
		startpage();
		return;
	}
	else
	{
		outfile.open(name.c_str());
		/*文件输出流，依次导出表名，项数，行数，数据*/
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
		cout<<"完成"<<endl;
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
			from_text();      //导入函数
			continue;
		}
		//语句解析
		else if(in1=="2")
		{
			system("cls");
			cout<<"请输入指令(输入\'QUIT;\'退出到主菜单)" << endl;
			cout << "每条指令以分号\';\'结束" << endl;
			while(true)
			{
				GlobalError = true;
				cout << "请输入指令:" << endl;
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
			to_text();       //导出函数
			continue;
		}
		else if(in1=="4")
		{
			cout<<"确定吗(Y/N)"<<endl;
			cin>>in2;
			if(in2=="Y"||in2=="y")
				return ;         //直接退出
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
	    <<"*                 小型数据库                    *"<<endl
		<<"*             版本号：V1.0最终版                *"<<endl
		<<"*         小组成员：王卓 周钊平 张高翔          *"<<endl
	    <<"*************************************************"<<endl;
}
void selectpage()
{
	cout<<"请输入选项前的编号执行对应的操作："<<endl
		<<"1.从文件中导入数据集；"<<endl
		<<"2.执行语句输入；"<<endl
		<<"3.向文件中导出数据集；"<<endl
		<<"4.退出本系统；"<<endl;
}