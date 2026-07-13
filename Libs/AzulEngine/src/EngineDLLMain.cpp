BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved)
{
	AZUL_UNUSED_VAR(hModule);
	AZUL_UNUSED_VAR(lpReserved);

	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			Trace::out("DLL_PROCESS_ATTACHED --> AzulEngineDLL\n");
			break;
		case DLL_PROCESS_DETACH:
			Trace::out("AzulEngineDLL     <-- DLL_PROCESS_DETACHED\n");
			break;
	}
	return TRUE;
}

// ---  End of File ---
