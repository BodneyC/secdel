#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm> // std::fill
#include <cstdlib> // std::srand
#include <ctime> // std::time
#include <cstring> // std::time
#include <tclap/CmdLine.h>
#include "secdel.h"

struct inputInfo {
	std::string pathOnly, filenameOnly, newFilename;
	std::vector<std::string> inputStrings;
	int levelVal;
	bool streamVal, renameVal;
} CMDArgs;
/*------------------------------------------------------------
RNG dependant
------------------------------------------------------------*/
BYTE getRandomNumber(){
    return std::rand() & 0xFF;
}
/*------------------------------------------------------------
Wiping function
------------------------------------------------------------*/
void wipeFunction (inputFile &cFile) {
	BYTE *byteArr, randVal;
	std::ios fmtCout(NULL); // Default formatting saved
	fmtCout.copyfmt(std::cout);

	if (CMDArgs.streamVal) {
		int loopcount = cFile.getInputInfo() / MEGABYTE;
		int loopremainder = cFile.getInputInfo() % MEGABYTE;
		byteArr = new BYTE[MEGABYTE];

		for (int levCount = 0; levCount < CMDArgs.levelVal; levCount++) {
			randVal = getRandomNumber();
			for (int i = 0; i < loopcount; i++) {
				memset (byteArr, randVal, MEGABYTE);
				cFile.writeToFile (byteArr, MEGABYTE);
			}
			randVal = getRandomNumber();
			memset (byteArr, randVal, MEGABYTE);
			cFile.writeToFile (byteArr, loopremainder);

			std::cout.copyfmt(fmtCout);
			std::cout << "Pass " << std::setw(2) << levCount << ": written value: " << std::hex << std::setfill('0') << std::internal << std::showbase << std::setw(4) << (int)randVal << '\n';
		}
	} else {
		byteArr = new BYTE[cFile.getInputInfo()];
		for (int levCount = 0; levCount < CMDArgs.levelVal; levCount++) {
			randVal = getRandomNumber();
			memset (byteArr, randVal, cFile.getInputInfo());
			cFile.writeToFile (byteArr, cFile.getInputInfo());
			cFile.resetFilePointer();
			std::cout.copyfmt(fmtCout);
			std::cout << "Pass " << std::setw(2) << levCount << ": written value: " << std::hex << std::setfill('0') << std::internal << std::showbase << std::setw(4) << (int)randVal << '\n';
		}
	}
	std::cout.copyfmt(fmtCout);
	delete[] byteArr;
}
/*------------------------------------------------------------
Renaming function
------------------------------------------------------------*/
std::string renameFunction (inputFile &cFile, std::string origName) {
	CMDArgs.newFilename.clear();
	CMDArgs.newFilename = CMDArgs.pathOnly;
	std::string tempFilename = origName;
	char ch = 'A';
	bool renameSucc;

	for (int o = 0; o < CMDArgs.levelVal && o < 25; o++) {
		for (int i = 0; i < 10; i++) {
			CMDArgs.newFilename.insert(CMDArgs.newFilename.end(), 1, ch);
		}
		renameSucc = cFile.renameFile(tempFilename.c_str(), CMDArgs.newFilename.c_str());
		tempFilename = CMDArgs.newFilename;
		CMDArgs.newFilename.erase(CMDArgs.newFilename.end() - 10, CMDArgs.newFilename.end());
		if (renameSucc) {
			std::cout << "Pass " << std::setw(2) << o << ": renamed as: ";
			for (int i = 0; i < 10; i++) {
				std::cout << ch;
			}
			std::cout << '\n';
		}
		ch++;
	}
	return tempFilename;
}
/*------------------------------------------------------------
Reverse iterate the provided file path
------------------------------------------------------------*/
void retPathname (std::string origName) {
	std::string::reverse_iterator revIt = origName.rbegin();
	CMDArgs.pathOnly.clear();

	while (*revIt != '\\') {
		revIt++;
	}
	if (revIt > origName.rend()) {
		CMDArgs.pathOnly = ".\\";
	} else {
		for (; revIt < origName.rend(); revIt++) {
			CMDArgs.pathOnly.push_back(*revIt);
		}
		std::reverse (CMDArgs.pathOnly.begin(), CMDArgs.pathOnly.end());
	}
}
/*------------------------------------------------------------
Reverse iterate the provided file path
------------------------------------------------------------*/
void retFilename (std::string origName) {
	std::string::reverse_iterator revIt = origName.rbegin();
	CMDArgs.filenameOnly.clear();

	while (*revIt != '\\') {
		CMDArgs.filenameOnly.push_back(*revIt);
		revIt++;
	}
	if (revIt > origName.rend()) {
		CMDArgs.filenameOnly = origName;
	} else {
		std::reverse (CMDArgs.filenameOnly.begin(), CMDArgs.filenameOnly.end());
	}
}
/*------------------------------------------------------------
Argument Returning (CMDArgs)
------------------------------------------------------------*/
void returnArgs (int argn, char const *args[]) {
	try {
		TCLAP::CmdLine cmd("Secure deletion tool for Windows", ' ', "0.01");
		TCLAP::UnlabeledMultiArg<std::string> inputArg("input", "Input File-path", true, "String", false);
		TCLAP::ValueArg<int> levelArg("l", "levels", "Number of times to wipe", false, 1, "Integer");
		TCLAP::SwitchArg streamSwitch("s", "stream", "Wipe in small chunks", cmd, 0);
		TCLAP::SwitchArg renameSwitch("r", "rename", "Rename [-l] levels before deletion", cmd, 0);
		cmd.add(inputArg);
		cmd.add(levelArg);
		cmd.parse(argn, args);
		CMDArgs.inputStrings = inputArg.getValue();
		CMDArgs.levelVal = levelArg.getValue();
		CMDArgs.renameVal = renameSwitch.getValue();
		CMDArgs.streamVal = streamSwitch.getValue();
	} catch (TCLAP::ArgException &e) {
		std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
	}
}
/*----------------------------------------------------
MAIN()
----------------------------------------------------*/
int main(int argc, char const *argv[]) {
	std::srand(std::time(0));
	returnArgs(argc, argv);

	for (std::vector<std::string>::const_iterator inputIt = CMDArgs.inputStrings.begin(); inputIt != CMDArgs.inputStrings.end(); inputIt++) {
		retPathname(*inputIt); retFilename(*inputIt);
		std::string finalFilename = *inputIt;

		LPCTSTR tempName = finalFilename.c_str();
		inputFile cFile (tempName);
		cFile.openFile();
		std::cout << "\nInput file: " << '\t' << CMDArgs.filenameOnly << '\n';
		if (cFile.hInput == INVALID_HANDLE_VALUE) {
			std::cout << "Failed to open handle to file. Error code: " << GetLastError() << '\n';
			return 1;
		}
		std::cout << "Filesize: " << '\t' << cFile.getInputInfo() << " bytes\n\nWiping File Contents:\n";
		wipeFunction(cFile);
		cFile.closeHandle();
		if (CMDArgs.renameVal) {
			std::cout << "\nWiping Filename:" << '\n';
			finalFilename = renameFunction(cFile, *inputIt);
		}
		if (!cFile.deleteFile(finalFilename.c_str())) {
			std::cout << "\nCould not delete file. Error code: " << GetLastError() << '\n';
			return 1;
		} else {
			std::cout << "\nFile deleted (securely)" << '\n';
			std::cout << '\n' << "File removed Successfully" << '\n';
		}
	}
	std::cout << '\n' << "Program Completed Successfully" << '\n';
	return 0;
}
