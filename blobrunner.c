#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>

#ifdef _WIN64
    #include <WinBase.h>
#endif

// Define bool
typedef int bool;
#define true 1
#define false 0

char *_version = "0.0.2";

void banner(){
    system("cls");
    printf(" __________.__        ___.  __________\n");
    printf(" \\______   \\  |   ____\\_ |__\\______   \\__ __  ____   ____   ___________ \n");
    printf("  |    |  _/  |  /  _ \\| __ \\|       _/  |  \\/    \\ /    \\_/ __ \\_  __ \\ \n");
    printf("  |    |   \\  |_(  <_> ) \\_\\ \\    |   \\  |  /   |  \\   |  \\  ___/|  | \\/ \n");
    printf("  |______  /____/\\____/|___  /____|_  /____/|___|  /___|  /\\___  >__|    \n");
    printf("         \\/                \\/       \\/           \\/     \\/     \\/        \n");
    printf("\n\n");
    return;
}

LPVOID process_file(char* inputfile_name){
	LPVOID lpvBase;
	FILE *file;
	unsigned long fileLen;
	char *buffer;

	file = fopen(inputfile_name, "rb");

	if (!file){
		printf(" [!] Unable to open inputfile!\n");

		return (LPVOID)-1;
	}

	printf (" [*] Reading file...\n");	
	fseek(file, 0, SEEK_END);
	fileLen=ftell(file); //Get Length

	printf (" [*] File Size: %d\n", fileLen);
	fseek(file, 0, SEEK_SET); //Reset 
	
	buffer=(char *)malloc(fileLen+1); //Create Buffer
	fread(buffer, fileLen, 1, file);  //Read file to buffer
	fclose(file);                     //Close file handle
	
	printf (" [*] Allocating Memory...");	

	lpvBase = VirtualAlloc(NULL,fileLen,0x3000,0x40);

	printf(".Allocated!\n");
	printf(" [*]   |-Base: 0x%08x\n",lpvBase);
	printf(" [*] Copying input data...\n");	
	
	CopyMemory(lpvBase, buffer, fileLen);
	return lpvBase;
}

void execute(LPVOID base, int offset, bool nopause, bool debug)
{
	LPVOID entry;
	#ifdef _WIN64
	    DWORD   thread_id;
	    HANDLE  thread_handle;
	#endif

	entry = (LPVOID)((int)base + offset);


	#ifdef _WIN64

	    printf(" [*] Creating Suspended Thread...\n");
        thread_handle = CreateThread(
            NULL,          // Attributes
            0,             // Stack size (Default)
            entry,         // Thread EP
            NULL,          // Arguments
            0x4,           // Create Suspended
            &thread_id);   // Thread identifier

        if(thread_handle == NULL){
            printf(" [!] Error Creating thread...");
            return;
        }
        printf(" [*] Created Thread: [%d]\n", thread_id);
        printf(" [*] Thread Entry: 0x%016x\n",entry);
        printf(" [*] Navigate to the Thread Entry and set a breakpoint. Then press any key to resume the thread.\n",entry);
        getchar();
        ResumeThread(thread_handle);
    #else
        printf(" [*] Entry: 0x%08x\n",entry);
        if(nopause == false){
	        printf(" [*] Navigate to the EP and set a breakpoint. Then press any key to jump to the shellcode.\n");
	        getchar();
	    }
	    else{
	        printf(" [*] Jumping to shellcode\n");
	    }
        __asm jmp entry;
    #endif
}

int main(int argc, char* argv[])
{
    LPVOID base;
	int i;
	int offset = 0;
	bool nopause = false;
	bool debug = false;
	char *nptr;

	banner();

	if(argc < 2){
		printf(" [!] Error: No file!\n\n");
		printf("     Required args: <inputfile>\n\n");
		printf("     Optional Args:\n");
		printf("                     --offset <offset> The offset to jump into.\n");
		printf("                     --nopause         Don't pause before jumping to shellcode. Danger!!! \n");
		printf("                     --debug           Verbose logging.\n");
		printf("                     --version         Print version and exit.\n\n");
		return -1;
	}

	for(i=2; i < argc; i++){
		if(strcmp(argv[i], "--offset") == 0){
			printf(" Parsing offset...");
			i=i+1;
			offset = strtol(argv[i], &nptr, 16);
		}
		else if(strcmp(argv[i], "--nopause") == 0){
			nopause = true;
		}
		else if(strcmp(argv[i],"--debug") == 0){
			debug = true;
		}
		else if(strcmp(argv[i],"--version") == 0){
			printf("Version: %s", _version);
		}
		else{
			printf(" [!] Unknown arg: %s",argv[i]);
		}
	}

	printf(" [*] Loading file: %s \n", argv[1]);
	base = process_file(argv[1]);
	printf(" [*] Using offset: 0x%08x\n", offset);
	execute(base, offset, nopause, debug);
	printf ("Pausing - Press any key to quit.\n");
	getchar();
	return 0;
}