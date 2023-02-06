namespace Obfuscator
{
	BOOLEAN Run(std::string fileName, std::string pdbFileName)
	{
		std::chrono::time_point startTime = std::chrono::steady_clock::now();

		PE Image{ fileName.c_str()};

		CPdbParser pdbParser{};
		pdbParser.Parse(pdbFileName.c_str());

		CZydisParser zydisParser{};

		for (const CRoutine& Routine : pdbParser.Routines)
			Virtualization::processRoutine(Image, zydisParser, Routine);

		IMAGE_SECTION_HEADER* stubSection = Image.createSection(".stub", 512, 0x60000020);
		Image.Refresh();






		std::chrono::time_point endTime = std::chrono::steady_clock::now();
		
		LONG64 timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
	
		Image.fileData.clear();
		return TRUE;
	}
}