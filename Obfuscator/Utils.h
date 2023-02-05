namespace Utils
{
	VOID createFile(const CHAR* fileName, PVOID dataBase, SIZE_T dataSize)
	{
		std::ofstream fileStream(fileName, std::ios_base::out | std::ios_base::binary);
		fileStream.write((CHAR*)dataBase, dataSize);
		fileStream.close();
	}

	VOID readFile(const CHAR* fileName, std::vector<BYTE>* fileData)
	{
		std::ifstream fileStream(fileName, std::ios::binary);
		fileData->assign((std::istreambuf_iterator<CHAR>(fileStream)), std::istreambuf_iterator<CHAR>());
		fileStream.close();
	}

	ULONG64 Align(ULONG64 Address, ULONG32 Alignment)
	{
		return Address + (Alignment - (Address % Alignment));
	}
}