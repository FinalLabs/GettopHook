#pragma once
#include <iostream>
#include "detours.h"

__forceinline int x(int addy) {
	return (addy - 0x400000 + reinterpret_cast<int>(GetModuleHandleA("Process.exe")));
}

namespace Gettop {
	uintptr_t state = 0;

	int __cdecl gettop_detour(int a1) { //gettop lol 
		state = a1;
		return (*(DWORD*)(a1 + top) - *(DWORD*)(a1 + base)) >> 4;
	}

	int GettopAddr = x(0x1526A90);

	int GetRState() {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&reinterpret_cast<void*&>(GettopAddr), &gettop_detour);
		DetourTransactionCommit();
	
		while (!state) {
			Sleep(0.001f);
		}

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&reinterpret_cast<void*&>(GettopAddr), &gettop_detour);
		DetourTransactionCommit();

		return state;
	}
}
