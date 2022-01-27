#include <Windows.h>
#include <stdio.h>
#include <numeric>

using namespace std;


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    DWORD dwProcessBase = (DWORD)GetModuleHandle(NULL);
    DWORD dwBaseAddress = dwProcessBase + 0x2A27B94;
    DWORD dwOffset[] = { 0xfc,0x54 };

    int Health = *(int*)(*(DWORD*)(*(DWORD*)dwProcessBase + dwOffset[0]) + dwOffset[1]);

    int index1 = 1;
    char buf[10];
    _itoa_s(index1, buf, 10);
    MessageBox(NULL, buf, "Heal", MB_OK);

    /*if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        MessageBox(0, L"Hello I am Felix!", L"Hello", MB_ICONINFORMATION);

    return TRUE;*/
}



