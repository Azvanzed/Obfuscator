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

#include "Pdb.h"
#include "Parser.h"

INT main(
	INT argc,
	CHAR** argv)
{
	PE Image{ "Default.exe" };
	printf("[+] Loaded %.2fkb file to memory\n", (float)Image.fileData.size() / 1024.00f);

	printf("==============================================================\n");
	std::chrono::time_point startTime = std::chrono::steady_clock::now();


	IMAGE_SECTION_HEADER* Stub = Image.createSection(".stub", 512, 0x60000020);
	Image.Refresh();
	

	std::chrono::time_point endTime = std::chrono::steady_clock::now();
	printf("==============================================================\n");

	LONG64 timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
	printf("[>] Done, took %.2fms.\n", timeTaken / 1000.00f );


	return std::cin.get();
}