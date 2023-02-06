namespace Virtualization
{
	BOOLEAN processRoutine( 
		PE& Image,
		const CZydisParser& zydisParser, 
		const CRoutine& Routine)
	{
		printf("Disassembling %s:\n", Routine.Name);

		IMAGE_SECTION_HEADER* _text = Image.findSection(".text");
		ULONG64 routineBase = (ULONG64)Image.fileData.data() + _text->PointerToRawData + Routine.Offset;

		std::vector<CZydisInstruction> Instructions;
		if (!zydisParser.disassembleRoutine((PVOID)routineBase, Routine.Size, Instructions))
			return FALSE;

		for (const CZydisInstruction& Instruction : Instructions)
		{
			printf("       %s\n", ZydisMnemonicGetString(Instruction.Decoded.mnemonic));
		}

		return TRUE;
	}

}