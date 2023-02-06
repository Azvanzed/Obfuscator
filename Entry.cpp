#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <dbghelp.h>
#include <vector>
#include <chrono>
#include <ShlObj.h>
#include <ShObjIdl.h>
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

#include "Virtualization.h"
#include "Obfuscator.h"

VOID onDrawCallback()
{
	static BOOLEAN isFirstFrame{ TRUE }, justRanObfuscator{ FALSE };
	if (isFirstFrame)
	{
		ImGui::SetNextWindowPos( { ( float )( ( GetSystemMetrics(SM_CXSCREEN) / 2 ) - (475 / 2)),
			(float)( ( GetSystemMetrics(SM_CYSCREEN) / 2 ) - (535 / 2) )});
		ImGui::SetNextWindowSize({ 475, 535 });
		isFirstFrame = FALSE;
	}
	ImGui::Begin("Obfuscation Engine", NULL, 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);

	static CHAR appFilePath[MAX_PATH], pdbFilePath[MAX_PATH];

	ImGui::Text("Application");
	ImGui::SetNextItemWidth(394.00f);
	ImGui::InputText("###Application", appFilePath, MAX_PATH);
	ImGui::SameLine();
	ImGui::Dummy({ -13.00f, 0.00f });
	ImGui::SameLine();
	if (ImGui::Button("Open File###OPEN_APP"))
	{
		strcpy(appFilePath, Utils::fileDialogBox().c_str());

		std::string Copy = appFilePath;
		Copy.resize(Copy.size() - 3);
		Copy += "pdb";

		Obfuscator::addLog("Application added => " + std::string(appFilePath));
		if (std::filesystem::exists(Copy))
		{
			Obfuscator::addLog("PDB found => " + Copy);
			strcpy(pdbFilePath, Copy.c_str());
		}
	}

	ImGui::Text("PDB");
	ImGui::SetNextItemWidth(394.00f);
	ImGui::InputText("###PDB_INPUT", pdbFilePath, MAX_PATH);
	ImGui::SameLine();
	ImGui::Dummy({ -13.00f, 0.00f });
	ImGui::SameLine();
	if (ImGui::Button("Open File###OPEN_PDB"))
	{
		strcpy(pdbFilePath, Utils::fileDialogBox().c_str());
		Obfuscator::addLog("PDB added => " + std::string(pdbFilePath));
	}
	
	if (ImGui::Button("Run", { 460, 20 }))
	{
		BOOLEAN appExists = std::filesystem::exists(appFilePath);
		if (!appExists)
			Obfuscator::addLog("Application doesn't exists.");

		BOOLEAN pdbExists = std::filesystem::exists(pdbFilePath);
		if (!pdbExists)
			Obfuscator::addLog("PDB doesn't exists.");

		if (appExists && pdbExists)
		{
			Obfuscator::Run(appFilePath, pdbFilePath);
			justRanObfuscator = TRUE;
		}
	}

	ImGui::Dummy({ 0.00f, 35.00f });

	ImGui::Text("Logs:");
	ImGui::BeginChild("Logs", { 460, 334 }, TRUE, 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);

	ImGui::PushStyleColor(ImGuiCol_Text, { 0.80f, 0.80f, 0.80f, 1.00f });
	for (SIZE_T i = 0; i < Obfuscator::Logs.size(); ++i)
		ImGui::Text(Obfuscator::Logs[i].c_str());
	ImGui::PopStyleColor();
	
	if (justRanObfuscator)
	{
		ImGui::SetScrollHereY(1.00f);
		justRanObfuscator = FALSE;
	}
		
	ImGui::EndChild();
	ImGui::End();
}

INT main(
	INT argc,
	CHAR** argv)
{
	/*CZydisParser ok;
	char raw_bytes[] = {0xE9,0x20,0x10,0x00,0x00};
	ZydisDecodedInstruction test;
	ZydisDecodedOperand testop[ZYDIS_MAX_OPERAND_COUNT];
	ok.decodeInstruction((PVOID)raw_bytes, 6, &test, testop);
	std::cout << ZydisMnemonicGetString(test.mnemonic) << std::endl;
	return 0;*/
	CWindow Window{ L"Obfuscator", L"Obfuscation Engine" };
	Window.Create();

	CDirectX DirectX{ &Window };

	Menu::drawLoop(&Window, &DirectX, &onDrawCallback);
	
	DirectX.resetDevice();
	Window.Destroy();
	return 0;
}