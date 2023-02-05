#pragma once
#include <cstdio>
#include <Zydis.h>
#include <vector>

#include "Defines.h"

class instruction
{
public:
	instruction(std::vector<BYTE>& encoded_instr)
	{
		DecodeInstruction(encoded_instr);
	}
	void EncodeInstruction(ZydisDecodedInstruction& decoded_instr)
	{

	}
	void DecodeInstruction(const std::vector<BYTE>& encoded_instr)
	{
		ZydisDecoder decoder;
		ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

		ZydisDecodedInstruction instruction;
		ZyanStatus status = ZydisDecoderDecodeInstruction(&decoder,
			static_cast<ZydisDecoderContext*>ZYAN_NULL, encoded_instr.data(), encoded_instr.size(), &decoded_instr);

		if (status != ZYAN_STATUS_SUCCESS)
		{
			printf("Decoding failed with error code: %d\n", status);
			return ;
		}

		printf("Instruction: %s\n", ZydisMnemonicGetString(decoded_instr.mnemonic));
		return ;
	}
	ZydisDecodedInstruction decoded_instr;
};
class instructions_parser
{

};