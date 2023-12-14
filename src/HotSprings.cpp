#include <HotSprings.hpp>

vector<vector<uint8_t>>  HotSprings::findCombinations(GroupsVector groups, size_t const instructionsSize)
{
    vector<vector<vector<uint8_t>>> possibleCombinations;

    for(auto groupWithType : groups)
    {
        auto group = get<Group>(groupWithType);
        vector<vector<uint8_t>> groupCombination;
        auto type = get<Spring>(groupWithType);
        if(Spring::Damaged == type)
        {
            uint8_t groupSize = static_cast<uint8_t>(group.size());
            groupCombination.push_back({groupSize});
        }
        else
        {
            uint32_t damagedBits = 0;
            if(Spring::Mixed == type)
            {
                for(size_t i = 0; i < group.size(); i++)
                {
                    if(Spring::Damaged == group.at(i))
                    {
                        uint32_t bit = 1 << i;
                        damagedBits |= bit;
                    }
                }
            }
            vector<uint32_t> damagedBitsCombinations;
            size_t bitSize = 1 << group.size();
            for(size_t i = 0; i < bitSize; i++)
            {
                uint32_t bits = static_cast<uint32_t>(i);
                vector<uint8_t> combination;
                uint8_t count = 0;
                if(0 != damagedBits)
                {
                    bits |= damagedBits;
                    if(damagedBitsCombinations.end() != find(damagedBitsCombinations.begin(), damagedBitsCombinations.end(), bits))
                    {
                        continue;
                    }
                    else
                    {
                        damagedBitsCombinations.push_back(bits);
                    }
                }
                else if(0 == damagedBits && 0 == bits)
                {
                    combination.push_back(0);
                }

                while(0 != bits)
                {
                    if(1 == (bits & 0x01))
                    {
                        count++;
                    }
                    else if(0 != count)
                    {
                        combination.push_back(count);
                        count = 0;
                    }
                    bits >>= 1;
                }
                if(0 != count)
                {
                    combination.push_back(count);
                }
                if(false == combination.empty())
                {
                    // if(1 < combination.size())
                    // {
                    //     cout << "Reversing\n";
                    //     reverse(combination.begin(), combination.end());
                    // }
                    groupCombination.push_back(combination);
                }
            }
        }
        
        possibleCombinations.push_back(groupCombination);
    }

    vector<vector<uint8_t>> finalCombinations = possibleCombinations.at(0);

    for(size_t i = 1; i < possibleCombinations.size(); i++)
    {
        auto nextCombinations = possibleCombinations.at(i);
        vector<vector<uint8_t>> finalCombinationsTemp;
        for(auto finalCombination : finalCombinations)
        {
            for(auto nextCombination : nextCombinations)
            {
                auto nextFinalCombination = finalCombination;
                nextFinalCombination.insert(nextFinalCombination.end(), nextCombination.begin(), nextCombination.end());
                finalCombinationsTemp.push_back(nextFinalCombination);
            }
        }
        finalCombinations = finalCombinationsTemp;
    }

    for(auto& finalCombination : finalCombinations)
    {
        vector<uint8_t>::iterator pos;
        do
        {
            pos = find(finalCombination.begin(), finalCombination.end(), 0);
            if(finalCombination.end() != pos)
            {
                finalCombination.erase(pos);
            }
        } while (finalCombination.end() != pos);

        if(instructionsSize != finalCombination.size())
        {
            finalCombination.clear();
        }

        // if(false == finalCombination.empty())
        // {
        //     cout << "(";
        //     for(auto element : finalCombination)
        //     {
        //         cout << char(element + '0') << ",";
        //     }
        //     cout <<"), ";
        // }
    }
    // cout << endl << endl;

    return finalCombinations;
}

uint64_t HotSprings::calculateArrangement(vector<vector<uint8_t>> rowCombinations, vector<uint8_t> instructions)
{
    uint64_t foundCombination = 0;

    for(auto combination : rowCombinations)
    {
        if(combination.size() == instructions.size())
        {
            bool found = true;
            for(size_t i = 0; i < combination.size(); i++)
            {
                if(combination.at(i) != instructions.at(i))
                {
                    found = false;
                    break;
                }
            }
            if(found)
            {
                foundCombination++;
            }
        }
    }
    // cout << "Found: " << foundCombination << endl;
    // cout << "___________________________________________\n";
    return foundCombination;
}

uint64_t HotSprings::calculateSpringArrangements(vector<string> lines)
{
    vector<GroupsVector> springRows;
    vector<vector<uint8_t>> allInstructions;

    for(auto line : lines)
    {
        GroupsVector springRow;
        Group group;
        Spring type = Spring::Operational;
        vector<uint8_t> instructions;
        auto split = Reader::splitString(line, " ");
        for(char c : split.at(0))
        {
            switch (c)
            {
            case '#':
                group.push_back(Spring::Damaged);
                if(Spring::Unknown == type)
                {
                    type = Spring::Mixed;
                }
                else if(Spring::Operational == type)
                {
                    type = Spring::Damaged;
                }
                break;
            case '?':
                group.push_back(Spring::Unknown);
                if(Spring::Damaged == type)
                {
                    type = Spring::Mixed;
                }
                else if(Spring::Operational == type)
                {
                    type = Spring::Unknown;
                }
                break;
            default:
                if(false == group.empty())
                {
                    GroupWithType groupWithType = make_tuple(type, group);
                    springRow.push_back(groupWithType);
                    group.clear();
                    type = Spring::Operational;
                }
                break;
            }
        }
        if(false == group.empty())
        {
            GroupWithType groupWithType = make_tuple(type, group);
            springRow.push_back(groupWithType);
            group.clear();
            type = Spring::Operational;
        }

        auto instructionsStr = Reader::splitString(split.at(1), ",");
        for(auto instructionStr : instructionsStr)
        {
            instructions.push_back(Reader::stringToNumber(instructionStr));
        }
        springRows.push_back(springRow);
        allInstructions.push_back(instructions);
    }

    uint64_t sum = 0;
    for(size_t i = 0; i < springRows.size(); i++)
    {
        auto rowCombinations = findCombinations(springRows.at(i), allInstructions.at(i).size());
        sum += calculateArrangement(rowCombinations, allInstructions.at(i));
    }
    return sum;
}