#include <Windows.h>
#include <numeric>
#include <TlHelp32.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    //Offsets sind leider nicht richtig, da diese sich pro update ändern. Deshalb wird in der Messagebox auch nur 0 ausgegeben.
    DWORD ProcessBase = (DWORD)GetModuleHandle(NULL);
    DWORD BaseAddress = ProcessBase + 0x2A27B94;
    DWORD Offset[] = { 0xfc,0x54 };

    const char* procName = "League of Legends.exe";
    DWORD procId = 0;

    int healthAddress = BaseAddress + Offset[0] + Offset[1];
    int health = 0;

    HANDLE pHandle = OpenProcess(PROCESS_VM_READ, FALSE, procId);

    ReadProcessMemory(pHandle, (LPVOID)healthAddress, &health, sizeof(int), NULL);

    //int wird in char convertiert, um die Zahl in der Messagebox ausgeben zu können.
    int ophealth = health;
    char buf[10];
    _itoa_s(ophealth, buf, 10);
    MessageBox(NULL, buf, "Heal", MB_OK);

    /* if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        MessageBox(0, L"Hello I am Felix!", L"Hello", MB_ICONINFORMATION);

        return TRUE;*/
}


