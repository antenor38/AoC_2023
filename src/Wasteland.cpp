#include <Wasteland.hpp>

#ifdef _WIN32
#include <windows.h>
#elif __unix
#include <pthread.h>
#endif


uint64_t WastelandInstructions::navigateDesert(std::vector<std::string> lines)
{
    map<string, shared_ptr<DesertNode>> nodes;
    vector<shared_ptr<DesertNode>> startNodes;
    vector<string> endNodes;
    string instructions = lines.at(0);

    for(auto c : instructions)
    {
        if('L' == c)
        {
            mNavigation.push_back(0);
        }
        else
        {
            mNavigation.push_back(1);
        }
    }

    lines.erase(lines.begin());
    lines.erase(lines.begin());

    for(auto line : lines)
    {
        string nodeName = line.substr(0, 3);
        auto childNames = (Reader::splitString(line, "(")).at(1);
        string leftNodeName = childNames.substr(0, 3);
        auto rightName = (Reader::splitString(childNames, " ")).at(1);
        string rightNodeName = rightName.substr(0, 3);
        // std::cout << "Names: " << nodeName << ", " << leftNodeName << ", " << rightNodeName << std::endl;
        if(nodes.end() == nodes.find(nodeName))
        {
            nodes[nodeName] = make_shared<DesertNode>();
            nodes[nodeName]->name = nodeName;
        }
        if(nodes.end() == nodes.find(leftNodeName))
        {
            nodes[leftNodeName] = make_shared<DesertNode>();
            nodes[leftNodeName]->name = leftNodeName;
        }
        nodes[nodeName]->leftNode = nodes[leftNodeName];
        if(nodes.end() == nodes.find(rightNodeName))
        {
            nodes[rightNodeName] = make_shared<DesertNode>();
            nodes[rightNodeName]->name = rightNodeName;
        }
        nodes[nodeName]->rightNode = nodes[rightNodeName];

        if(string::npos != nodeName.find("A"))
        {
            startNodes.push_back(nodes[nodeName]);
        }
        if(string::npos != nodeName.find("Z"))
        {
            endNodes.push_back(nodeName);
        }
    }

    map<uint64_t, uint8_t> pathHits;

    // for(auto node : startNodes)
    // {
    //     for(auto endName : endNodes)
    //     {
    //         auto result = followInstructionsInnovative(node, endName);
    //         pathHits[result]++;
    //     }
    // }

    // for(auto path : pathHits)
    // {
    //     if(startNodes.size() == path.second)
    //     {
    //         return path.first;
    //     }
    // }

    return followInstructionThread(startNodes);
}

void WastelandInstructions::threadFunction(shared_ptr<DesertNode> currentNode, atomic_bool& destinationReached
                    , mutex& pathHitsLock, uint16_t threadNum, map<uint64_t, uint8_t>& pathHits, uint64_t& result, bool master)
{
    uint64_t steps = 0;

    while(false == destinationReached)
    {
        for(auto instruction : mNavigation)
        {
            if(true == destinationReached)
            {
                break;
            }
            if(0 == instruction)
            {
                currentNode = currentNode->leftNode;
            }
            else
            {
                currentNode = currentNode->rightNode;
            }

            steps++;

            if(string::npos != currentNode->name.find("Z"))
            {
                unique_lock<mutex> lock(pathHitsLock);

                pathHits[steps]++;
            
                if(true == master)
                {
                    for(auto path : pathHits)
                    {
                        if(threadNum == path.second)
                        {
                            destinationReached = true;
                            result = path.first;
                            break;
                        }
                    }
                }
            }
            if(true == master && steps % 100000)
            {
                unique_lock<mutex> lock(pathHitsLock);
                for(auto path : pathHits)
                {
                    if(threadNum == path.second)
                    {
                        destinationReached = true;
                        result = path.first;
                        break;
                    }
                }
            }
        }
    }
}

