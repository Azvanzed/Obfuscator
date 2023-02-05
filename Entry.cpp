#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "Utils.h"
#include "PE.h"
#include "Routines.h"
#include "EntryPoint.h"



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

	EntryPoint::addCustomEntry(Image, Stub, &Routines::customEntry);

	/* Spoof entrypoint */
	printf("[>] Spoofed entrypoint to 0x%llx\n", Image.Nt->OptionalHeader.AddressOfEntryPoint);



	std::chrono::time_point endTime = std::chrono::steady_clock::now();
	printf("==============================================================\n");

	LONG64 timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
	printf("[>] Done, took %.2fms.\n", timeTaken / 1000.00f );

	Utils::createFile("Obfuscated.exe", fileData.data(), fileData.size());
	return std::cin.get();
}