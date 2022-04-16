#include <Windows.h>
#include <numeric>
#include <TlHelp32.h>
#include <iostream>

using namespace std;

void mainHack();

#define DEFINE_RVA(address) ((DWORD)address)

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

enum class Offsets
{
	NewCastSpell = 0x5E5050,
	HudInstance = 0x24a4bc0
};

enum class SpellSlot
{
	Invalid = -1,
	Q = 0,
	W,
	E,
	R,
	Summoner1,
	Summoner2,
	Item_1,
	Item_2,
	Item_3,
	Item_4,
	Item_5,
	Item_6,
	Trinket,
	Recall
};

enum class CastType
{
	NormalCastSpell = 1,
	QuickCastSpell = 2,
	SelfCastSpell = 3,
	QuickCastWithIndicatorSpell = 4,
	QuickCastOrSelfCastSpell = 5,
	QuickCastWithIndicatorOrSelfCastSpell = 6,
};

void CastSpell(SpellSlot slot, CastType castType);

void mainHack()
{
	/*AllocConsole();
	freopen("CONOUT$", "w", stdout);
	cout << "We Can Use Console For Debugging!\n";*/

	DWORD BaseAddress = (DWORD)GetModuleHandle(NULL);
	DWORD* localPlayer = (DWORD*)(BaseAddress + 0x30F5BBC);
	float* health = (float*)(*localPlayer + 0xDB4);

	/*std::cout << 100;
	std::cout << *health;
	std::cout << 100;*/

	while (true)
	{
		if (*health < 300)
		{
			CastSpell(SpellSlot::Summoner1, CastType::SelfCastSpell);
		}
	}

}

void CastSpell(SpellSlot slot, CastType castType)
{
	typedef void(__thiscall* fnnewcastspell)(DWORD hudinstance, SpellSlot spellIndex, CastType castType, float a4);
	static fnnewcastspell CastSpell = (fnnewcastspell)(DEFINE_RVA(Offsets::NewCastSpell));

	DWORD HUDInputLogic = *(DWORD*)(*(DWORD*)DEFINE_RVA(Offsets::HudInstance) + 0x34);
	CastSpell(HUDInputLogic, slot, castType, 0.0f);
}


