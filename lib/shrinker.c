//This is the main file of the project and can be included by itself to use the library functions in your own program.
//Made by Humanagon (Hayden J. Breerwood) in 2025!

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

char *compressAsciiString(char *str) {
	size_t len = strlen(str);
	char *ret = 0;
	if(len == 0) {
		ret = malloc(1);
		*ret = 0;
		return ret;
	}
	ret = malloc(len + 1);
	size_t size = 0;
	for(size_t i = 0; i < len; i += 8) {
		if(i >= len) break;
		for(size_t j = 1; j <= 7; j++) {
			if(i + j > len) break;
			int dif = j - 1;
			unsigned int code = *(str + i + dif) & 127;
			*(ret + size) = code << j;
			size++;
			if(i == 0 && j == 1) continue;
			int offset = size - 2;
			*(ret + offset) = *(ret + offset) | (code >> (7 - dif));
		}
		if(i + 7 < len) {
			size_t offset = size - 1;
			*(ret + offset) = *(ret + offset) | (*(str + i + 7) & 127);
		}
	}
	*(ret + size) = '\0';
	return ret;
}

char *decompressAsciiString(char *str) {
	size_t len = strlen(str);
	char *ret = 0;
	if(len == 0) {
		ret = malloc(1);
		*ret = 0;
		return ret;
	}
	size_t size = (len * 1.125) + 8; //Eight extra bytes just in case the calculation is off.
	ret = malloc(size);
	size = 0;
	for(size_t i = 0; i < len; i += 7) {
		if(i >= len) break;
		*(ret + size) = (*(str + i) >> 1) & 127;
		size++;
		for(size_t j = 1; j < 8; j++) {
			if(len <= i + j) break;
			*(ret + size) = ((((*(str + ((i - 1) + j)) & 255) << 8) | (*(str + i + j) & 255)) >> (j + 1)) & 127;
			size++;
		}
	}
	*(ret + size) = '\0';
	return ret;
}

int compressAsciiFile(char *source_name, char *output_name) {
	FILE *source = fopen(source_name, "rb");
	if(!source) return 1;
	FILE *output = fopen(output_name, "wb");
	if(!output) {
		fclose(source);
		return 3;
	}
	int selector = 0;
	unsigned char last = '\0';
	char first = 1;
	while(selector != EOF) {
		for(size_t j = 0; j < 7; j++) {
			selector = fgetc(source);
			if(selector == EOF) {
				if(fputc(last, output) == EOF) {
					fclose(source);
					fclose(output);
					return 2;
				}
				break;
			}
			unsigned char code = selector & 127;
			if(!first) if(fputc(last | (code >> (7 - j)), output) == EOF) {
				fclose(source);
				fclose(output);
				return 2;
			}
			first = 0;
			last = code << j + 1;
		}
		selector = fgetc(source);
		if(selector == EOF) break;
		last = last | selector;
	}
	fclose(source);
	fclose(output);
	return 0;
}

int decompressAsciiFile(char *source_name, char *output_name) {
	FILE *source = fopen(source_name, "rb");
	if(!source) return 1;
	FILE *output = fopen(output_name, "wb");
	if(!output) return 3;
	char ret[9];
	int size = 0;
	int selector = 0;
	
	selector = fgetc(source);
	for(size_t i = 0; selector != EOF; i += 7) {
		memset(ret, 0, 8);
		size = 0;
		if(selector == EOF) break;
		fputc((selector >> 1) & 127, output);
		for(size_t j = 1; j < 8; j++) {
			int selector2 = fgetc(source);
			if(selector2 == EOF) {
				selector = selector2;
				break;
			}
			*(ret + size) = ((((selector & 255) << 8) | (selector2 & 255)) >> (j + 1)) & 127;
			selector = selector2;
			size++;
		}
		fputs(ret, output);
	}
	return 0;
}

int printCompressedAsciiFile(char *source_name) {
	FILE *source = fopen(source_name, "rb");
	if(!source) return 1;
	char ret[9];
	int size = 0;
	int selector = 0;
	
	selector = fgetc(source);
	for(size_t i = 0; selector != EOF; i += 7) {
		memset(ret, 0, 8);
		size = 0;
		if(selector == EOF) break;
		printf("%c", (selector >> 1) & 127);
		for(size_t j = 1; j < 8; j++) {
			int selector2 = fgetc(source);
			if(selector2 == EOF) {
				selector = selector2;
				break;
			}
			*(ret + size) = ((((selector & 255) << 8) | (selector2 & 255)) >> (j + 1)) & 127;
			selector = selector2;
			size++;
		}
		printf("%s", ret);
	}
	return 0;
}