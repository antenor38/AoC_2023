#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <map>
#include <memory>
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
    uint64_t calculateArrangement(vector<vector<uint8_t>> rowCombinations, vector<uint8_t> instructions);
    vector<vector<uint8_t>> findCombinations(GroupsVector groups, size_t const instructionsSize);
};