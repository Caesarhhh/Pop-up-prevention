#pragma once
#include<string>
HINSTANCE hInstancee;

HWND hhwnd;

RECT x;

#pragma data_seg("flag_data")
int app_count = 0;
HHOOK hhkHook = NULL;                            //¶¨Òå¹³×Ó¾ä±ú
HHOOK hhkHook2 = NULL;
int BAN_NUM = 0;
int WEB_NUM = 0;
char rootpath[128] = "..";
char lastban[128] = "LastBan\n";
#pragma data_seg()



char banlist[8192];

char inputvalue[128];

__declspec(dllexport) BOOL SetWinCreateHook();

__declspec(dllexport) BOOL EndWinCreateHook();

__declspec(dllexport) void getinput();

__declspec(dllexport) void setinput(char *a);

__declspec(dllexport) BOOL SetWebCreateHook();

__declspec(dllexport) BOOL EndWebCreateHook();

__declspec(dllexport) char* getlastban();

__declspec(dllexport) void setlastban();

__declspec(dllexport) int get_Count();

__declspec(dllexport) void set_Count(int a);

__declspec(dllexport) void initEnum();

__declspec(dllexport) char* getroot();