#include <Windows.h>
#include <numeric>
#include <TlHelp32.h>
#include <iostream>

using namespace std;

void mainHack();

#define baseAddress (DWORD)GetModuleHandleA(NULL)
//Das ist eine Macro die zu der eingegebenen Addresse automatisch die baseAddress addiert.
#define DEFINE_RVA(address) (baseAddress + (DWORD)address)

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) {
		//Deaktiviert DLL_THREAD_ATTACH und DLL_THREAD_DETACH Benachrichtigungen
		DisableThreadLibraryCalls(hModule);		
		//Erstellt einen neuen Thread und startet die mainHack Funktion
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)mainHack, NULL, NULL, NULL);
	}
	else if (dwReason == DLL_PROCESS_DETACH) {

	}
	return TRUE;
}

//Alle SpellSlots, die es in League of Legends gibt --> Localplayer
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

//Alle Arten von Spellcasts in League of Legends --> vom Cheatforum
//Wir verwenden in unserem Fall QuickCastSpell.
enum class CastType
{
	NormalCastSpell = 1,
	QuickCastSpell = 2,
	SelfCastSpell = 3,
	QuickCastWithIndicatorSpell = 4,
	QuickCastOrSelfCastSpell = 5,
	QuickCastWithIndicatorOrSelfCastSpell = 6,
};

//Diese Methode ist für den Healspell cast verantwortlich.
void CastSpell(SpellSlot slot, CastType castType)
{
	typedef void(__thiscall* fnnewcastspell)(DWORD hudinstance, SpellSlot spellIndex, CastType castType, float a4);
	//NewCastSpell = 0x5E5050
	static fnnewcastspell CastSpell = (fnnewcastspell)(DEFINE_RVA(0x5E5050));

	//HudInstance = 0x24a4bc0
	DWORD HUDInputLogic = *(DWORD*)(*(DWORD*)DEFINE_RVA(0x24a4bc0) + 0x34);
	CastSpell(HUDInputLogic, slot, castType, 0.0f);
}

void mainHack()
{
	//Pointet auf die health address --> *health = value
	DWORD* localPlayer = (DWORD*)(baseAddress + 0x30F5BBC);
	//health offset = 0xDB4
	float* health = (float*)(*localPlayer + 0xDB4);

	bool finished = false;
	
	do
	{
		//Wenn der Spieler weniger als 300 Leben hat, wird der Spell aktiviert.
		if (*health < 300)
		{
			//Methode wird aufgerufen und der boolean auf true gesetzt.
			CastSpell(SpellSlot::Summoner1, CastType::QuickCastSpell);
			finished = true;
		}
	} while (finished == false);
}




