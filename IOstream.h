#ifndef IOstream_H_
#define IOstream_H_

#include "Database_ADT.h"
#include <Windows.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <queue>
using namespace std;

/*º¯ÊýÉùÃ÷*/
int type_select(string p);
void print(TableNode *ReturnNode);
void startpage();
void selectpage();
void versionmenu();
void errorin(int x);
void from_text();
void to_text();

#endif