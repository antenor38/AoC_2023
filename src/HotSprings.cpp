#include <HotSprings.hpp>

uint64_t HotSprings::calculateCombinations(bitset<100> const& springRow, vector<size_t> groupBorders, vector<tuple<uint64_t, size_t>> instructions)
{
    uint64_t combinations = 0;
    auto currentInstructionTuple = instructions.at(0);
    uint64_t currentInstr = get<0>(currentInstructionTuple);
    bitset<100> currentInstruction = currentInstr;
    int32_t currentInstructionSize = static_cast<int32_t>(get<1>(currentInstructionTuple));

    int32_t rowSize = static_cast<int32_t>(groupBorders.back());

    instructions.erase(instructions.begin());

    // bitset<34> rowBits = springRow;

    if(0 == rowSize)
    {
        return 0;
    }

    for(int32_t i = 0; i < (rowSize - currentInstructionSize + 1); i++)
    {
        bool insideRange = true;
        uint8_t leftBetweenNextBorder = 1;
        for(auto const& border : groupBorders)
        {
            if(0 == border)
            {
                continue;
            }
            insideRange = ((i + currentInstructionSize) <= border) || (border <= i);
            if(false == insideRange)
            {
                i = border - 1;
                break;
            }
            else if((i + currentInstructionSize) == border)
            {
                leftBetweenNextBorder = 0;
                break;
            }
        }

        if(false == insideRange)
        {
            continue;
        }

        uint32_t unimportantBits = 100- (i + currentInstructionSize + leftBetweenNextBorder);
        // bitset<100> currentInstrBit = (currentInstruction << i) << unimportantBits;
        bitset<100> springRowWork = (springRow << unimportantBits) | ((currentInstruction << i) << unimportantBits);
        // cout << ", Current row size: " << rowSize << ", row:\n" << rowBits.to_string() << endl;
        // cout << springRowWork.to_string() << endl << currentInstrBit.to_string() << endl;
        // cout << "i = " << i << ", currentInstrSize = " << currentInstructionSize << ", leftBetweenNextBorder = " << static_cast<uint32_t>(leftBetweenNextBorder) << endl;
        if(currentInstructionSize == springRowWork.count())
        {
            // cout << "Match!\n\n";
            uint64_t possibleCombinations = 1;
            if(instructions.size() > 0)
            {
                uint64_t diff = static_cast<uint64_t>(i + currentInstructionSize + leftBetweenNextBorder);
                bitset<100> nextSpringRow = springRow >> diff;
                auto nextGroupBorders = groupBorders;

                for_each(nextGroupBorders.begin(), nextGroupBorders.end(), [&diff](auto& border){
                    if(border >= diff)
                    {
                        border -= diff;
                    }
                    else
                    {
                        border = 0;
                    }
                });
                possibleCombinations *= calculateCombinations(nextSpringRow, nextGroupBorders, instructions);
            }
            combinations += possibleCombinations;
        }
        else if (i > 0)
        {
            bitset<100> passedBits = springRow << (100- i);
            // bitset<100> passedBitsPrint = passedBits;
            // cout << "leftover bits:\n" << passedBitsPrint.to_string() << endl << endl;
            if(passedBits.count() != 0)
            {
                break;
            }
        }
    }

    // cout << "Return combinations: " << combinations << endl << endl;
    return combinations;
}

uint64_t HotSprings::findCombinations(tuple<bitset<100>, vector<size_t>> groups, vector<uint8_t> instructions)
{
    uint64_t finalCombinations = 0;
    bitset<100> const springRow = get<bitset<100>>(groups);
    vector<size_t> groupBorders = get<vector<size_t>>(groups);
    vector<tuple<uint64_t, size_t>> bitInstructions;

    for(auto instruction : instructions)
    {
        uint64_t bitInstruction = 0;
        for(uint8_t i = 0; i < instruction; i++)
        {
            bitInstruction <<= 1;
            bitInstruction += 1;
        }
        bitInstructions.push_back(make_tuple(bitInstruction, static_cast<size_t>(instruction)));
    }

    finalCombinations = calculateCombinations(springRow, groupBorders, bitInstructions);

    return finalCombinations;
}

