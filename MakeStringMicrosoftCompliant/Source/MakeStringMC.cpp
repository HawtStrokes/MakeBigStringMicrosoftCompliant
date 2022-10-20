#include <fstream>
#include <string>
#include <vector>
#include <format>
#include <iostream>
#include <filesystem>

static void CreateMicrosoftCompliantStringCode(const std::string& inputFileName, const std::string& outputFileName, bool isMinimized = false, unsigned int maxSize = 2048) {
	std::vector<std::string> vecStr;
	std::ifstream ifs(inputFileName);
	std::string line;
	std::string buffer;
	unsigned count = 0;
	while (std::getline(ifs, line)) {
		for (char c : line) {
			if (count == maxSize - 1) {
				vecStr.push_back(buffer);
				buffer = "";
				count = 0;
			}
			if (isMinimized && c == ' ') continue;	// skip white spaces if minimized
			if (isMinimized && c == '\t') continue;	// skip tabs if minimized
			buffer += c;
			++count;
		}
	}
	if (count != 0) vecStr.push_back(buffer);	// push remaining contents if not empty
	ifs.close();

	std::ofstream ofs(outputFileName);

	for (size_t i = 0; i < vecStr.size(); ++i) {
		ofs << std::format(" std::string(R{})", std::format("\"({})\"", vecStr[i])) << (i == vecStr.size() - 1 ? ";" : " +") << std::endl;
	}
	ofs.close();
}

int main(int argc, char** argv) {
	auto Help = []() {
		std::cout << "./MakeStringMC <inputFile> <outputFile> <options>" << std::endl;
		std::cout << "Options:\nm\tMinimize Output" << std::endl;
	};

	if (argc > 4 || argc < 3) {
		Help();
		return -1;
	}

	if (!std::filesystem::exists(argv[1])) {std::cout << "isMinimized = true" << std::endl;
		std::cout << "Input File Does Not Exist!!!" << std::endl;
		return -1;
	}


	bool isMinimized = false;
	if (argc == 4)
	{
		if (std::string(argv[3]).find('m') != std::string::npos)
		{
			isMinimized = true;
			std::cout << "isMinimized = true" << std::endl;
		}
		else std::cout << "isMinimized = false" << std::endl;
	}
	CreateMicrosoftCompliantStringCode(argv[1], argv[2], isMinimized);
	std::cout << "Done!" << std::endl;
	return 0;
}