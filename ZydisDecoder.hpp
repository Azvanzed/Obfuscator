class CZydisParser
{
private:
	ZydisDecoder Decoder;

public:

	CZydisParser()
	{
		ZydisDecoderInit(&Decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);
	}

	BOOLEAN decodeInstruction(
		PVOID instructionBase, 
		SIZE_T instructionSize, 
		ZydisDecodedInstruction* decodedInstruction, 
		ZydisDecodedOperand* decodedOperands) const
	{
		return ZydisDecoderDecodeFull(&Decoder, instructionBase, instructionSize, 
			decodedInstruction, decodedOperands) == ZYAN_STATUS_SUCCESS;
	}

	BOOLEAN decodeInstruction(
		std::vector<BYTE>& Opcode,
		ZydisDecodedInstruction* decodedInstruction, 
		ZydisDecodedOperand* decodedOperands) const
	{
		return this->decodeInstruction(Opcode.data(), Opcode.size(), decodedInstruction, decodedOperands);
	}

	BOOLEAN disassembleRoutine(
		PVOID routineBase, 
		SIZE_T routineSize, 
		std::vector<CZydisInstruction>& decodedInstructions) const
	{
		ULONG64 routineStart = (ULONG64)routineBase;
		const ULONG64 routineEnd = routineStart + routineSize;
		
		while (routineStart < routineEnd)
		{
			ZydisDecodedInstruction decodedInstruction;
			std::array<ZydisDecodedOperand, ZYDIS_MAX_OPERAND_COUNT> decodedOperands;
			if (!this->decodeInstruction((PVOID)routineStart, routineSize, &decodedInstruction, decodedOperands.data()))
				return FALSE;

			BYTE instructionLength = decodedInstruction.length;
			routineStart += instructionLength;
			routineSize -= instructionLength;

			decodedInstructions.push_back({ decodedInstruction, decodedOperands });
		}

		return TRUE;
	}

	BOOLEAN disassembleRoutine(
		std::vector<BYTE>& Opcode,
		std::vector<CZydisInstruction>& decodedInstructions) const
	{
		return this->disassembleRoutine(Opcode.data(), Opcode.size(), decodedInstructions);
	}

	//ULONG64 getMovValue(
	//	ZydisDecodedInstruction& Instruction, 
	//	ZydisDecodedOperand* Operands) const
	//{
	//	ZydisInstructionSegments Segements;
	//	if (ZydisGetInstructionSegments(&Instruction, &Segements) != ZYAN_STATUS_SUCCESS)
	//		return 0;
	//
	//	for (BYTE i = 0; i < Instruction.operand_count; ++i)
	//	{
	//		ZydisDecodedOperand& Operand = Operands[i];
	//		if (!Operand.type == ZYDIS_OPERAND_TYPE_IMMEDIATE || !Operand.imm.is_relative)
	//			continue;
	//			
	//		for (BYTE x = 0; x < Segements.count; ++x)
	//		{
	//			if (Segements.segments[x].type != ZYDIS_INSTR_SEGMENT_IMMEDIATE)
	//				continue;

	//			// code
	//		}
	//	}

	//	Segements.segments[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE;
	//}
};