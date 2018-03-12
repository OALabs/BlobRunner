#include <stdio.h>
#include <windows.h>

// Define bool
typedef int bool;
#define true 1
#define false 0

#define COLOR_GREEN(txt) "\033[92m"txt"\033[0m"
#define COLOR_YELLOW(txt) "\033[93m"txt"\033[0m"
#define COLOR_RED(txt) "\033[31m"txt"\033[0m"


const char *_version = "0.0.4";
const char *_banner = " __________.__        ___.  __________\n"
" \\______   \\  |   ____\\_ |__\\______   \\__ __  ____   ____   ___________        \n"
"  |    |  _/  |  /  _ \\| __ \\|       _/  |  \\/    \\ /    \\_/ __ \\_  __ \\     \n"
"  |    |   \\  |_(  <_> ) \\_\\ \\    |   \\  |  /   |  \\   |  \\  ___/|  | \\/    \n"
"  |______  /____/\\____/|___  /____|_  /____/|___|  /___|  /\\___  >__|             \n"
"         \\/                \\/       \\/           \\/     \\/     \\/           \n\n"
"                                                               " COLOR_GREEN("%s") " \n\n";

const char *_help = "     Required args: <inputfile>\n"
"     Optional Args:\n"
"                     --offset <offset> The offset to jump into.\n"
"                     --nopause         Don't pause before jumping to shellcode. Danger!!! \n"
"                     --autobreak       Insert a breakpoint at the offset. (Default: 0)\n"
//"                     --debug           Verbose logging.\n"
"                     --version         Print version and exit.\n\n";

void banner() {
	system("cls");
	printf(_banner, _version);
	return;
}


typedef struct {
	int offset;
	bool nopause;
	bool debug;
	bool autobreak;
	char *input_file;
} ProgramArgs, *pProgramArgs;

bool parse_args(pProgramArgs args, char* argv[], int argc) {
	if (argc < 2) {
		printf(COLOR_RED("[!] Error: ") "No file!\n");
		printf(_help);
		return false;
	}
	args->input_file = argv[1];

	for (int i = 2; i < argc; i++) {
		if (strcmp(argv[i], "--offset") == 0) {
			printf(" [*] Parsing offset...\n");
			args->offset = strtol(argv[++i], NULL, 16);
		}
		else if (strcmp(argv[i], "--nopause") == 0) {
			args->nopause = true;
		}
		else if (strcmp(argv[i], "--autobreak") == 0) {
			args->autobreak = true;
			args->nopause = true;
		}
		else if (strcmp(argv[i], "--debug") == 0) {
			args->debug = true;
		}
		else if (strcmp(argv[i], "--version") == 0) {
			printf("Version: %s", _version);
		}
		else {
			printf(COLOR_YELLOW(" [!] Warning: ") "Unknown arg: %s\n", argv[i]);
		}
	}
	return true;
}


char* read_data_from_file(char *input_file, size_t *data_size, bool debug) {
	printf(" [*] Using file: %s \n", input_file);

	FILE *file = fopen(input_file, "rb");
	if (!file) {
		printf(COLOR_RED(" [!] Error: ") "Unable to open %s\n", input_file);
		return false;
	}

	printf(" [*] Reading file...\n");
	fseek(file, 0, SEEK_END);
	unsigned long fileLen = ftell(file);

	printf(" [*] File Size: 0x%04x\n", fileLen);
	char *buffer = (char *)malloc(fileLen);

	fseek(file, 0, SEEK_SET);
	fread(buffer, fileLen, 1, file);
	fclose(file);

	*data_size = fileLen;
	return buffer;
}

LPVOID copy_data_to_executable_memory(char *data, size_t data_size, bool debug) {
	printf(" [*] Allocating Memory...");

	LPVOID lpvBase = VirtualAlloc(NULL,					//The starting address of the region to allocate
		data_size,					//The size of the region, in bytes.
		MEM_COMMIT | MEM_RESERVE,	//The type of memory allocation. 
		PAGE_EXECUTE_READWRITE);    //The memory protection type

	printf(".Allocated!\n");
	printf(" [*]   |-Base: %p\n", lpvBase);
	printf(" [*] Copying input data...\n");

	CopyMemory(lpvBase, data, data_size);
	return lpvBase;
}


int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {
	printf(COLOR_RED(" [!] Warning: ") "Unhandled Exception was generated during shellcode execution\n");
	printf("Catch it? y/n\n");

	char ch;
	while (ch = getchar()) {
		if (ch == 'y' || ch == 'Y' || ch == '\n') {
			return EXCEPTION_EXECUTE_HANDLER;
		}
		if (ch == 'n' || ch == 'N') {
			return EXCEPTION_CONTINUE_SEARCH;
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

void execute(LPVOID base, int offset, bool nopause, bool autobreak, bool debug) {
	printf(" [*] Using offset: 0x%08x\n", offset);

	LPVOID entry = (LPVOID)((int)base + offset);
	printf(" [*] Entry: %p\n", entry);

	if (nopause == false) {
		printf(" [*] Navigate to the EP and set a breakpoint. Then press any key to jump to the shellcode.\n");
		getchar();
	}
	else {
		printf(" [*] Jumping to shellcode\n");
	}

	__try {
		if (autobreak) {
			__debugbreak();
		}
		((void(*)())entry)();
	}
	__except (filter(GetExceptionCode(), GetExceptionInformation())) {
	}
}


int main(int argc, char* argv[])
{
	banner();

	ProgramArgs args = { 0 };
	if (!parse_args(&args, argv, argc)) {
		return -1;
	}

	size_t shellcode_size = 0;
	char *shellcode = read_data_from_file(args.input_file, &shellcode_size, args.debug);
	if (!shellcode) {
		return -1;
	}

	LPVOID base = copy_data_to_executable_memory(shellcode, shellcode_size, args.debug);
	if (!base) {
		return -1;
	}

	// run the shellcode
	execute(base, args.offset, args.nopause, args.autobreak, args.debug);

	// free allocated memory
	VirtualFree(base, shellcode_size, MEM_RELEASE);
	free(shellcode);

	printf("Pausing - Press any key to quit.\n");
	getchar();
	return 0;
}
