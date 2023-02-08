namespace Parser
{
	BOOLEAN parseRoutine(
		PE& Image,
		const CZydisParser& zydisParser, 
		const CRoutine& Routine)
	{
		printf("%s:\n", Routine.Name);

		IMAGE_SECTION_HEADER* sectionText = Image.findSection(".text");
		ULONG64 routineBase = (ULONG64)Image.fileData.data() + sectionText->PointerToRawData + Routine.Offset;

		std::vector<CZydisInstruction> Instructions;
		if (!zydisParser.disassembleRoutine((PVOID)routineBase, Routine.Size, Instructions))
			return FALSE;

		IMAGE_SECTION_HEADER* stubSection = Image.findSection(".stub");
		ULONG64 stubBase = ( ULONG64 )Image.fileData.data() + stubSection->PointerToRawData;

		for (const CZydisInstruction& Instruction : Instructions)
		{
			switch (Instruction.Decoded.mnemonic)
			{
			case ZYDIS_MNEMONIC_JMP:
				if (Instruction.Operands[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE
					&& Instruction.Operands[0].size == 32) // 4 bytes
				{
					ULONG64 jmpSrc = routineBase + Instruction.Offset;
					printf("jmpSrc: 0x%llx\n", jmpSrc - (ULONG64)Image.fileData.data());

					ULONG64 jmpDst = jmpSrc + 5 + Instruction.Decoded.raw.imm->value.u;
					printf("jmpDst: 0x%llx\n", jmpDst - (ULONG64)Image.fileData.data());

					ULONG64 jmpRva = jmpDst - (jmpSrc + 5);
					printf("jmpRva: 0x%x\n", jmpRva);

					 d

					*(LONG*)(jmpSrc + 1) = jmpRva;
					memcpy((PVOID)stubBase, (PVOID)jmpDst, Routine.Size);

					return TRUE;
				}

				break;

			default:
				break;
			}
		}

		return FALSE;
	}

}