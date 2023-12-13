#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <map>
#include <Reader.hpp>

class CamelCard
{
public:
    CamelCard(){}

    enum Type
    {
        HighCard = 0,
        OnePair,
        TwoPair,
        Three,
        FullHouse,
        Four,
        Five
    };

    uint64_t calculateTotalWinnings(std::vector<std::string> lines);

private:
    std::tuple<Type, std::vector<uint8_t>, uint16_t> processHand(std::string hand);

    bool compareCard(std::tuple<Type, std::vector<uint8_t>, uint16_t> const& first, std::tuple<Type, std::vector<uint8_t>, uint16_t> const& second);

    uint32_t mSpeed;
    uint32_t mTime;
};