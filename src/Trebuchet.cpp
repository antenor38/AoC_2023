#include <Trebuchet.hpp>

int Trebuchet::readCalibration(std::string line, bool firstRecursion)
{
    int result = 0;
    std::string numbers = "0123456789";
    std::vector<std::string> numberWords = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    std::vector<size_t> positions;

    if (false == firstRecursion)
    {
        for(auto& s : numberWords)
        {
            std::reverse(s.begin(), s.end());
        }
    }

    for(auto s : numberWords)
    {
        size_t wordPos = line.find(s);
        if(std::string::npos != wordPos)
        {
            positions.push_back(wordPos);
        }
        else
        {
            positions.push_back(INT32_MAX);
        }
    }

    size_t minPos = INT32_MAX;
    int minVal = 0;
    for(size_t i = 0; i < positions.size(); i++)
    {
        if(positions.at(i) < minPos)
        {
            minPos = positions.at(i);
            minVal = i + 1;
        }
    }

    if(minPos != INT32_MAX)
    {
        char number = '0' + minVal;
        line.at(minPos) = number;
    }

    size_t pos = line.find_first_of(numbers);

    if(std::string::npos != pos)
    {
        char digit = line.at(pos);
        result = digit - '0';
    }

    if(true == firstRecursion)
    {
        std::reverse(line.begin(), line.end());
        result = result * 10 + readCalibration(line, false);
    }

    return result;    
}