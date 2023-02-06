namespace Utils
{
	std::string toStr(std::wstring Wstr)
	{
		return std::string(Wstr.begin(), Wstr.end());
	}

	ULONG64 Align(ULONG64 Address, ULONG32 Alignment)
	{
		return Address + (Alignment - (Address % Alignment));
	}

	std::string fileDialogBox()
	{
		if (!SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
			return std::string();
		
		IFileOpenDialog* pFileOpen;
		if (!SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen))))
		{
			CoUninitialize();
			return std::string();
		}

		if (!SUCCEEDED(pFileOpen->Show(NULL)))
		{
			pFileOpen->Release();
			CoUninitialize();
			return std::string();
		}

		IShellItem* pItem;
		if (!SUCCEEDED(pFileOpen->GetResult(&pItem)))
		{
			pFileOpen->Release();
			CoUninitialize();
			return std::string();
		}

		LPWSTR filePath;
		if (!SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath)))
		{
			pItem->Release();
			pFileOpen->Release();
			CoUninitialize();
			return std::string();
		}

		pItem->Release();
		pFileOpen->Release();
		CoUninitialize();
		return toStr(std::wstring( filePath ));
	}
}