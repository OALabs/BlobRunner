# BlobRunner 

BlobRunner is a simple tool to quickly debug shellcode extracted during malware analysis.  
BlobRunner allocates memory for the target file and jumps to the base (or offset) of the allocated memory. This allows
an analyst to quickly debug into extracted artifacts with minimal overhead and effort. 

![BlobRunner Sample](br.png "BlobRunner")

To use BlobRunner, you can download the compiled executable from the [releases](https://github.com/OALabs/BlobRunner/releases) page or build your own using the steps below.

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

##### Remote Debugging Shell Blobs (IDAPro)

The process is identical to debugging shellcode locally with the exception that the you need to copy the shell blob
to the remote system. It's easiest if you copy the file to the same location that you are running win32_remote.exe 
from.

## Feedback / Help

 - Any questions, comments or requests you can find us on twitter: @seanmw or @herrcore
 - Pull requests welcome! 