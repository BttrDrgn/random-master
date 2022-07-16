#include <sdk/SexySDK.hpp>
#include <callbacks/callbacks.hpp>

bool shutdown = false;
bool give_powerup = false;

void init()
{
	callbacks::on(callbacks::type::beginturn2, []()
	{
		give_powerup = true;
	});

	callbacks::on_peg_hit([](Sexy::Ball* ball, Sexy::PhysObj* phys_obj, bool a4)
	{
		if (give_powerup)
		{
			static std::random_device rd;
			static std::mt19937 mt(rd());
			static std::uniform_int_distribution<int> rand_powerup(0, 13);
			Sexy::LogicMgr::DoPowerup(ball, phys_obj, rand_powerup(mt), 0);
			give_powerup = false;
		}
	});
}

DWORD WINAPI OnAttachImpl(LPVOID lpParameter)
{
	init();
	return 0;
}

DWORD WINAPI OnAttach(LPVOID lpParameter)
{
	__try
	{
		return OnAttachImpl(lpParameter);
	}
	__except (0)
	{
		FreeLibraryAndExitThread((HMODULE)lpParameter, 0xDECEA5ED);
	}

	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, OnAttach, hModule, 0, nullptr);
		return true;
	}

	return false;
}
