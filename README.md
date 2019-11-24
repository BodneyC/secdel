# SECDEL

This is a very small secure delete option for Windows. The reason for its creation is that I have been working on a simple file-cipher and figured that when I had the plain-text, it would make sense that I was able to securely delete it from the disk.

## Compilation

Clone the [repository](https://gitlab.com/BodneyC/secdel.git) and compile the program with `make` or `comp.bat`; use in situ or add to PATH.

```batch
    git clone https://github.com/BodneyC/secdel.git
    cd secdel/
    make
```

## Usage

The program takes file-paths at the command line and fills them with a random value, if **[-r]** is provided, the file is renamed also; both of these processes are done **[-l]** number of times (or once).

As the program creates a single byte array of the size of each file as default, the **[-s]** option can be used on larger files. The stream option **[-s]** means that the program will at most use 1MiB for each file in turn for the deletion.

**Command format (TCLAP output):**

	USAGE:

	   secdel.exe  [-l <Integer>] [-r] [-s] [--] [--version] [-h] <String> ...


	Where:

	   -l <Integer>,  --levels <Integer>
	     Number of times to wipe

	   -r,  --rename
	     Rename [-l] levels before deletion

	   -s,  --stream
	     Wipe in small chunks

	   --,  --ignore_rest
	     Ignores the rest of the labeled arguments following this flag.

	   --version
	     Displays version information and exits.

	   -h,  --help
	     Displays usage information and exits.

	   <String>  (accepted multiple times)
	     (required)  Input File-path


	   Secure deletion tool for Windows
