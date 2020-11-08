// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "KBLoc.h"
#include <tchar.h>
#include <atlstr.h> 
#include<fstream>
#include  < windows.h > 
#include<string>
#include<sstream>
#include<mshtml.h>
#include<MsHtmcid.h>
#include<atlbase.h>
#include<iostream>
#include<oleacc.h>
#include<time.h>
#include <stdio.h>
#include <direct.h>
using namespace std;
#pragma comment(linker,"/SECTION:flag_data,RWS")
LPCWSTR stringToLPCWSTR(std::string orig);
string ltos(long l);
string BAN_CLASSES[128];
string WEB_CLASSES[128];
void setinput(char *a) {
    strcpy_s(inputvalue, a);
}
void set_Count(int a) {
    app_count = a;
}
int get_Count() {
    return app_count;
}
char* getroot() {
    return rootpath;
}
bool ifin(char* a, string* cl, int num)
{
    string temp(a);
    for (int i = 0; i < num; i++)
    {
        if (temp.find(cl[i]) < string::npos)
        {
            return true;
        }
        if (temp == cl[i]) {
            return true;
        }
    }
    return false;
}
BOOL KillProcess(DWORD ProcessId)
{
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessId);
    if (hProcess == NULL)
        return FALSE;
    if (!TerminateProcess(hProcess, 0))
        return FALSE;
    return TRUE;
}
BOOL APIENTRY DllMain(HANDLE hModule,

    DWORD ul_reason_for_call,

    LPVOID lpReserved

)

{

    switch (ul_reason_for_call)

    {
    case DLL_PROCESS_ATTACH:

    case DLL_THREAD_ATTACH:

    case DLL_THREAD_DETACH:

    case DLL_PROCESS_DETACH:

        break;

    }

    hInstancee = (HINSTANCE)hModule;          

    return TRUE;

}

string ltos(long l)
{
    ostringstream os;
    os << l;
    string result;
    istringstream is(os.str());
    is >> result;
    return result;
}
LPCWSTR stringToLPCWSTR(std::string orig)
{
    size_t origsize = orig.length() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
    mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

    return wcstring;
}

void setWin(RECT a, HWND b) {
    x = a;
    hhwnd = b;
}

void setlastban(string s) {
    strcpy_s(lastban,128,s.c_str());
}
string to_String(int n)
{
    int m = n;
    char s[100];
    char ss[100];
    int i = 0, j = 0;
    if (n < 0)
    {
        m = 0 - m;
        j = 1;
        ss[0] = '-';
    }
    while (m > 0)
    {
        s[i++] = m % 10 + '0';
        m /= 10;
    }
    s[i] = '\0';
    i = i - 1;
    while (i >= 0)
    {
        ss[j++] = s[i--];
    }
    ss[j] = '\0';
    return ss;
}
char* getlastban() {
    return lastban;
}

int i = 0;
void fileempty(string s) {
    FILE* P;
    P = fopen(s.c_str(), "w");
}

void addBanlist() {

}

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam) {
    CWPSTRUCT* pCwp = reinterpret_cast<CWPSTRUCT*>(lParam);
    
    char wintext[128];
    char winclass[128];
    GetWindowTextA((HWND)wParam,wintext,128);
    RealGetWindowClassA((HWND)wParam, winclass, 128);
    ofstream file;
    file << "\n";
    
    if (nCode == HCBT_CREATEWND||nCode==HCBT_ACTIVATE)
    {
        GetWindowTextA((HWND)wParam, wintext, 128);
        RealGetWindowClassA((HWND)wParam, winclass, 128);
        string path = string(rootpath);
        string temppath = string("/banlist.txt");
        ifstream readfile((path+temppath).c_str());
        string temp;
        BAN_NUM = 0;
        while (getline(readfile, temp)) {
            if (temp.empty()) {
                continue;
            }
            BAN_CLASSES[BAN_NUM++] = temp;
        }
        //MessageBox(NULL, NULL, stringToLPCWSTR(string(wintext)), NULL);
        if ((ifin(wintext, BAN_CLASSES, BAN_NUM)&&!string(wintext).empty())||(ifin(winclass, BAN_CLASSES, BAN_NUM) && !string(winclass).empty())) {
            time_t now_time = time(NULL);
            tm* t_tm = localtime(&now_time);
            string temp = "LastBan\n" + string(wintext) + "\n" + string(winclass) + "\n" + asctime(t_tm);
            strcpy(lastban, temp.c_str());
            app_count++;
            DWORD ban_handle = GetProcessId((HANDLE)wParam);
            DWORD tem;
            DWORD ThreadID;
            ThreadID = GetWindowThreadProcessId((HWND)wParam, &tem);
            BOOL result = KillProcess(tem);
            
        }
    }
    file.close();
    
    return CallNextHookEx(hhkHook, nCode, wParam, lParam);
}

