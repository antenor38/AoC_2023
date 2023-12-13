#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include <numeric>
#include <Reader.hpp>

class Race
{
public:
    Race(){}

    uint32_t calculateOptionsForBeatingRecord(std::vector<std::string> lines);

private:
    uint32_t calculateDistance(uint64_t time, uint64_t distance);

    uint32_t mSpeed;
    uint32_t mTime;
};