class PE
{
public:
	std::vector<BYTE>* fileData;
	IMAGE_NT_HEADERS* Nt;
	ULONG64 imageBase;

	PE(std::vector<BYTE>* fileData)
		: fileData(fileData)
	{
		this->Refresh();
	}

	VOID Refresh()
	{
		this->imageBase = (ULONG64)fileData->data();
		this->Nt = (IMAGE_NT_HEADERS*)(this->imageBase + ( (IMAGE_DOS_HEADER*)this->imageBase)->e_lfanew);
	}

	IMAGE_SECTION_HEADER* createSection(
		const CHAR* sectionName,
		ULONG32 sectionSize,
		ULONG32 Characteristics)
	{
		IMAGE_SECTION_HEADER* lastSection = &IMAGE_FIRST_SECTION(Nt)[Nt->FileHeader.NumberOfSections - 1];
		IMAGE_SECTION_HEADER* newSection = (IMAGE_SECTION_HEADER*)((ULONG64)&lastSection->Characteristics + 4);

		memcpy(newSection->Name, sectionName, strlen(sectionName) + 1);

		newSection->Misc.VirtualSize = Utils::Align(sectionSize + 5, Nt->OptionalHeader.SectionAlignment);
		newSection->VirtualAddress = Utils::Align(lastSection->VirtualAddress + lastSection->Misc.VirtualSize, Nt->OptionalHeader.SectionAlignment);
		newSection->SizeOfRawData = Utils::Align(sectionSize + 5, Nt->OptionalHeader.FileAlignment);
		newSection->PointerToRawData = Utils::Align(lastSection->PointerToRawData + lastSection->SizeOfRawData, Nt->OptionalHeader.FileAlignment);
		newSection->Characteristics = Characteristics;
		newSection->PointerToRelocations = 0;
		newSection->PointerToLinenumbers = 0;
		newSection->NumberOfRelocations = 0;
		newSection->NumberOfLinenumbers = 0;

		Nt->FileHeader.NumberOfSections += 1;
		Nt->OptionalHeader.SizeOfImage =
			Utils::Align(Nt->OptionalHeader.SizeOfImage + sectionSize + 5 + sizeof(IMAGE_SECTION_HEADER), Nt->OptionalHeader.SectionAlignment);
		Nt->OptionalHeader.SizeOfHeaders =
			Utils::Align(Nt->OptionalHeader.SizeOfHeaders + sizeof(IMAGE_SECTION_HEADER), Nt->OptionalHeader.FileAlignment);

		fileData->resize(Nt->OptionalHeader.SizeOfImage);
		return newSection;
	}
};