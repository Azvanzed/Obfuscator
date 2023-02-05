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
#include "zydisParser.h"

#include <d3d9.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>

#include "GUI/Window.h"
#include "GUI/DirectX.h"
#include "GUI/Menu.h"

VOID onDrawCallback()
{
	ImGui::SetNextWindowSize({ 400, 600 });
	ImGui::Begin("Obfuscation Engine", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);



	ImGui::End();
}

INT main(
	INT argc,
	CHAR** argv)
{
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


	std::chrono::time_point endTime = std::chrono::steady_clock::now();
	printf("==============================================================\n");

	LONG64 timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
	printf("[>] Done, took %.2fms.\n", timeTaken / 1000.00f );


	return std::cin.get();
}