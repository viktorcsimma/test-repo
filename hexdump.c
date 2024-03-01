#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Author: fadedave <fadedave77 ät fadedave.com>
// Licensed under the AGPL license <https://www.gnu.org/licenses/agpl-3.0.html>

#define WIDTH 16

void printLine(int chars[], int len, long addr) {
	printf("%08lx  ", addr);
	for (int i=0; i<len; i++) {
		if (i%8==0 && 0<i && i<WIDTH) printf(" ");
		if (i<len-1) printf("%02x ",chars[i]);
		else {
			printf("%02x",chars[i]);
			printf("%*c", (WIDTH*3-1)-(len*3-1)-((len-1)/8)+3 , ' ');
			printf("|");
			for (int j=0; j<len; j++) {
				if (isprint(chars[j])) printf("%c", chars[j]);
				else printf(".");
			}
			printf("|\n");
		}
	}
}

void readAndPrint(FILE* files[], int len) {
	char c = EOF; int chars[WIDTH] = {0}; int charCount = 0; long addr = 0; int fCount = 0;
	do {
		c = fgetc(files[fCount]);
		if (c==EOF) {
			if (fCount<len-1) {
				fCount++;
				continue;
			}
			break;
		}
		chars[charCount] = c;
		if (charCount == WIDTH-1) {
			printLine(chars, WIDTH, addr);
			charCount = 0;
			addr += WIDTH;
		} else {
			charCount++;
		}
	} while (1);
	if (charCount!=0) printLine(chars, charCount, addr);
	printf("%08lx\n", charCount ? addr+charCount : addr);
}

int main(int argAm, char* args[]) {
	if (WIDTH<1) {printf("\n\n********** MUST set WIDTH to at least 1 **********\n\n"); return 1;}
	FILE* files[argAm-1];
	if (argAm != 1) {
		for (int i=1; i<argAm; i++) {
			files[i-1] = fopen(args[i], "r");
			if (files[i-1] == NULL) return 1;
		}
		readAndPrint(files, argAm-1);
		for (int i=1; i<argAm; i++) {
			fclose(files[i-1]);
		}
	} else {
		files[0] = stdin;
		readAndPrint(files, 1);
	}
	return 0;
}
