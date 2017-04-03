#pragma once
#ifndef TUTORIAL_FILE_HPP
#define TUTORIAL_FILE_HPP

#include <utils.hpp>

#include <fstream>
#include <sstream>

std::string ReadFile(std::string FileName) {
	std::ifstream File{FileName};
	Assert(File.is_open());

	std::string Result;

#ifdef MSVC
	struct stat Stat;
	if (stat(FileName.c_str(), &Stat) != -1) {
		Result.reserve(Stat.st_size);
	}

	char Buffer[1024];
	while (File.get(Buffer, SizeOf(Buffer), EOF)) {
		Result.append(Buffer);
	}
#else
	std::stringstream Buffer;
	Buffer << File.rdbuf();
	Result = Buffer.str();
#endif

	return Result;
}

#ifdef MSVC
#include <direct.h>
char* GetCurrentWorkingDirectory() {
	return _getcwd(nullptr, 0);
}
#endif


#endif //TUTORIAL_FILE_HPP
