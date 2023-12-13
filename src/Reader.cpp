#include <Reader.hpp>

Reader::Reader(std::string filename):
    mFilename(filename)
{
}

std::vector<std::string> Reader::readLines()
{
    std::vector<std::string> lines;
    std::ifstream input;
    input.open(mFilename);
    if(input.is_open())
    {
        std::string line;
        while(std::getline(input, line))
        {
            lines.push_back(line);

            // For Garden
            // lines.push_back("\n");
        }
    }
    else
    {
        std::cout << "Couldn't open file: " << mFilename << std::endl;
    }

    return lines;
}

std::vector<std::string> Reader::splitString(std::string line, std::string delimiter)
{
    std::vector<std::string> substrings;
    size_t pos = line.find(delimiter);
    if(std::string::npos != pos)
    {
        size_t continuePos = line.find_first_not_of(delimiter, pos);
        if(0 != pos)
        {
            substrings.emplace_back(line.substr(0, pos));
            if(std::string::npos != continuePos)
            {
                line.erase(0, continuePos);
            }
            else
            {
                line.clear();
            }
        }
        if(0 < line.size())
        {
            auto result = splitString(line, delimiter);
            substrings.insert(substrings.end(), result.begin(), result.end());
        }
    }
    else if (0 < line.size())
    {
        substrings.emplace_back(line);
    }
    return substrings;
}

uint64_t Reader::stringToNumber(std::string numStr)
{
    uint64_t number = 0;
    std::stringstream ss = std::stringstream(numStr);
    ss >> number;

    return number;
}

void Reader::changeFile(std::string filename)
{
    mFilename = filename;
}