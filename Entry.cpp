#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <dbghelp.h>
#include <vector>
#include <chrono>
#include <filesystem>

#include <Zydis.h>

#include "Utils.h"
#include "PE.h"
#include "Routine.h"

#include "PdbParser.h"
#include "Parser.h"

INT main(
	INT argc,
	CHAR** argv)
{
#pragma region test
	std::vector<BYTE> raw_bytes = { 0xE9,0x10,0x20,0x00,0x00 };
	zydisParser ok;
	ZydisDecodedInstruction wa;
	ZydisDecodedOperand ws[ZYDIS_MAX_OPERAND_COUNT];
	ok.decodeInstruction(raw_bytes,&wa,ws);
	std::cout << std::hex << "0x" << ok.getJmpOffset(wa, ws) << std::endl;
	return 0;
#pragma endregion test
	// end of test
	PE Image{ "Default.exe" };
	printf("[+] Loaded %.2fkb file to memory\n", (float)Image.fileData.size() / 1024.00f);

	printf("==============================================================\n");
	std::chrono::time_point startTime = std::chrono::steady_clock::now();


	IMAGE_SECTION_HEADER* Stub = Image.createSection(".stub", 512, 0x60000020);
	Image.Refresh();

	std::string pdbFilePath = Image.filePath;
	pdbFilePath.resize(pdbFilePath.size() - 3);
	pdbFilePath += "pdb";

	CPdbParser pdbParser{};
	pdbParser.Parse(pdbFilePath.c_str());

	for (CRoutine Routine : pdbParser.Routines)
	{
		printf("[0x%x] %s\n", Routine.Offset, Routine.Name);
	}


	std::chrono::time_point endTime = std::chrono::steady_clock::now();
	printf("==============================================================\n");

	LONG64 timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
	printf("[>] Done, took %.2fms.\n", timeTaken / 1000.00f);


	return std::cin.get();
}