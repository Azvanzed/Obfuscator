class Routine
{
private:
	PVOID routineAddress;

public:

	Routine(PVOID routineAddress)
		: routineAddress(routineAddress)
	{
	}

	ULONG32 getSize()
	{
		ULONG32 Size = 0;
		while (*(BYTE*)((ULONG64)this->routineAddress + Size) != 0xCC)
			++Size;

		return Size;
	}

	VOID setDWORD(ULONG32 Value, ULONG32 newValue)
	{
		ULONG32 Size = this->getSize();

		for (BYTE* Current = (BYTE*)routineAddress; 
			(ULONG64)Current < (ULONG64)routineAddress + Size; 
			++Current)
		{
			if (Current[0] != *(BYTE*)((ULONG64)&Value)
				|| Current[1] != *(BYTE*)((ULONG64)&Value + 1)
				|| Current[2] != *(BYTE*)((ULONG64)&Value + 2)
				|| Current[3] != *(BYTE*)((ULONG64)&Value + 3))
				continue;


			DWORD newState{ PAGE_EXECUTE_READWRITE }, oldState;
			VirtualProtect((PVOID)Current, 4, newState, &oldState);
			*(ULONG32*)Current = newValue;
			VirtualProtect((PVOID)Current, 4, oldState, &newState);
		}
	}
};