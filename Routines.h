#pragma runtime_checks( "", off )
#pragma optimize( "", off )
namespace Routines
{
	ULONG32 __stdcall customEntry(ULONG32 argc, CHAR** argv)
	{
		PEB* Peb = (PEB*)__readgsqword(0x60);

		LIST_ENTRY Head = Peb->Ldr->InMemoryOrderModuleList;
		for (LIST_ENTRY Current = Head; Current.Flink != &Peb->Ldr->InMemoryOrderModuleList; Current = *Current.Flink)
		{
			LDR_DATA_TABLE_ENTRY* Module = (LDR_DATA_TABLE_ENTRY*)(CONTAINING_RECORD(Current.Flink, PEB_LDR_DATA, InMemoryOrderModuleList));



		}

		ULONG64 imageBase = (ULONG64)Peb->ImageBaseAddress;
		IMAGE_DOS_HEADER* Dos = (IMAGE_DOS_HEADER*)imageBase;
		IMAGE_NT_HEADERS* Nt = (IMAGE_NT_HEADERS*)(imageBase + Dos->e_lfanew);

		ULONG64 entryPoint = imageBase + Nt->OptionalHeader.CheckSum;
		return ((ULONG32(__stdcall*)(ULONG32, CHAR**))entryPoint)(argc, argv);
	}
}
#pragma runtime_checks( "", on )
#pragma optimize( "", on )