namespace Obfuscator
{
	BOOLEAN Run(std::string fileName, std::string pdbFileName)
	{
		PE Image{ fileName.c_str()};

		CPdbParser pdbParser{};
		pdbParser.Parse(pdbFileName.c_str());

		CZydisParser zydisParser{};

		IMAGE_SECTION_HEADER* stubSection = Image.createSection(".stub", 512, 0x60000020);
		Image.Refresh();


		//for (const CRoutine& Routine : pdbParser.Routines)
			Parser::parseRoutine(Image, zydisParser, Routine);



		Image.storeOnDisk("Output.exe");
		Image.fileData.clear();
		return TRUE;
	}
}