LRESULT CALLBACK WebProc(int nCode, WPARAM wParam, LPARAM lParam) {
    CWPSTRUCT* pCwp = (CWPSTRUCT*)lParam;
    char winclass[128];
    RealGetWindowClassA(pCwp->hwnd, winclass, 128);
    bool bIsClosed = false;
    if ((string(winclass) == "IEFrame"|| string(winclass)=="MozillaWindowClass")&&pCwp->message==174)
    {
        ofstream webfile;
        char wintext[128];
        CString wc;
        GetWindowTextA(pCwp->hwnd, wintext, 128);
        string path = string(rootpath);
        string temppath = string("/weblist.txt");
        ifstream readfile((path+temppath).c_str());
        string temp;
        WEB_NUM = 0;
        while (getline(readfile, temp)) {
            WEB_CLASSES[WEB_NUM++] = temp;
        }
        if ((ifin(wintext, WEB_CLASSES,WEB_NUM) && !string(wintext).empty()) || (ifin(winclass, WEB_CLASSES, WEB_NUM) && !string(winclass).empty())) {
            time_t now_time = time(NULL);
            tm* t_tm = localtime(&now_time);
            string temp = "LastBan\n" + string(wintext) + "\n" + string(winclass) + "\n" + asctime(t_tm);
            strcpy(lastban, temp.c_str());
            app_count++;
            DWORD ban_handle = GetProcessId((HANDLE)wParam);
            DWORD tem;
            DWORD ThreadID;
            ThreadID = GetWindowThreadProcessId((HWND)wParam, &tem);
            BOOL result = KillProcess(tem);
        }
        webfile << pCwp->message<<" "<<pCwp->hwnd;
        webfile << "\n";
        webfile.close();
    }
    return CallNextHookEx(hhkHook2, nCode, wParam, lParam);
}

