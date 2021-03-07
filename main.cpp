#include <iostream>
#include <string>
#include <windows.h>
#include <psapi.h>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include "Application.cpp"

#define length 512
using namespace std;

vector<string> v;
unordered_map<string, Application*> applications;
string activeExe = "";

string getString(wchar_t *w) {
    wstring ws(w);
    string str(ws.begin(), ws.end());
    return str;
}

void updateApplication(string exe, string title, int memory, boolean active) {
    Application* application;
    if (applications.find(exe) == applications.end()) { // null
        application = new Application(exe);
        applications.insert(make_pair(exe, application));
    } else {
        application = applications[exe];
    }
    
    Application* subApp;
    if (application->findKey(title)) { // not null
        subApp = application->getApplication(title);
    } else {
        subApp = new Application(title);
    }

    if (active) {
        subApp->incrementTime(true);
        application->incrementTime(true);
        application->putApplication(title, subApp);
    } else {
        application->incrementTime(false);
    }
}

void exec(HWND hwnd, boolean active) {
    // get process ID
    DWORD pId;
    DWORD threadId = GetWindowThreadProcessId(hwnd, &pId);

    // get EXE
    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pId);
    char exeBuf[length];
	GetModuleFileNameExA(process, NULL, exeBuf, length);
    string exe(exeBuf);

    // get title
    wchar_t titleBuf[length];
    GetWindowTextW(hwnd, titleBuf, length);
    string title = getString(titleBuf);

    if (exe.empty() 
            || title.empty() 
            || exe == activeExe 
            || find(v.begin(), v.end(), exe) != v.end()) {
        return;
    }

    if (active) {
        activeExe = exe;
    }
    updateApplication(exe, title, 0, active);
    v.insert(v.begin(), exe);
}

static BOOL CALLBACK enumWindowCallback(HWND hwnd, LPARAM lparam) {
    exec(hwnd, false);
    return TRUE;
}

void printMap() {
    cout << "-------" << endl;
    for (auto& x: applications) {
        Application* app = x.second;
        cout << app->toString();
    }
}

void run() {
    v.clear();
    activeExe = "";
    HWND active = GetForegroundWindow();
    exec(active, true);
    // uncomment to track every open window
    //EnumWindows(enumWindowCallback, 0);
    printMap();
}

int main() {
    thread t1(run);
    while (true) {
        run();
        chrono::milliseconds timespan(interval);
        this_thread::sleep_for(timespan);
    }
}