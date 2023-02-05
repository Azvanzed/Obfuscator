#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <dbghelp.h>
#include <vector>
#include <chrono>
#include <filesystem>

#include <Zydis.h>

#include "Defines.h"

#include "Utils.h"
#include "PE.h"
#include "Routine.h"

#include "PdbParser.h"
#include "ZydisParser.h"

#include <d3d9.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>

#include "GUI/Window.h"
#include "GUI/DirectX.h"
#include "GUI/Menu.h"

VOID onDrawCallback()
{
	ImGui::SetNextWindowSize({ 475, 535 });
	ImGui::Begin("Obfuscation Engine", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);



	ImGui::End();
}

INT main(
	INT argc,
	CHAR** argv)
{
	std::vector<BYTE> raw_bytes = { 0xE9,0x10,0x20,0x00,0x00 };
	zydisParser ok;
	ZydisDecodedInstruction wa;
	ZydisDecodedOperand ws[ZYDIS_MAX_OPERAND_COUNT];
	ok.decodeInstruction(raw_bytes,&wa,ws);
	std::cout << std::hex << "0x" << ok.getJmpOffset(wa) << std::endl;
	return 0;

	// end of test
	CWindow Window{ L"Window Class", L"Window Title" };
	Window.Create(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2, 500, 700);

	CDirectX DirectX{ &Window };

	Menu::drawLoop(&Window, &DirectX, &onDrawCallback);
	
	DirectX.resetDevice();
	Window.Destroy();

	Sleep(-1);

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