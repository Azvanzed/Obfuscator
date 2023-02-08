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

	BOOLEAN isJmpRelative() const
	{
		if (!(Decoded.attributes & ZYDIS_ATTRIB_IS_RELATIVE))
			return FALSE;

		return Decoded.mnemonic >= ZYDIS_MNEMONIC_JB
			&& Decoded.mnemonic <= ZYDIS_MNEMONIC_JZ;
	}
};