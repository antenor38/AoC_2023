#include <HotSprings.hpp>

uint64_t HotSprings::calculateArrangement(vector<Spring> springRow, vector<uint8_t> instructions)
{
    auto instructionIt = instructions.begin();
    uint8_t damagedCount = 0;
    uint8_t foundCombination = 1;
    size_t pos = 0;
    auto remainingInstructions(instructions);
    bool atLeastOneDamaged = false;

    for(size_t i = 0; i < springRow.size(); i++)
    {
        uint8_t instruction = 0;
        auto spring = springRow.at(i);
        if(Spring::Damaged == spring || Spring::Unknown == spring)
        {
            if(Spring::Damaged == spring)
            {
                atLeastOneDamaged = true;
            }

            if(0 == pos && 0 != i)
            {
                pos = i;
            }
            
            if(instructions.end() != instructionIt)
            {
                instruction = *instructionIt;
            }
            else if(Spring::Damaged == spring)
            {
                foundCombination = 0;
                break;
            }
            else // Unknown spring
            {
                continue;
            }

            damagedCount++;
            if(damagedCount == instruction)
            {
                instructionIt++;
                remainingInstructions.erase(remainingInstructions.begin());
                damagedCount = 0;
                i++;
                if(0 == pos && 0 != i)
                {
                    pos = i;
                }
                cout << "i = " << i << endl;
                if(springRow.size() <= i)
                {
                    break;
                }
                if(Spring::Damaged == springRow.at(i))
                {
                    foundCombination = 0;
                    break;
                }
            }

        }
    }

    cout << "Got here, pos: " << pos << endl;

    vector<Spring> nextSpringRow = {};
    vector<uint8_t> nextInstructions = {};

    if(0 != pos && springRow.size() >= pos)
    {
        nextSpringRow = {springRow.begin() + pos, springRow.end()};
    }

    uint8_t instructionsSum = 0;
    for(auto instruction : nextInstructions)
    {
        instructionsSum += instruction + 1;
    }

    if(0 < instructionsSum && nextSpringRow.size() >= instructionsSum)
    {
        foundCombination += calculateArrangement(nextSpringRow, nextInstructions);
    }
    return foundCombination;
}

uint64_t HotSprings::calculateSpringArrangements(vector<string> lines)
{
    vector<vector<Spring>> springRows;
    vector<vector<uint8_t>> allInstructions;

    for(auto line : lines)
    {
        vector<Spring> springRow;
        vector<uint8_t> instructions;
        auto split = Reader::splitString(line, " ");
        for(char c : split.at(0))
        {
            Spring spring = Spring::Operational;
            switch (c)
            {
            case '#':
                spring = Spring::Damaged;
                break;
            case '?':
                spring = Spring::Unknown;
                break;
            default:
                break;
            }
            springRow.push_back(spring);
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
        sum += calculateArrangement(springRows.at(i), allInstructions.at(i));
    }
    return sum;
}