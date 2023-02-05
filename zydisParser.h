class zydisParser
{
private:
	ZydisDecoder Decoder;

public:

	zydisParser()
	{
		ZydisDecoderInit(&Decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);
	}

	BOOLEAN decodeInstruction(std::vector<BYTE>& Opcode, ZydisDecodedInstruction* decodedInstruction, ZydisDecodedOperand* Operands)
	{
		return ZydisDecoderDecodeFull(&Decoder,
			Opcode.data(), Opcode.size(), decodedInstruction, Operands) == ZYAN_STATUS_SUCCESS;
	}

	BOOLEAN isJmpRelative(ZydisDecodedInstruction& Instruction)
	{
		if (!(Instruction.attributes & ZYDIS_ATTRIB_IS_RELATIVE))
			return FALSE;
	
		return Instruction.mnemonic >= ZYDIS_MNEMONIC_JB 
			&& Instruction.mnemonic <= ZYDIS_MNEMONIC_JZ;
	}

	ULONG64 getJmpOffset(ZydisDecodedInstruction& Instruction)
	{
		return isJmpRelative(Instruction) ? Instruction.raw.imm->value.u : 0;
	}

	ULONG64 getMovValue(ZydisDecodedInstruction& Instruction, ZydisDecodedOperand* Operands)
	{
		ZydisInstructionSegments Segements;
		if (ZydisGetInstructionSegments(&Instruction, &Segements) != ZYAN_STATUS_SUCCESS)
			return 0;
	
		for (BYTE i = 0; i < Instruction.operand_count; ++i)
		{
			ZydisDecodedOperand& Operand = Operands[i];
			if (!Operand.type == ZYDIS_OPERAND_TYPE_IMMEDIATE || !Operand.imm.is_relative)
				continue;
				
			for (BYTE x = 0; x < Segements.count; ++x)
			{
				if (Segements.segments[x].type != ZYDIS_INSTR_SEGMENT_IMMEDIATE)
					continue;

				// code
			}
		}

		Segements.segments[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE;
	}
};