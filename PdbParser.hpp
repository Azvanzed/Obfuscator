class CPdbParser
{
private:
	typedef struct _SYMBOL_ENUM_CTX
	{
		ULONG64 pdbBase;
		PVOID	Collector;
	}SYMBOL_ENUM_CTX, *PSYMBOL_ENUM_CTX;

	static BOOL __stdcall symbolEnum(SYMBOL_INFO* symbolInfo, ULONG symbolSize, SYMBOL_ENUM_CTX* pUserCtx)
	{
		CRoutine newRoutine;
		strcpy(newRoutine.Name, symbolInfo->Name);
		newRoutine.Size = symbolInfo->Size;

		if (!SymGetTypeInfo(GetCurrentProcess(), pUserCtx->pdbBase, symbolInfo->Index, TI_GET_OFFSET, &newRoutine.Offset)
			&& !SymGetTypeInfo(GetCurrentProcess(), pUserCtx->pdbBase, symbolInfo->Index, TI_GET_ADDRESSOFFSET, &newRoutine.Offset))
			return FALSE;

		std::vector<CRoutine>* Routines = (std::vector<CRoutine>*)pUserCtx->Collector;
		if (std::find_if(Routines->begin(), Routines->end(),
			[&](CRoutine Routine)
			{
				return Routine.Offset == newRoutine.Offset;
			}
		) == Routines->end())
		{
			((std::vector<CRoutine>*)pUserCtx->Collector)->push_back(newRoutine);
		}

		return TRUE;
	}


public:
	std::vector<CRoutine> Routines;

	CPdbParser()
	{
		SymInitialize(GetCurrentProcess(), NULL, FALSE);
	}

	BOOLEAN Parse(const CHAR* pdbFilePath)
	{
		ULONG64 pdbBase = SymLoadModuleEx(GetCurrentProcess(), NULL, pdbFilePath, 0, 0x10000000, std::filesystem::file_size(pdbFilePath), NULL, 0);
		if (!pdbBase)
			return FALSE;
		
		SYMBOL_ENUM_CTX Ctx;
		Ctx.pdbBase		= pdbBase;
		Ctx.Collector	= &Routines;

		if (!SymEnumSymbols(GetCurrentProcess(), pdbBase, NULL, (PSYM_ENUMERATESYMBOLS_CALLBACK)&symbolEnum, &Ctx))
		{
			SymUnloadModule64(GetCurrentProcess(), pdbBase);
			return FALSE;
		}

		return SymUnloadModule64(GetCurrentProcess(), pdbBase);
	}

	~CPdbParser()
	{
		SymCleanup(GetCurrentProcess());
		CloseHandle(GetCurrentProcess());
	}
};