#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <tuple>

class Scratchcard
{
public:
    Scratchcard(){}

    int scanCard(std::string card);

    int collectCards(std::vector<std::string> originalCards);

private:
    std::vector<uint32_t> extractNums(std::string strNums);
};