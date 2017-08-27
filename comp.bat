@echo off
REM: Short compilation file for secure deletion program for Windows
g++ -c main.cc fileinfoClass.cc && g++ -Wall main.o fileinfoClass.o -o main.exe
