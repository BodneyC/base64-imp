#include <iostream>
#include <cstdio>
#include <cstring>
#include "omp.h"
typedef unsigned char BYTE;
//------------------------------------------------------------------------------
static const std::string b64Chars =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//------------------------------------------------------------------------------
void b64dec(BYTE* group3, BYTE* group4) {
	BYTE tmp4[4];
	memcpy(tmp4, group4, 4);

	group3[0] = (tmp4[0] << 2) + ((tmp4[1] & 0x30) >> 4);
	group3[1] = ((tmp4[1] & 0x0F) << 4) + ((tmp4[2] & 0x3C) >> 2);
	group3[2] = ((tmp4[2] & 0x3) << 6) + tmp4[3];
}
//------------------------------------------------------------------------------
std::string decode(BYTE* inputString, int inputLength) {
	std::string outputString;
	int charGroups;

	if (inputLength % 4 == 0) {
		charGroups = inputLength / 4;
	} else {
		charGroups = inputLength / 4 + 1;
	}

	BYTE outputArr[charGroups][3];

	std::cout << "Input: " << inputString << "\nLength: " << inputLength << " chars\n";

	#pragma omp parallel for ordered schedule(static,1)
		for (size_t i = 0; i < charGroups; i++) {
			BYTE group3[3] = {0};
			BYTE group4[4] = { inputString[i * 4], inputString[(i * 4) + 1], inputString[(i * 4) + 2], inputString[(i * 4) + 3]};

			#pragma omp critical // To be printed thread at a time
			{
				std::cout << "\nIteration: " << i << "\n";
				for (size_t j = 0; j < 4; j++) {
					std::cout << "group4[" << j << "]: " << group4[j] << "\n";
				}
			}

			for (size_t j = 0; j < 4; j++) {
				if (group4[j] == '=') {
					group4[j] = 0x00;
				} else {
					group4[j] = b64Chars.find(group4[j]);
				}
			}

			b64dec(group3, group4);

			#pragma omp ordered
			{
				for (size_t j = 0; j < 3; j++) {
					outputString += group3[j];
				}
			}
		}

	// std::cout << outputString;

	return outputString;
}
//------------------------------------------------------------------------------
void b64enc(BYTE* group4, BYTE* group3) {
	BYTE tmp3[3];
	memcpy(tmp3, group3, 3);

	group4[0] = (tmp3[0] & 0xFC) >> 2;
	group4[1] = ((tmp3[0] & 0x03) << 4) + ((tmp3[1] & 0xF0) >> 4);
	group4[2] = ((tmp3[1] & 0x0F) << 2) + ((tmp3[2] & 0xC0) >> 6);
	group4[3] = tmp3[2] & 0x3F;
}
//------------------------------------------------------------------------------
std::string encode(BYTE* inputString, int inputLength) {
	std::string outputString;
	int charGroups = inputLength / 3; // charGroups: number of three-char groups
	bool finalEnc = (inputLength % 3 == 0)? false: true;
	BYTE outputArr[charGroups + finalEnc][4]; // [# of groups (+1 for remainder)][chars per output group]

	std::cout << "Input: " << inputString << "\nLength: " << inputLength << " chars\n";

	#pragma omp parallel for
		for (size_t i = 0; i < charGroups + finalEnc; i++) {
			BYTE group4[4] = {0};
			BYTE group3[3] = { inputString[i * 3], inputString[(i * 3) + 1], inputString[(i * 3) + 2]};

			#pragma omp critical // To be printed thread at a time
			{
				std::cout << "\nIteration: " << i << "\n";
				for (size_t j = 0; j < 3; j++) {
					std::cout << "group3[" << j << "]: " << group3[j] << "\n";
				}
			}

			b64enc(group4, group3);

			for (size_t out4Count = 0; out4Count < 4; out4Count++) {
				outputArr[i][out4Count] = b64Chars[group4[out4Count]];
			}

			if (group3[1] == 0) {
				outputArr[i][3] = '=';
				outputArr[i][2] = '=';
			} else if (group3[2] == 0) {
				outputArr[i][3] = '=';
			}
		}

	for (size_t i = 0; i < charGroups + finalEnc; i++) {
		for (size_t j = 0; j < 4; j++) {
			outputString += outputArr[i][j];
		}
	}

	return outputString;
}
//------------------------------------------------------------------------------
int main(int argc, char const *argv[]) {
	bool encSwitch;
	int inputLength;
	BYTE inputString[255] = {0};
	std::string outputString;

	if (argc == 3) { // CLAP
		// strcmp() returns 0 if equal...
		if (!strcmp(argv[1], "-e")) {
			encSwitch = true;
		} else if (!strcmp(argv[1], "-d")) {
			if (strlen(argv[2]) % 4 == 1) {
				std::cout << "\nIncorrect input. Decode [-d] strings cannot have remainder one when divided by four.";
				return 1;
			}
			encSwitch = false;
		} else {
			std::cout << "\nIncorrect input. Command format:\n\tbenc64 [-e][-d] [input_string]\n";
			return 1;
		}
	} else {
		std::cout << "\nIncorrect input. Command format:\n\tbenc64 [-e][-d] [input_string]\n";
		return 1;
	}

	inputLength = strlen(argv[2]);
	memcpy(inputString, argv[2], inputLength);

	if (encSwitch) {
		outputString = encode(inputString, inputLength);
	} else {
		outputString = decode(inputString, inputLength);
	}

	std::cout << "\nOutput string: " << outputString << std::endl;

	return 0;
}
//------------------------------------------------------------------------------
