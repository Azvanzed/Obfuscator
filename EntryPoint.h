namespace EntryPoint
{
	VOID addCustomEntry(PE& Image, IMAGE_SECTION_HEADER* Stub, PVOID customEntry)
	{
		Routine entryRoutine{ customEntry };
		memcpy((PVOID)(Image.imageBase + Stub->PointerToRawData), customEntry, entryRoutine.getSize());
		
		Image.Nt->OptionalHeader.CheckSum = Image.Nt->OptionalHeader.AddressOfEntryPoint;
		Image.Nt->OptionalHeader.AddressOfEntryPoint = Stub->VirtualAddress;
	}
}