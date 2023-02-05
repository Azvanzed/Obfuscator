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
		while (*(BYTE*)((ULONG64)this->routineAddress + Size) != 0xC3)
			++Size;

		return Size;
	}
};