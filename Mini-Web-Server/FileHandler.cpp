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
		webFile.close();
	}
}
