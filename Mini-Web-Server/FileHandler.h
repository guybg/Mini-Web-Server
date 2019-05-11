#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class FileHandler
{
public:
	bool readFile(string i_filePath, string &o_fileContent);
	bool writeFile(string i_filePath, string fileContent);
	bool createFile(string i_filePath);
	bool isFileExists(string i_filePath);
};

#endif