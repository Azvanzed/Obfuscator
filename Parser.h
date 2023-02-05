class zydisParser
{
private:
	ZydisDecoder Decoder;

public:

	zydisParser()
	{
		ZydisDecoderInit(&Decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);
	}

	ZydisDecodedInstruction* decodeInstruction(std::vector<BYTE> Opcode)
	{	
		ZydisDecodedInstruction Instruction;
		if ( ZydisDecoderDecodeInstruction(&Decoder, static_cast<ZydisDecoderContext*>( ZYAN_NULL ),
			Opcode.data(), Opcode.size(), &Instruction) != ZYAN_STATUS_SUCCESS )
			return NULL;

		return &Instruction;
	}

	VOID encodedInstruction()
	{
		eodenc
	}
};