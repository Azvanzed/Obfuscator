class Pdb
{
private:
	std::vector<Routine> Routines;

public:

	Pdb(PE* Image, const CHAR* pdbFilePath)
	{
		HANDLE currentProcess = GetCurrentProcess();

		if (!SymInitialize(currentProcess, NULL, FALSE))
			return;

		SymLoadModuleEx(currentProcess, NULL, pdbFilePath, 0, 0, std::filesystem::file_size(pdbFilePath), 0, 0);
	}

	BOOLEAN Parse()
	{

	}
};