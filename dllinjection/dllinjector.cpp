#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <conio.h>

using namespace std;

DWORD procId = 0;
DWORD baseAddr = 0;

string chooseDLL()
{
    OPENFILENAME ofn;
    char filename[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    if (GetOpenFileName(&ofn))
    {
        return filename;
    }
}

//In dieser Methode wird die ProcessId herausgefunden
DWORD GetProcId(const char* procName)
{
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnap, &procEntry))
        {
            do
            {
                if (!_stricmp(procEntry.szExeFile, procName))
                {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &procEntry));
        }
    }
    CloseHandle(hSnap);
    return procId;
}

int main()
{
    cout << "Welcome to our DLL-Injector. Make sure you already opened --> League of Legends.exe" << endl;
    cout << "Press enter to choose your DLL" << endl;
    _getch();

    string stringpath = chooseDLL();

    const char* dp = stringpath.c_str();
    cout << "You chose " << dp << endl;

    Sleep(1000);

    cout << "Press enter to inject!" << endl;
    _getch();

    //Gibt den Path des Dll file an
    //const char* dllPath = "C:\\Users\\felix\\source\\repos\\dlltest\\Debug\\dlltest.dll";
    const char* dllPath = dp;

    //procName wird an die Methode GetProcId übergeben
    const char* procName = "League of Legends.exe";
    DWORD procId = 0;
    DWORD baseAddr = 0;

    while (!procId)
    {
        //Methode GetProcId wird aufgerufen
        procId = GetProcId(procName);
        Sleep(30);
    }

    //Nun wird ein Prozess geöffnet, um das Memorywriting zu ermöglichen
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);

    if (hProc && hProc != INVALID_HANDLE_VALUE)
    {
        //gibt die Location an, an der später der Dll Path eingeschleust wird
        void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if (loc)
        {
            //Nun wird der Path in den Ram geschrieben
            WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, 0);
        }

        HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

        //Location des Dll paths
        //std::cout << "Dll path located at: " << std::hex << loc << std::endl;
        //std::cin.get();

        if (hThread)
        {
            CloseHandle(hThread);
        }
    }

    if (hProc)
    {
        CloseHandle(hProc);
    }
    return 0;
}
