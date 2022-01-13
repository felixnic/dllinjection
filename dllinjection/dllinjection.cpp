#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>

using namespace std;

DWORD procId = 0;
DWORD baseAddr = 0;

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

bool getModuleBaseAddress(string module)
{
    HANDLE hHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procId);
    MODULEENTRY32 mentry;
    mentry.dwSize = sizeof(mentry);
    do
    {
        if (!strcmp(mentry.szModule, module.c_str()))
        {
            CloseHandle(hHandle);
            baseAddr = (DWORD)mentry.modBaseAddr;
            return true;
        }
    } while (Module32Next(hHandle, &mentry));
    return false;
}

int main()
{
    const char* dllPath = "C:\\Users\\felix\\Desktop\\dlltest.dll";
    const char* procName = "League of Legends.exe";
    DWORD procId = 0;
    DWORD baseAddr = 0;

    while (!procId)
    {
        procId = GetProcId(procName);
        Sleep(30);
    }

   /* while (!baseAddr)
    {
        baseAddr = getModuleBaseAddress("League of Legends.exe");
        Sleep(10);
    }

    cout << "PID: " << procId << endl << "Base Address: " << baseAddr;*/

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);

    if (hProc && hProc != INVALID_HANDLE_VALUE)
    {
        void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if (loc)
        {
            WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, 0);
        }   

        HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

        std::cout << "Dll path allocated at: " << std::hex << loc << std::endl;
        std::cin.get();

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