uint64_t WastelandInstructions::followInstructionThread(vector<shared_ptr<DesertNode>> startNodes)
{
    vector<thread> allThreads;
    atomic_bool destinationReached(false);
    map<uint64_t, uint8_t> pathHits;
    mutex pathHitsLock;
    uint16_t threadNum = static_cast<uint16_t>(startNodes.size());
    uint64_t result = 0;
    bool master = false;
    cout << "Nodes and threads size: " << threadNum << endl;
    for(size_t i = 0; i < startNodes.size(); i++)
    {
        if(startNodes.size() - 1 == i)
        {
            master = true;
        }
        auto node = startNodes.at(i);
        allThreads.push_back(move(thread([=, &destinationReached, &pathHits, &pathHitsLock, &result](){
            this->threadFunction(node, destinationReached, pathHitsLock, threadNum, pathHits, result, master);
            })));
    }

    cout << "All Threads started\n";
    for(size_t i = 0; i < allThreads.size(); i++)
    {
        auto thread = move(allThreads.at(i));
        thread.join();
    }

    return result;
}

uint64_t WastelandInstructions::followInstructions(vector<shared_ptr<DesertNode>> startNodes)
{
    vector<tuple<shared_ptr<DesertNode>, uint64_t>> currentNodes;

    for(auto node : startNodes)
    {
        currentNodes.push_back(make_tuple(node, 0));
    }

    bool allFinished = false;

    while(false == allFinished)
    {
        for(size_t i = 0; i < currentNodes.size(); i++)
        {
            bool destinationReached = false;
            auto currentNode = get<shared_ptr<DesertNode>>(currentNodes.at(i));
            auto& countSteps = get<uint64_t>(currentNodes.at(i));
            uint64_t maxValue = 0;
            if(0 != countSteps)
            {
                if(allAtSameValue(currentNodes))
                {
                    break;
                }
            }
            while(false == destinationReached)
            {
                for(auto instruction : mNavigation)
                {
                    if(0 == instruction)
                    {
                        currentNode = currentNode->leftNode;
                    }
                    else
                    {
                        currentNode = currentNode->rightNode;
                    }
                    countSteps++;
                    if(string::npos != currentNode->name.find("Z"))
                    {
                        destinationReached = true;
                        get<shared_ptr<DesertNode>>(currentNodes.at(i)) = currentNode;
                        break;
                    }
                }
                
                if(true == destinationReached)
                {
                    auto maxElement = max_element(currentNodes.begin(), currentNodes.end(), [](auto const& first, auto const& second){
                        return (get<uint64_t>(first) < get<uint64_t>(second));
                    });
                    maxValue = get<uint64_t>(*maxElement);

                    if(maxValue > countSteps)
                    {
                        destinationReached = false;
                    }
                }
            }
        }
        allFinished = true;
        sort(currentNodes.begin(), currentNodes.end(), [&allFinished](auto const& first, auto const& second){
            if(get<uint64_t>(first) < get<uint64_t>(second))
            {
                allFinished = false;
                cout << "Sorted\n";
                return true;
            }
            return false;
        });
        if(true == allFinished)
        {
            allFinished = allAtSameValue(currentNodes);
        }
    }

    return get<uint64_t>(currentNodes.at(0));
}

uint64_t WastelandInstructions::followInstructionsInnovative(shared_ptr<DesertNode> startNode, string endNode)
{
    bool destinationReached = false;
    auto currentNode = startNode;
    uint64_t countSteps = 0;
    uint64_t maxValue = 0;

    while(false == destinationReached)
    {
        for(auto instruction : mNavigation)
        {
            if(0 == instruction)
            {
                currentNode = currentNode->leftNode;
            }
            else
            {
                currentNode = currentNode->rightNode;
            }
            countSteps++;
            if(string::npos != currentNode->name.find(endNode))
            {
                destinationReached = true;
                break;
            }
        }
    }

    return countSteps;
}

bool WastelandInstructions::allAtSameValue(vector<tuple<shared_ptr<DesertNode>, uint64_t>>& nodes)
{
    bool result = true;
    auto maxElement = max_element(nodes.begin(), nodes.end(), [](auto const& first, auto const& second){
        return (get<uint64_t>(first) < get<uint64_t>(second));
    });
    auto minElement = min_element(nodes.begin(), nodes.end(), [](auto const& first, auto const& second){
        return (get<uint64_t>(first) < get<uint64_t>(second));
    });
    auto maxValue = get<uint64_t>(*maxElement);
    auto minVal = get<uint64_t>(*minElement);
    if(maxValue != minVal)
    {
        result = false;
    }
    return result;
}