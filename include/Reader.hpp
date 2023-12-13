#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Reader
{
public:
    Reader(std::string filename);

    std::vector<std::string> readLines();

    static std::vector<std::string> splitString(std::string line, std::string delimiter);

    static uint64_t stringToNumber(std::string numStr);

    void changeFile(std::string filename);
private:

    std::string mFilename;
};