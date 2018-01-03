@echo off
REM: Short compilation file for secure deletion program for Windows
g++ -c fileinfoClass.cc && g++ -std=c++11 -Wall -I include -o secdel.exe secdel.cc fileinfoClass.o 
