class PE
{
public:
	CHAR				filePath[MAX_PATH];
	std::vector<BYTE>	fileData;
	IMAGE_NT_HEADERS*	Nt;
	ULONG64				imageBase;

	PE(const CHAR* fileName)
	{
		GetFullPathNameA(fileName, MAX_PATH, this->filePath, NULL);

		std::ifstream fileStream(this->filePath, std::ios::binary);
		fileData.assign((std::istreambuf_iterator<CHAR>(fileStream)), std::istreambuf_iterator<CHAR>());
		fileStream.close();

		this->Refresh();
	}

	VOID storeOnDisk(const CHAR* fileName)
	{
		std::ofstream fileStream(fileName, std::ios_base::out | std::ios_base::binary);
		fileStream.write((CHAR*)this->fileData.data(), this->fileData.size());
		fileStream.close();
	}

	VOID Refresh()
	{
		this->imageBase = (ULONG64)fileData.data();
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

		fileData.resize(Nt->OptionalHeader.SizeOfImage);
		return newSection;
	}

	IMAGE_SECTION_HEADER* findSection(const CHAR* sectionName)
	{
		IMAGE_SECTION_HEADER* Section = IMAGE_FIRST_SECTION(this->Nt );
		for (USHORT i = 0; i < this->Nt->FileHeader.NumberOfSections; ++i, ++Section)
		{
			if (strcmp((CHAR*)Section->Name, sectionName))
				continue;

			return Section;
		}

		return NULL;
	}
};