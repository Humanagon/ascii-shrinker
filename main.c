#include "lib/ascii_shrinker.c"

int main(int argc, char *argv[]) {
	char *msgs[] = {"Could not open the source file.", "Invalid mode."};
	if(argc < 2) {
		printf("Please pass in your arguments, or \"help\" for help information.");
	} else {
		char *arg = *(argv + 1);
		if(strcmp(arg, "help") == 0) {
			printf("%s", global_long_help_string);
		} else if(strcmp(arg, "read") == 0 && argc > 2) {
			switch(printCompressedAsciiFile(*(argv + 2))) {
				case 1:
					printf("Could not open the source file.");
			}
		} else if(argc <= 3) {
			printf("Too few arguments were passed.");
		} else {
			unsigned char mode = 0;
			if(strcmp(arg, "shrink") == 0 || strcmp(arg, "compress") == 0 || strcmp(arg, "deflate") == 0) {
				mode = 1;
			} else if(strcmp(arg, "grow") == 0 || strcmp(arg, "decompress") == 0 || strcmp(arg, "inflate") == 0) {
				mode = 2;
			}
			arg = *(argv + 2);
			if(strcmp(arg, "base64") == 0) {
				arg = *(argv + 3);
				char *str = 0;
				char *data = 0;
				size_t size_temp = 0;
				switch(mode) {
					case 1:
						data = base64_decode(arg, strlen(arg), &size_temp);
						data = realloc(data, size_temp + 1);
						*(data + size_temp) = '\0';
						str = compressAsciiString(data);
						free(data);
						data = base64_encode(str, strlen(str), &size_temp);
						free(str);
						data = realloc(data, size_temp + 1);
						*(data + size_temp) = '\0';
						printf("%s", data);
						free(data);
						base64_cleanup();
						break;
					case 2:
						data = base64_decode(arg, strlen(arg), &size_temp);
						data = realloc(data, size_temp + 1);
						*(data + size_temp) = '\0';
						str = decompressAsciiString(data);
						free(data);
						data = base64_encode(str, strlen(str), &size_temp);
						free(str);
						data = realloc(data, size_temp + 1);
						*(data + size_temp) = '\0';
						printf("%s", data);
						free(data);
						base64_cleanup();
						break;
					default:
						printf(msgs[1]);
				}
			} else if(strcmp(arg, "string") == 0) {
				arg = *(argv + 3);
				char *str = 0;
				char *data = 0;
				size_t size_temp = 0;
				switch(mode) {
					case 1:
						str = compressAsciiString(arg);
						data = base64_encode(str, strlen(str), &size_temp);
						free(str);
						data = realloc(data, size_temp + 1);
						*(data + size_temp) = '\0';
						printf("%s", data);
						free(data);
						base64_cleanup();
						break;
					case 2:
						data = base64_decode(arg, strlen(arg), &size_temp);
						data = realloc(data, size_temp + 1);
						*(data + size_temp) = '\0';
						str = decompressAsciiString(data);
						free(data);
						printf("%s", str);
						free(str);
						base64_cleanup();
						break;
					default:
						printf(msgs[1]);
				}
			} else if(strcmp(arg, "raw") == 0) {
				arg = *(argv + 3);
				char *str = 0;
				switch(mode) {
					case 1:
						str = compressAsciiString(arg);
						printf("%s", str);
						free(str);
						break;
					case 2:
						str = decompressAsciiString(arg);
						printf("%s", str);
						free(str);
						break;
					default:
						printf(msgs[1]);
				}
			} else if(strcmp(arg, "file") == 0) {
				if(argc == 4) printf("Missing output file."); else {
					char ret_err = -1;
					switch(mode) {
						case 1:
							printf("Shrinking ASCII file...\n");
							ret_err = compressAsciiFile(*(argv + 3), *(argv + 4));
							break;
						case 2:
							printf("Decompressing ASCII file...\n");
							ret_err = decompressAsciiFile(*(argv + 3), *(argv + 4));
							break;
						default:
							printf(msgs[1]);
					}
					if(ret_err != -1) switch(ret_err) {
						case 0:
							printf("Complete.");
							break;
						case 1:
							printf(msgs[0]);
							break;
						case 2:
							printf("Write failure.");
							break;
						case 3:
							printf("Could not create/open the output file.");
							break;
						default:
							printf("An unknown error occured. (Code %d)", ret_err);
					}
				}
			} else printf("Invalid type.");
		}
	}
	return 0;
}