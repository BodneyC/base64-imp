#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "omp.h"

int main(int argc, char const *argv[]) {
	if (argc != 2) { // CLAP
		printf("Incorrect input\n");
		return 1;
	}
	char filename[255];
	strcpy(filename, argv[1]);
	int inputLength = strlen(argv[1]);
	int byteGroups = inputLength / 3; // byteGroups: number of three-byte groups
	bool finalEnc = inputLength % 3 == 0? true: false;
	char outputArr[byteGroups + finalEnc][4]; // [# of groups (+1 for remainder)][bytes per output group]

	printf("Input: %s\nLength: %d bytes\n", filename, inputLength);

	#pragma omp parallel for
		for (size_t i = 0; i < byteGroups; i++) {
			char TMParr[3] = { filename[i * 3], filename[(i * 3) + 1], filename[(i * 3) + 2]};

			#pragma omp critical // To be printed thread at a time
			{
				printf("\nIteration: %d\n", i);
				for (size_t j = 0; j < 3; j++) {
					printf("TMParr[%d]: %c\n", j, TMParr[j]);
				}
			}

		}

	// deal with (potential) final three

	return 0;
}
