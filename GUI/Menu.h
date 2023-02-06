namespace Menu
{
	BOOLEAN drawLoop(CWindow* Window, CDirectX* DirectX, VOID(*onDrawCallback_fn)())
	{
		ImGui::CreateContext();

		ImGui_ImplWin32_Init(Window->Hwnd);
		ImGui_ImplDX9_Init(DirectX->pDevice);

		MSG Msg;
		memset(&Msg, 0, sizeof(MSG));

		ImFontConfig fontCfg;
		fontCfg.PixelSnapH = TRUE;
		fontCfg.OversampleH = 4;
		fontCfg.OversampleV = 5;

		ImGuiIO& Io = ImGui::GetIO();
		Io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 14.00f, &fontCfg);
		
		ImGuiStyle& Style = ImGui::GetStyle();
		Style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(200, 200, 200);
		Style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(255, 255, 255);
		Style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(225, 225, 225);

		Style.ScrollbarSize = 6.00f;

		while (Msg.message != WM_QUIT)
		{
			if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
				continue;
			}

			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			onDrawCallback_fn();

			ImGui::End();
			ImGui::EndFrame();

			DirectX->pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			if (DirectX->pDevice->BeginScene() >= 0)
			{
				ImGui::Render();
				ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
				DirectX->pDevice->EndScene();
			}

			if (DirectX->pDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST 
				&& DirectX->pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
				DirectX->resetDevice();
		}

		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		return TRUE;
	}
}