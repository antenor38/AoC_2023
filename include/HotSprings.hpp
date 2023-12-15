#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <map>
#include <memory>
#include <bitset>
// #include <tgmath.h>
#include <Reader.hpp>

using namespace std;

class HotSprings
{
public:
    HotSprings(){}

    uint64_t calculateSpringArrangements(vector<string> lines);

    enum Spring
    {
        Operational = 0,
        Damaged,
        Unknown,
        Mixed
    };

    typedef vector<Spring> Group;
    typedef tuple<Spring, Group> GroupWithType;
    typedef vector<GroupWithType> GroupsVector;

private:
    vector<uint16_t> findEmptyColumns(vector<string> columns);
    uint64_t findCombinations(tuple<bitset<100>, vector<size_t>> groups, vector<uint8_t> instructions);
    uint64_t calculateCombinations(bitset<100> const& springRow, vector<size_t> groupBorders, vector<tuple<uint64_t, size_t>> instructions);

    void parseSpringRowsAndInstruction(bitset<100>& springRow, string const& springRowString, vector<size_t>& groupBorders, vector<uint8_t>& instructions, string const& instructionsString);
};