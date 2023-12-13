#include <CamelCard.hpp>

std::tuple<CamelCard::Type, std::vector<uint8_t>, uint16_t> CamelCard::processHand(std::string hand)
{
    std::map<uint8_t, uint8_t> cardRepetition;
    std::tuple<CamelCard::Type, std::vector<uint8_t>, uint16_t> card;
    std::vector<uint8_t> cardValues;
    CamelCard::Type type = CamelCard::Type::HighCard;
    auto parts = Reader::splitString(hand, " ");

    uint8_t jNumber = 0;

    for(char c : parts.at(0))
    {
        uint8_t value = 0;
        if(c >= '2' && c <= '9')
        {
            value = c - '0';
        }
        else if(c == 'T')
        {
            value = 10;
        }
        else if(c == 'J')
        {
            value = 1;
            jNumber++;
            cardValues.push_back(value);
            continue;
        }
        else if(c == 'Q')
        {
            value = 11;
        }
        else if(c == 'K')
        {
            value = 12;
        }
        else if(c == 'A')
        {
            value = 13;
        }
        cardRepetition[value]++;
        cardValues.push_back(value);
    }

    uint8_t numTwos(0), numThrees(0);

    for(auto element : cardRepetition)
    {
        if(element.second == 2)
        {
            numTwos++;
        }
        else if(element.second == 3)
        {
            numThrees++;
        }
        else if(element.second == 4)
        {
            if(1 == jNumber)
            {
                type = CamelCard::Type::Five;
            }
            else
            {
                type = CamelCard::Type::Four;
            }
        }
        else if(element.second == 5)
        {
            type = CamelCard::Type::Five;
        }
    }

    if(2 == numTwos)
    {
        if(1 == jNumber)
        {
            type = CamelCard::Type::FullHouse;
        }
        else
        {
            type = CamelCard::Type::TwoPair;
        }
    }
    else if (1 == numTwos)
    {
        if(1 == numThrees)
        {
            type = CamelCard::Type::FullHouse;
        }
        else
        {
            switch (jNumber)
            {
            case 1:
                type = CamelCard::Type::Three;
                break;
            case 2:
                type = CamelCard::Type::Four;
                break;
            case 3:
                type = CamelCard::Type::Five;
                break;
            default:
                type = CamelCard::Type::OnePair;
                break;
            }
        }
    }
    else if (1 == numThrees)
    {
        switch (jNumber)
            {
            case 1:
                type = CamelCard::Type::Four;
                break;
            case 2:
                type = CamelCard::Type::Five;
                break;
            default:
                type = CamelCard::Type::Three;
                break;
            }
    }
    else
    {
        switch (jNumber)
            {
            case 5:
            case 4:
                type = CamelCard::Type::Five;
                break;
            case 3:
                type = CamelCard::Type::Four;
                break;
            case 2:
                type = CamelCard::Type::Three;
                break;
            case 1:
                if(CamelCard::Type::HighCard == type)
                {
                    type = CamelCard::Type::OnePair;
                }
                break;
            default:
                break;
            }
    }

    uint16_t bet = static_cast<uint16_t>(Reader::stringToNumber(parts.at(1)));

    return std::make_tuple(type, cardValues, bet);
}

bool CamelCard::compareCard(std::tuple<CamelCard::Type, std::vector<uint8_t>, uint16_t> const& first, std::tuple<Type, std::vector<uint8_t>, uint16_t> const& second)
{
    if(std::get<CamelCard::Type>(first) < std::get<CamelCard::Type>(second))
    {
        return true;
    }
    else if (std::get<CamelCard::Type>(first) == std::get<CamelCard::Type>(second))
    {
        auto firstHand = std::get<std::vector<uint8_t>>(first);
        auto secondHand = std::get<std::vector<uint8_t>>(second);

        for(size_t i = 0; i < firstHand.size(); i++)
        {
            if(firstHand.at(i) < secondHand.at(i))
            {
                return true;
            }
            else if(firstHand.at(i) > secondHand.at(i))
            {
                return false;
            }
        }
    }
    return false;
}

uint64_t CamelCard::calculateTotalWinnings(std::vector<std::string> lines)
{
    std::vector<std::tuple<CamelCard::Type, std::vector<uint8_t>, uint16_t>> allHands;
    uint64_t totalWinning = 0;

    for(auto line : lines)
    {
        allHands.push_back(processHand(line));
    }
    std::sort(allHands.begin(), allHands.end(), [this](auto const& first, auto const& second){return this->compareCard(first, second);});

    for(size_t i = 0; i < allHands.size(); i++)
    {
        auto bet = std::get<uint16_t>(allHands.at(i));
        uint64_t winning = (i+1) * bet;

        totalWinning += winning;
    }

    return totalWinning;
}