void HotSprings::parseSpringRowsAndInstruction(bitset<100>& springRow, string const& springRowString, vector<size_t>& groupBorders, vector<uint8_t>& instructions, string const& instructionsString)
{
    bitset<100> mark = 0;
    uint64_t count = 0;
    bool parsingGroup = false;

    for(char c : springRowString)
        {
            switch (c)
            {
            case '#':
                mark = static_cast<bitset<100>>(1) << count;
                springRow |= mark;
                count++;
                parsingGroup = true;
                // cout << "mark: " << mark << ", count: " << count << " | ";
                break;
            case '?':
                count++;
                parsingGroup = true;
                break;
            default:
                if(true == parsingGroup)
                {
                    parsingGroup = false;
                    groupBorders.push_back(count);
                }
                break;
            }
        }

        if(true == parsingGroup)
            {
                groupBorders.push_back(count);
            }

        auto instructionsStr = Reader::splitString(instructionsString, ",");
        for(auto instructionStr : instructionsStr)
        {
            instructions.push_back(Reader::stringToNumber(instructionStr));
        }

}

uint64_t HotSprings::calculateSpringArrangements(vector<string> lines)
{
    vector<tuple<bitset<100>, vector<size_t>>> springRows;
    vector<vector<uint8_t>> allInstructions;
    uint64_t sum = 0;
    uint16_t lineNum = 0;

    for(auto line : lines)
    {
        lineNum++;
        bitset<100> springRow = 0;
        
        vector<size_t> groupBorders;
        vector<uint8_t> instructions;

        auto split = Reader::splitString(line, " ");
        auto springRowString = split.at(0);
        auto instructionsString = split.at(1);

        auto springRowStringSecond = springRowString + "?" + springRowString;
        auto instructionsStringSecond = instructionsString + "," + instructionsString;

        auto springRowStringThird = springRowStringSecond + "?" + springRowString;
        auto instructionsStringThird = instructionsStringSecond + "," + instructionsString;

        // cout << springRowString << endl << instructionsString << endl;

        parseSpringRowsAndInstruction(springRow, springRowString, groupBorders, instructions, instructionsString);
        uint64_t firstResult = findCombinations(make_tuple(springRow, groupBorders), instructions);

        springRow = 0;
        groupBorders.clear();
        instructions.clear();

        parseSpringRowsAndInstruction(springRow, springRowStringSecond, groupBorders, instructions, instructionsStringSecond);
        uint64_t secondResult = findCombinations(make_tuple(springRow, groupBorders), instructions);

        springRow = 0;
        groupBorders.clear();
        instructions.clear();

        parseSpringRowsAndInstruction(springRow, springRowStringThird, groupBorders, instructions, instructionsStringThird);
        uint64_t thirdResult = findCombinations(make_tuple(springRow, groupBorders), instructions);

        long double diffSecondToFirst = static_cast<long double>(secondResult)/firstResult;
        long double diffThirdToSecond = static_cast<long double>(thirdResult)/secondResult;

        uint64_t totalResult = 0;

        if(diffSecondToFirst == diffThirdToSecond)
        {
            totalResult = thirdResult * static_cast<uint64_t>(powl(diffThirdToSecond, 2));
        }
        else
        {
            long double diffOfDiffs = diffThirdToSecond / diffSecondToFirst;
            long double diffFourthToThird = diffOfDiffs * diffThirdToSecond;
            long double diffFifthToFourth = diffOfDiffs * diffFourthToThird;

            uint64_t fourthResult = thirdResult * diffFourthToThird;
            totalResult = fourthResult * diffFifthToFourth;
        }
        cout << "totalResult = " << totalResult << endl;
        // if(thirdResult != totalResult)
        // {
        //     cout << "Line: " << lineNum << ", first result = " << firstResult << ", second = " << secondResult << ", third = " << thirdResult << endl;
        // }
        sum += totalResult;
        
        // bitset<100> bitRow(springRow);
        // cout << bitRow.to_string() << endl;
        // springRows.push_back(make_tuple(springRow, groupBorders));
        // allInstructions.push_back(instructions);
    }

    // for(size_t i = 0; i < springRows.size(); i++)
    // {
    //     cout << "processing line " << i+1 << endl;
    //     auto result = findCombinations(springRows.at(i), allInstructions.at(i));
    //     cout << "Result per row: " << result << endl << endl;
    //     sum += result;
    // }
    return sum;
}