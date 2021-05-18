# Tenable Pwntown 4 Write Up

## Details:
Points: 200

Jeopardy style CTF

Category: Reverse Engineering

Comment:


I've been trying to make a video game. It's pretty rough around the edges right now. Character sprites don't always cooperate, one of the character classes can't even attack yet, but I'm pretty proud of it so far. I made it using Unity, a starter package called uMMORPG, and a bunch of assets from the public domain.

It's really difficult, but I'm sure a hacker such as yourself won't have any problem figuring it out. ;)

The flag for this challenge can be obtained by completing Questy McQuestgiver's quest.

Hint:

Questy McQuestgiver is too wise to fall for your tricks. Did you think you could just lie to him? Did you think you could just take the scroll without actually killing all those saplings? Think again. That said... he did spill the beans on what the prize is... I wonder if there's another way to obtain it?

## Write up:

This last pwntown hacking was really the only game hacking among these challenges. For this I used the metadata from the game files to create a dummy dll using ill2cpp dumper. I then used dnspy to look through until I found the item mail unlock function. I then hooked that function and bought the monster scroll rather than the potion. Using the scroll got the flag.

``` c++
#include "Hack.h"


typedef int(__fastcall *tItemBuy)(void*, unsigned int, unsigned int);

tItemBuy itemBuyGateway;

int __fastcall buyCall(void* ecx, void* edx, unsigned int a2, unsigned int a3)
{

	return itemBuyGateway(ecx, 3, 0);
}

void Pwntown(HANDLE h, HMODULE hMod)
{
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);


	Hook itemBuyHook((BYTE*)(((uintptr_t)GetModuleHandleW(L"GameAssembly.dll") + 0x251E40)), (BYTE*)buyCall, (BYTE*)&itemBuyGateway, 15);

	itemBuyHook.Enable();

	while (true)
	{
		if (GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_ESCAPE)) { break; }
	}

	itemBuyHook.Disable();
	fclose(f);
	FreeConsole();
}
```