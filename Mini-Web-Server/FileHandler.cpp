#include "FileHandler.h"

bool FileHandler::readFile(string i_filePath, string &o_fileContent) {
	ifstream webFile;
	string lineContent;
	try {
		webFile.open(i_filePath);
		if(!webFile.is_open()) throw std::ios::failure("Error opening file!");
	}
	catch (std::ios::failure const& e) {
		cout << e.what();
		return 0;
	}

	if (webFile.is_open())
	{
		while (getline(webFile, lineContent))
		{
			o_fileContent += lineContent;
			o_fileContent += '\n';
		}
		o_fileContent = o_fileContent.substr(0, o_fileContent.length() - 1);
		webFile.close();
	}
}

bool FileHandler::isFileExists(string i_filePath) {
	ifstream file;
	bool isOpen;
	file.open(i_filePath);
	if (file.is_open()) {
		file.close();
		isOpen = true;
	}
	else {
		isOpen = false;
	}
	return isOpen;
}

bool FileHandler::writeFile(string i_filePath, string i_fileContent) {
	ofstream file;
	bool isOpen;

	file.open(i_filePath, ios::out);
	if (file.is_open()) {
		file << i_fileContent;
		file.close();
		isOpen = true;
	}
	else {
		isOpen = false;
	}
	
	return isOpen;
}
bool FileHandler::createFile(string i_filePath) {
	ofstream file;
	bool isCreated;

	file.open(i_filePath, ios::out | ios::trunc);
	if (file.is_open()) {
		file.close();
		isCreated = true;
	}
	else {
		isCreated = false;
	}

	return isCreated;
}
