class CZydisInstruction
{
public:
	USHORT														Offset;
	ZydisDecodedInstruction										Decoded;
	std::array<ZydisDecodedOperand, ZYDIS_MAX_OPERAND_COUNT>	Operands;

	CZydisInstruction()
	{
	}

	CZydisInstruction(const ZydisDecodedInstruction& Instruction,
		const std::array<ZydisDecodedOperand, ZYDIS_MAX_OPERAND_COUNT>& Operands)
		: Decoded(Instruction), Operands(Operands)
	{
	}

	BOOLEAN isJumpRelative() const
	{
		if (!(Decoded.attributes & ZYDIS_ATTRIB_IS_RELATIVE))
			return FALSE;

		return Decoded.mnemonic >= ZYDIS_MNEMONIC_JB
			&& Decoded.mnemonic <= ZYDIS_MNEMONIC_JZ;
	}
	std::vector<ZyanU8> encodeImmJumpWithOffset( ZyanU64 newOffset)
	{
		if (isJumpRelative())
			return {};
		ZydisEncoderRequest req;
		memset(&req, 0, sizeof(req));
		req.mnemonic = Decoded.mnemonic; // the jump could be a conditional jump, like JE, JB, JNZ, JZ etc... this is the whole point of this
		req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
		req.operand_count = 1;
		req.operands[0].type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
		req.operands[0].imm.u = newOffset;

		std::vector<ZyanU8> encodedInstruction(ZYDIS_MAX_INSTRUCTION_LENGTH);
		ZyanUSize encodedLength;

		if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encodedInstruction.data(), &encodedLength)))
			return {};
		encodedInstruction.resize(encodedLength);
		return encodedInstruction;
	}
};