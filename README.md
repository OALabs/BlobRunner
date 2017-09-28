# BlobRunner 

BlobRunner is a simple tool to quickly debug shellcode extracted during malware analysis.  
BlobRunner allocates memory for the target file and jumps to the base (or offset) of the allocated memory. This allows
an analyst to quickly debug into extracted artifacts with minimal overhead and effort. 

![BlobRunner Sample](br.png "BlobRunner")

To use BlobRunner, you can download the compiled executable from the [releases](https://github.com/OALabs/BlobRunner/releases) page or build your own using the steps below.

## Building
Building the executable is straight forward and relatively painless. 

__Requirements__
 - Download and install Microsoft Visual C++ Build Tools or Visual Studio 
 
__Build Steps__

 - Open Visual Studio Command Prompt 
 - Navigate to the directory where BlobRunner is checked out
 - Build the executable by running: cl blobrunner.c
 
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

The process is virtually identical to debugging shellcode locally - with the exception that the you need to copy the shellcode file
to the remote system. If the file is copied to the same path you are running _win32_remote.exe_ from, you just need to use 
the file name for the parameter. Otherwise, you will need to specify the path to the shellcode file on the remote system.

## Feedback / Help

 - Any questions, comments or requests you can find us on twitter: @seanmw or @herrcore
 - Pull requests welcome! 