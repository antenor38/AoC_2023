#include <Scratchcard.hpp>

int Scratchcard::scanCard(std::string card)
{
    uint32_t value = 0;
    std::string values = card.substr(card.find(": "));
    std::string winNumsStr = values.substr(0, values.find(" | "));
    std::string numsStr = values.substr(values.find(" | "));

    std::vector<uint32_t> winNums = extractNums(winNumsStr);
    std::vector<uint32_t> nums = extractNums(numsStr);

    std::sort(winNums.begin(), winNums.end());
    std::sort(nums.begin(), nums.end());
    for(auto winNum : winNums)
    {
        if(std::binary_search(nums.begin(), nums.end(), winNum))
        {
            value++;
        }
    }
    return value;
}

int Scratchcard::collectCards(std::vector<std::string> originalCards)
{
    std::vector<std::tuple<std::string, uint32_t>> allCards;

    for(auto card : originalCards)
    {
        allCards.emplace_back(std::make_tuple(card, 1));
    }

    for(size_t i = 0; i < allCards.size(); i++)
    {
        int result = scanCard(std::get<std::string>(allCards[i]));
        for(size_t j = i + 1; j < i + 1 + result; j++)
        {
            std::get<uint32_t>(allCards[j]) += std::get<uint32_t>(allCards[i]);
        }
    }

    uint32_t sumCards = 0;
    for(auto card : allCards)
    {
        sumCards += std::get<uint32_t>(card);
    }

    return sumCards;
}

std::vector<uint32_t> Scratchcard::extractNums(std::string strNums)
{
    std::stringstream ss;
    std::vector<uint32_t> nums;

    ss << strNums;
    uint32_t num;
    std::string temp;
    while(!ss.eof())
    {
        ss >> temp;
        if(std::stringstream(temp) >> num)
        {
            nums.emplace_back(num);
        }
    }

    return nums;
}