#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "strutils.h"

uint32_t getFileLength(FILE * fptr)
{
	uint32_t		fileLength;
	
	fseek(fptr, SEEK_END, 0);
	fileLength = ftell(fptr);
	fseek(fptr, SEEK_SET, 0);
	
	return fileLength;
}

int main(int argc, char *argv[])
{
	char *		szSourceFile;
	char *		szOutputFile;
	char *		buffer;
	
	FILE *		fptr;
	
	uint32_t	fileLength;
	uint32_t	bytesRead;
	uint32_t	bytesWritten;
	
	if (argc < 3) {
		printf("Too few arguments supplied, expecting a source file name and output file name.\n");
		exit(-1);
	}
	else if (argc == 3) {
		szSourceFile = strdup(&argv[1][0]);
		szOutputFile = strdup(&argv[2][0]);
	}
	else {
		printf("Invalid number of arguents supplied.\n");
		exit(-1);
	}
	
	fptr = fopen(szSourceFile, "rt");
	
	if (fptr == NULL) {
		printf("failed to open source file [%s]\n", szSourceFile);
		exit(-1);
	}

	fileLength = getFileLength(fptr);
	
	buffer = (char *)malloc(fileLength);
	
	if (buffer == NULL) {
		printf("Failed to allocate %u bytes for input file\n", fileLength);
		exit(-1);
	}
	
	printf("Reading %u bytes from file %s\n", fileLength, szSourceFile);
	
	bytesRead = fread(buffer, 1, fileLength, fptr);
	
	fclose(fptr);
	
	if (bytesRead != fileLength) {
		printf(
			"Failed to read %u bytes of file %s, got %u bytes instead\n", 
			fileLength, 
			szSourceFile, 
			bytesRead);
			
		free(buffer);
		exit(-1);
	}
	
	printf("Before trim: [%s]\n", buffer);
	
	char * outputBuffer = str_trim_trailing(buffer);
	
	printf("After str_trim_trailing(): [%s]\n", outputBuffer);
	
	fptr = fopen(szOutputFile, "wt");
	
	if (fptr == NULL) {
		printf("failed to open output file [%s]\n", szOutputFile);
		exit(-1);
	}
	
	bytesWritten = fwrite(outputBuffer, 1, strlen(outputBuffer), fptr);
	
	if (bytesWritten != strlen(outputBuffer)) {
		printf(
			"Failed to write %lu bytes of file %s, wrote %u bytes instead\n",
			strlen(outputBuffer),
			szOutputFile,
			bytesWritten);
	}
	
	fclose(fptr);
	
	free(buffer);
	free(outputBuffer);
	
	return 0;
}
