namespace Utils
{
	ULONG64 Align(ULONG64 Address, ULONG32 Alignment)
	{
		return Address + (Alignment - (Address % Alignment));
	}
}