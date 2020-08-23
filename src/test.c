#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "strutils.h"

uint32_t getFileLength(FILE * fptr)
{
	uint32_t		fileLength;
	
	fseek(SEEK_END, fptr);
	fileLength = ftell(fptr);
	fseek(SEEK_SET, fptr);
	
	return fileLength;
}

int main(void)
{
	char		szSourceFile[128];
	char		szOutputFile[128];
	char *		buffer;
	
	FILE *		fptr;
	
	uint32_t	fileLength;
	uint32_t	bytesRead;
	uint32_t	bytesWritten;
	
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
			"Failed to write %u bytes of file %s, wrote %u bytes instead\n",
			strlen(outputBuffer),
			szOutputFile,
			bytesWritten);
	}
	
	fclose(fptr);
	
	free(buffer);
	free(outputBuffer);
	
	return 0;
}
