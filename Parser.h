class zydisParser
{
private:
	ZydisDecoder Decoder;

public:

	zydisParser()
	{
		ZydisDecoderInit(&Decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);
	}

	BOOLEAN decodeInstruction(std::vector<BYTE> Opcode, ZydisDecodedInstruction* decodedInstruction)
	{	
		return ZydisDecoderDecodeInstruction(&Decoder, static_cast<ZydisDecoderContext*>(ZYAN_NULL),
			Opcode.data(), Opcode.size(), decodedInstruction) == ZYAN_STATUS_SUCCESS;
	}
};