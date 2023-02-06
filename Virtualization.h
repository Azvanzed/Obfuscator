namespace Virtualization
{
	BOOLEAN processRoutine( 
		PE& Image,
		const CZydisParser& zydisParser, 
		const CRoutine& Routine)
	{
		IMAGE_SECTION_HEADER* _text = Image.findSection(".text");
		ULONG64 routineBase = (ULONG64)Image.fileData.data() + _text->VirtualAddress + Routine.Offset;

		ZydisDecodedOperand		Operands;
		ZydisDecodedInstruction Instruction;
		if (!zydisParser.decodeInstruction( ( PVOID )routineBase, Routine.Size, &Instruction, &Operands ))
			return FALSE;

		return TRUE;
	}

}