#include <Windows.h>
#include <stdio.h>

#include <string>

#include "sdk/class.h"
#include "hook/hook.h"


//#define DEBUG_NO_HOOK 

void SetupConsole()
{
	AllocConsole();
	freopen("CONOUT$", "wb", stdout);
	freopen("CONOUT$", "wb", stderr);
	freopen("CONIN$", "rb", stdin);
}


std::string get_dll_dir(HMODULE module) {
	char path[MAX_PATH];
	GetModuleFileNameA(module, path, sizeof(path));
	std::string buffer = std::string(path);
	size_t last_slash = buffer.find_last_of("\\");
	buffer = buffer.substr(0, last_slash + 1);
	return buffer;
}

DWORD WINAPI main_thread(LPVOID param)
{
	HMODULE h_module = static_cast<HMODULE>(param);

	SetConsoleTitle("DEBUG [x]");

	sdk::Initialize();
#ifdef DEBUG_NO_HOOK
#else
	Hook::Hook(FindWindowA(0, "Deceit"));
#endif
	while (true) {
		Sleep(10);
		if (GetAsyncKeyState(VK_DELETE))
			break;
	}

	printf("unhooked \n");
	FreeLibraryAndExitThread(h_module, 0);
	return TRUE;
}


typedef LONG(NTAPI* NtSuspendProcess)(IN HANDLE ProcessHandle);

void suspend()
{
	HANDLE processHandle = GetCurrentProcess();


	NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(
		GetModuleHandle("ntdll"), "NtSuspendProcess");

	pfnNtSuspendProcess(processHandle);
	CloseHandle(processHandle);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	HANDLE hThread;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		SetupConsole();
		hThread = CreateThread(0, 0, main_thread, hModule, 0, NULL);
		CloseHandle(hThread);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
#ifdef DEBUG_NO_HOOK
#else
		Hook::UnHook();
#endif
		FreeConsole();
		break;
	}
	return TRUE;
}