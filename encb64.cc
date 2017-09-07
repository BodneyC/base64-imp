#include <iostream>
#include <cstring>
#include "omp.h"
typedef unsigned char BYTE;
//------------------------------------------------------------------------------
static const BYTE b64Chars[65] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
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
	if (argc != 2) { // CLAP
		std::cout << "Incorrect input\n";
		return 1;
	}
	int inputLength = strlen(argv[1]);
	BYTE inputString[255] = {0};
	memcpy(inputString, argv[1], inputLength);

	std::string outputString = encode(inputString, inputLength);

	std::cout << "Output string: " << outputString;

	return 0;
}
//------------------------------------------------------------------------------
