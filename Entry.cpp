#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <dbghelp.h>
#include <vector>
#include <chrono>
#include <array>
#include <ShlObj.h>
#include <ShObjIdl.h>
#include <filesystem>

#include <d3d9.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>

#include "GUI/Window.hpp"
#include "GUI/DirectX.hpp"
#include "GUI/Menu.hpp"

#include <Zydis.h>

#include "Defines.hpp"

#include "Utils.hpp"
#include "PE.hpp"
#include "Routine.hpp"

#include "PdbParser.hpp"
#include "ZydisInstruction.hpp"
#include "ZydisDecoder.hpp"

#include "Virtualization.hpp"
#include "Obfuscator.hpp"

VOID onDrawCallback()
{
	static BOOLEAN isFirstFrame{ TRUE };
	if (isFirstFrame)
	{
		ImGui::SetNextWindowPos({ (float)((GetSystemMetrics(SM_CXSCREEN) / 2) - (475 / 2)),
			(float)((GetSystemMetrics(SM_CYSCREEN) / 2) - (140 / 2)) });
		ImGui::SetNextWindowSize({ 475, 140 });
		isFirstFrame = FALSE;
	}

	ImGui::Begin("Obfuscation Engine", NULL, 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);

	static std::string appFilePath, pdbFilePath;

	ImGui::Text("Application");
	ImGui::SetNextItemWidth(394.00f);
	ImGui::InputText("##APP_INPUT", ( CHAR* )appFilePath.c_str(), MAX_PATH);
	ImGui::SameLine();
	ImGui::Dummy({ -13.00f, 0.00f });
	ImGui::SameLine();
	if (ImGui::Button("Open File##APP_BUTTON"))
		appFilePath = Utils::fileDialogBox();
		
	ImGui::Text("PDB");
	ImGui::SetNextItemWidth(394.00f);
	ImGui::InputText("##PDB_INPUT", ( CHAR * )pdbFilePath.c_str(), MAX_PATH);
	ImGui::SameLine();
	ImGui::Dummy({ -13.00f, 0.00f });
	ImGui::SameLine();
	if (ImGui::Button("Open File##PDB_BUTTON"))
		pdbFilePath = Utils::fileDialogBox();
	
	if (ImGui::Button("Run", { 460, 20 }))
		Obfuscator::Run(appFilePath, pdbFilePath);

	ImGui::End();
}

INT main(
	INT argc,
	CHAR** argv)
{
	CWindow Window{ L"Obfuscator", L"Obfuscation Engine" };
	Window.Create();

	CDirectX DirectX{ &Window };

	Menu::drawLoop(&Window, &DirectX, &onDrawCallback);
	
	DirectX.resetDevice();
	Window.Destroy();
	return 0;
}