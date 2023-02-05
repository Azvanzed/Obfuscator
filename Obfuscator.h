namespace Obfuscator
{
	std::vector<std::string> Logs;
	
	BOOLEAN Run(std::string fileName, std::string pdbFileName)
	{
		PE Image{ fileName.c_str()};
		Logs.push_back(std::format("Loaded {:.2f}kb file to memory.\n", Image.fileData.size() / 1024.00f));

		std::chrono::time_point startTime = std::chrono::steady_clock::now();

		IMAGE_SECTION_HEADER* stubSection = Image.createSection(".stub", 512, 0x60000020);
		Image.Refresh();

		CPdbParser pdbParser{};
		pdbParser.Parse(pdbFileName.c_str());

		IMAGE_SECTION_HEADER* textSection = Image.findSection(".text");
		for (CRoutine Routine : pdbParser.Routines)
		{
			ULONG64 functionAddress = (ULONG64)Image.fileData.data() + textSection->VirtualAddress + Routine.Offset;
			Logs.push_back(std::format("[{:#x}]  {}\n", functionAddress, Routine.Name));
		}
		Logs.push_back(std::format("Parsed {} routines.\n", pdbParser.Routines.size()));

		std::chrono::time_point endTime = std::chrono::steady_clock::now();
		
		LONG64 timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
		Logs.push_back(std::format("Done, took {:.2f}ms.\n", timeTaken / 1000.00f));

		Image.fileData.clear();
		return TRUE;
	}
}