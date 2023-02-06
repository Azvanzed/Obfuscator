extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class CWindow
{
private:
    static LRESULT __stdcall WndProc(HWND Hwnd, ULONG32 Msg, WPARAM wParam, LPARAM lParam)
    {
        switch (Msg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            ImGui_ImplWin32_WndProcHandler(Hwnd, Msg, wParam, lParam);
            break;
        }

        return DefWindowProc(Hwnd, Msg, wParam, lParam);
    }

public:

	HWND Hwnd;
    WNDCLASSEX wndClass;

	CWindow(LPCWSTR titleName, LPCWSTR className )
	{
        memset(&wndClass, 0, sizeof(WNDCLASSEX));
        wndClass.cbSize = sizeof(WNDCLASSEX);
        wndClass.style = CS_CLASSDC;
        wndClass.lpfnWndProc = &WndProc;
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = 0;
        wndClass.hInstance = GetModuleHandle( NULL );
        wndClass.hIcon = NULL;
        wndClass.hCursor = NULL;
        wndClass.hbrBackground = NULL;
        wndClass.lpszMenuName = titleName;
        wndClass.lpszClassName = className;
        wndClass.hIconSm = NULL;

        RegisterClassEx(&wndClass);
	}

    BOOLEAN Create()
    {
        this->Hwnd = CreateWindowEx(WS_EX_LAYERED, wndClass.lpszClassName, wndClass.lpszMenuName,
            WS_POPUP, 0, 0, GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, wndClass.hInstance, NULL);
        if (!this->Hwnd)
            return FALSE;

        SetLayeredWindowAttributes(this->Hwnd, RGB(0, 0, 0), 0, ULW_COLORKEY);
        ShowWindow(this->Hwnd, SW_SHOW);
        UpdateWindow(this->Hwnd);
        return TRUE;
    }

    VOID Destroy()
    {
        DestroyWindow(this->Hwnd);
        UnregisterClass(this->wndClass.lpszClassName, this->wndClass.hInstance);
    }
};