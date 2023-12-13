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

struct Node
{
    uint64_t xCoordinate;
    uint64_t yCoordinate;
    uint32_t name;
};

class CosmicExpansion
{
public:
    CosmicExpansion(){}

    uint64_t calculateShortestPaths(vector<string> lines);

private:
    vector<uint16_t> findEmptyColumns(vector<string> columns);
    uint64_t calculateShortestPath(Node const& first, Node const& second); 

    vector<uint8_t> mNavigation;
};