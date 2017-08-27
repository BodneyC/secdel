# SECDEL

This is very small secure delete option for Windows.

- Opens the file and gets its size
- Creates a static array of file-size (zero initialised)
- Writes array to file
- Deletes file

Written in C++ via WinAPI.
