#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <map>
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <Reader.hpp>

using namespace std;

struct DesertNode
{
    shared_ptr<DesertNode> leftNode;
    shared_ptr<DesertNode> rightNode;
    string name;
};

class WastelandInstructions
{
public:
    WastelandInstructions(){}

    uint64_t navigateDesert(std::vector<std::string> lines);

private:
    uint64_t followInstructions(vector<shared_ptr<DesertNode>> startNodes);
    uint64_t followInstructionThread(vector<shared_ptr<DesertNode>> startNodes);
    uint64_t followInstructionsInnovative(shared_ptr<DesertNode> startNode, string endNode);

    void threadFunction(shared_ptr<DesertNode> currentNode, atomic_bool& destinationReached, mutex& pathHitsLock
                        , uint16_t threadNum, map<uint64_t, uint8_t>& pathHits, uint64_t& result, bool master = false);

    bool allAtSameValue(vector<tuple<shared_ptr<DesertNode>, uint64_t>>& nodes);

    vector<uint8_t> mNavigation;
};