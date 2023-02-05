class zydisParser
{
private:
	ZydisDecoder Decoder;

public:

	zydisParser()
	{
		ZydisDecoderInit(&Decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);
	}

	BOOLEAN decodeInstruction(std::vector<BYTE>& Opcode, ZydisDecodedInstruction* decodedInstruction, ZydisDecodedOperand* operands)
	{
		return ZydisDecoderDecodeFull(&Decoder,
			Opcode.data(), Opcode.size(), decodedInstruction, operands) == ZYAN_STATUS_SUCCESS;
	}
	bool isJmpRelative(ZydisDecodedInstruction& instr)
	{
		if (!(instr.attributes & ZYDIS_ATTRIB_IS_RELATIVE))
			return false;
		switch (instr.mnemonic)
		{
		case ZYDIS_MNEMONIC_JNBE:
		case ZYDIS_MNEMONIC_JB:
		case ZYDIS_MNEMONIC_JBE:
		case ZYDIS_MNEMONIC_JCXZ:
		case ZYDIS_MNEMONIC_JECXZ:
		case ZYDIS_MNEMONIC_JKNZD:
		case ZYDIS_MNEMONIC_JKZD:
		case ZYDIS_MNEMONIC_JL:
		case ZYDIS_MNEMONIC_JLE:
		case ZYDIS_MNEMONIC_JNB:
		case ZYDIS_MNEMONIC_JNL:
		case ZYDIS_MNEMONIC_JNLE:
		case ZYDIS_MNEMONIC_JNO:
		case ZYDIS_MNEMONIC_JNP:
		case ZYDIS_MNEMONIC_JNS:
		case ZYDIS_MNEMONIC_JNZ:
		case ZYDIS_MNEMONIC_JO:
		case ZYDIS_MNEMONIC_JP:
		case ZYDIS_MNEMONIC_JRCXZ:
		case ZYDIS_MNEMONIC_JS:
		case ZYDIS_MNEMONIC_JZ:
		case ZYDIS_MNEMONIC_JMP:
			return true;
		default:
			return false;
		}
	}
	BOOLEAN getJmpOffset(ZydisDecodedInstruction& instr, ZydisDecodedOperand* operands,uint64_t& jmp_offset)
	{
		if (isJmpRelative(instr))
			return NULL;
		return instr.raw.imm->value.u;
		ZydisInstructionSegments instrSegments;
		ZydisGetInstructionSegments(&instr, &instrSegments);
		for (int i = 0; i < instr.operand_count; ++i)
		{
			auto& op = operands[i];
			if (op.type == ZYDIS_OPERAND_TYPE_IMMEDIATE && op.imm.is_relative)
			{
				for (int j = 0; j < instrSegments.count; ++j)
				{
					auto seg = instrSegments.segments[j];
					if (seg.type == ZYDIS_INSTR_SEGMENT_IMMEDIATE)
					{
						// get mov immediate stuff
					}
				}
			}
		}
		instrSegments.segments[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE;
	}
	/*BOOLEAN encodeInstruction(ZydisDecodedInstruction encoded_instr)
	{
		ZydisEncoderRequest req;
		ZydisEncoderEncodeInstruction
		req.mnemonic = encoded_instr.mnemonic;
		req.operand_count = encoded_instr.operand_count;
		for(int i = 0; i < encoded_instr.operand_count; i++)
		{
			req.operands[i] = encoded_instr.operand[i];
		}
	}*/
};