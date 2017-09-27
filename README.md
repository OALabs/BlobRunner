# BlobRunner 

BlobRunner is a simple tool to quickly debug shellcode extracted during malware analysis.  
BlobRunner allocates memory for the target file and jumps to the base (or offset) of the allocated memory. This allows
an analyst to quickly debug into extracted artifacts with minimal overhead and effort. 

![BlobRunner Sample](br.png "BlobRunner")

To use BlobRunner you can download a compiled version from /bin or follow the steps below to build the executable.

## Building

Requirements
 - Download and install Microsoft Visual C++ Build Tools or Visual Studio 

Build using: cl blobrunner.c
 
## Usage

To debug: 

 - Open BlobRunner in your favorite debugger.
 - Pass the shellcode file as the first parameter. 
 - Add a breakpoint before the jump into the shellcode 
 - Step into the shellcode 
 
```
BlobRunner.exe shellcode.bin
```

Debug into file at a specific offset.

```
BlobRunner.exe shellcode.bin --offset 0x0100
```

Debug into file and don't pause before the jump. __Warning:__ Ensure you have a breakpoint set before the jump.

```
BlobRunner.exe shellcode.bin --nopause
```

