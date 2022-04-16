#include <Windows.h>
#include <numeric>
#include <TlHelp32.h>
#include <iostream>

void mainHack();

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hModule);		//disables attach and detach notifications
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)mainHack, NULL, NULL, NULL);	//creates a new thread and starts at function mainHack()
	}
	else if (dwReason == DLL_PROCESS_DETACH) {

	}
	return TRUE;
}

void mainHack()
{
	/*AllocConsole();
	freopen("CONOUT$", "w", stdout);
	std::cout << "We Can Use Console For Debugging!\n";

	DWORD BaseAddress = (DWORD)GetModuleHandle(NULL);
	DWORD* localPlayer = (DWORD*)(BaseAddress+0x31168D4);
	int* health = (int*)(*localPlayer + 0xDC4);

	std::cout << 100;
	std::cout << *health;
	std::cout << 100;*/

}