void getinput() {
    CString temp(inputvalue);
    MessageBox(NULL, NULL, temp, 0);
}
BOOL CALLBACK EnumChildWindowsProc(HWND hWnd, LPARAM lParam)
{
    char wintext[128];
    GetWindowTextA(hWnd, wintext, 128);
    char winclass[128];
    RealGetWindowClassA(hWnd, winclass, 128);
    string path = string(rootpath);
    string temppath = string("/banlist.txt");
    ifstream readfile((path+temppath).c_str());
    string temp;
    BAN_NUM = 0;
    while (getline(readfile, temp)) {
        if (temp.empty()) {
            continue;
        }
        BAN_CLASSES[BAN_NUM++] = temp;
    }
    temppath = string("/weblist.txt");
    ifstream readdfile((path + temppath).c_str());
    WEB_NUM = 0;
    while (getline(readdfile, temp)) {
        WEB_CLASSES[WEB_NUM++] = temp;
    }
    if ((ifin(wintext, BAN_CLASSES, BAN_NUM) && !string(wintext).empty()) || (ifin(winclass, BAN_CLASSES, BAN_NUM) && !string(winclass).empty())) {
        time_t now_time = time(NULL);
        tm* t_tm = localtime(&now_time);
        string temp = "LastBan\n" + string(wintext) + "\n" + string(winclass) + "\n" + asctime(t_tm);
        strcpy(lastban, temp.c_str());
        app_count++;
        DWORD tem;
        DWORD ThreadID;
        ThreadID = GetWindowThreadProcessId((HWND)hWnd, &tem);
        BOOL result = KillProcess(tem);
    }
    return TRUE;
}
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
    
    EnumChildWindows(hWnd, EnumChildWindowsProc, NULL);
    char wintext[128];
    GetWindowTextA(hWnd, wintext, 128);
    char winclass[128];
    RealGetWindowClassA(hWnd, winclass, 128);
    string path = string(rootpath);
    string temppath = string("/banlist.txt");
    ifstream readfile((path+temppath).c_str());
    string temp;
    BAN_NUM = 0;
    while (getline(readfile, temp)) {
        if (temp.empty()) {
            continue;
        }
        BAN_CLASSES[BAN_NUM++] = temp;
    }
    temppath = string("/weblist.txt");
    ifstream readdfile((path + temppath).c_str());
    WEB_NUM = 0;
    while (getline(readdfile, temp)) {
        WEB_CLASSES[WEB_NUM++] = temp;
    }
    if ((ifin(wintext, BAN_CLASSES, BAN_NUM) && !string(wintext).empty()) || (ifin(winclass, BAN_CLASSES, BAN_NUM) && !string(winclass).empty())) {
        time_t now_time = time(NULL);
        tm* t_tm = localtime(&now_time);
        string temp = "LastBan\n" + string(wintext) + "\n" + string(winclass) + "\n" + asctime(t_tm);
        strcpy(lastban, temp.c_str());
        app_count++;
        DWORD tem;
        DWORD ThreadID;
        ThreadID = GetWindowThreadProcessId((HWND)hWnd, &tem);
        BOOL result = KillProcess(tem);
    }
    return TRUE;
}
void initEnum() {
    char szapipath[MAX_PATH];
    memset(szapipath, 0, MAX_PATH);
    GetModuleFileNameA(NULL, szapipath, MAX_PATH);
    string path = string(szapipath).substr(0,string(szapipath).size()-13);
    fileempty("C:\\Users\\yujkl\\Desktop\\test.txt");
    ofstream file;
    file.open("C:\\Users\\yujkl\\Desktop\\test.txt");
    file << path;
    file.close();
    strcpy(rootpath,path.c_str());
    string temppath = string("/banlist.txt");
    ifstream readfile((path+temppath).c_str());
    string temp;
    EnumWindows(EnumWindowsProc, NULL);
}
BOOL SetWinCreateHook() {
    string path = string(rootpath);
    string temppath = string("/banlist.txt");
    ifstream readfile((path+temppath).c_str());
    string temp;
    BAN_NUM = 0;
    while (getline(readfile, temp)) {
        if (temp.empty()) {
            continue;
        }
        BAN_CLASSES[BAN_NUM++] = temp;
    }
    hhkHook = SetWindowsHookEx(WH_CBT, CBTProc,hInstancee,NULL);
    return hhkHook != NULL;
}

BOOL SetWebCreateHook() {
    string path = string(rootpath);
    string temppath = string("/weblist.txt");
    ifstream readfile((path+temppath).c_str());
    string temp;
    WEB_NUM = 0;
    while (getline(readfile, temp)) {
        WEB_CLASSES[WEB_NUM++] = temp;
    }
    hhkHook2 = SetWindowsHookEx(WH_CALLWNDPROC, WebProc, hInstancee, NULL);
    return hhkHook2 != NULL;
}

BOOL EndWinCreateHook() {
    return UnhookWindowsHookEx(hhkHook);
}

BOOL EndWebCreateHook() {
    return UnhookWindowsHookEx(hhkHook2);
}