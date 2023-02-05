#include <windows.h>
#include <intrin.h>
#include <tlhelp32.h>
#include <iostream>
#include <fstream>
#include <functional>
#include <Psapi.h>
#include <DbgHelp.h>
#include <random>
#include <vector>
#include <string>
#include <chrono>

#include "Defines.h"
#include "Utils.h"

#include "PE.h"
#include "Routine.h"

ULONG32 entryStub(ULONG32 argc, CHAR** argv)
{
	PEB* Peb = ( PEB * )__readgsqword(0x60);
	
	LIST_ENTRY Head = Peb->Ldr->InMemoryOrderModuleList;
	for (LIST_ENTRY Current = Head; Current.Flink != &Peb->Ldr->InMemoryOrderModuleList; Current = *Current.Flink)
	{
		LDR_DATA_TABLE_ENTRY* Module = (LDR_DATA_TABLE_ENTRY*)(CONTAINING_RECORD(Current.Flink, PEB_LDR_DATA, InMemoryOrderModuleList));

		

	}

	ULONG64 imageBase = ( ULONG64 )Peb->ImageBaseAddress;
	IMAGE_DOS_HEADER* Dos = (IMAGE_DOS_HEADER*)imageBase;
	IMAGE_NT_HEADERS* Nt = (IMAGE_NT_HEADERS*)(imageBase + Dos->e_lfanew);

	ULONG64 entryPoint = imageBase + Nt->OptionalHeader.CheckSum;
	return ((ULONG32(*)(ULONG32, CHAR**))entryPoint)(argc, argv);
}

INT main(
	INT argc,
	CHAR** argv)
{
	if (argc < 1)
		return 1;

	std::vector<BYTE> fileData;
	Utils::readFile(argv[1], &fileData);
	printf("[+] Loaded %.2fkb file to memory\n", (float)fileData.size() / 1024.00f);

	printf("==============================================================\n");
	std::chrono::time_point startTime = std::chrono::steady_clock::now();



	PE Image{ &fileData };
	IMAGE_SECTION_HEADER* Stub = Image.createSection(".stub", 512, 0x60000020);
	Image.Refresh();


	/* Spoof entrypoint */
	Routine entryRoutine{ &entryStub };
	memcpy((PVOID)(Image.imageBase + Stub->PointerToRawData), &entryStub, entryRoutine.getSize());
	Image.Nt->OptionalHeader.CheckSum = Image.Nt->OptionalHeader.AddressOfEntryPoint;
	Image.Nt->OptionalHeader.AddressOfEntryPoint = Stub->VirtualAddress;
	printf("[>] Spoofed entrypoint to 0x%llx\n", Image.Nt->OptionalHeader.AddressOfEntryPoint);



	std::chrono::time_point endTime = std::chrono::steady_clock::now();
	printf("==============================================================\n");

	LONG64 timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
	printf("[>] Done, took %.2fms.\n", timeTaken / 1000.00f );

	Utils::createFile("Obfuscated.exe", fileData.data(), fileData.size());
	return std::cin.get();
}