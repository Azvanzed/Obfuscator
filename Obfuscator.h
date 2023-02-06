namespace Obfuscator
{
	std::vector<std::string> Logs;
	VOID addLog(std::string Message)
	{
		if (ImGui::CalcTextSize( Message.c_str() ).x > 420.00f)
		{
			while (ImGui::CalcTextSize(Message.c_str()).x > 420.00f)
				Message.resize(Message.size() - 1);

			Message += "...";
		}

		Logs.push_back( Message );
	}
	
	BOOLEAN Run(std::string fileName, std::string pdbFileName)
	{
		Obfuscator::addLog("\n========================|Obfuscating|========================");
		std::chrono::time_point startTime = std::chrono::steady_clock::now();

		PE Image{ fileName.c_str()};


		CPdbParser pdbParser{};
		pdbParser.Parse(pdbFileName.c_str());

		CZydisParser zydisParser{};

		Virtualization::processRoutine(Image, zydisParser, pdbParser.Routines[0]);
		//for (const CRoutine& Routine : pdbParser.Routines)
		//{
		//	Virtualization::processRoutine(Image, zydisParser, Routine);
		//	//addLog(std::format("[{:#x}] {}\n", functionAddress, Routine.Name));

		//}
		addLog(std::format("Parsed {} routines.\n", pdbParser.Routines.size()));

		IMAGE_SECTION_HEADER* stubSection = Image.createSection(".stub", 512, 0x60000020);
		Image.Refresh();
		addLog(std::format("Created .stub section.\n", pdbParser.Routines.size()));




		std::chrono::time_point endTime = std::chrono::steady_clock::now();
		
		LONG64 timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
		addLog(std::format("=========================|{:.2f}ms|=========================", timeTaken / 1000.00f));

		Image.fileData.clear();
		return TRUE;
	}
}