#include <Race.hpp>

uint32_t Race::calculateOptionsForBeatingRecord(std::vector<std::string> lines)
{
    std::vector<uint64_t> times;
    std::vector<uint64_t> distances;

    for(auto line : lines)
    {
        std::cout << "Analyze lines : " << lines.size() << std::endl;;
        bool time = true;
        std::vector<std::string> parts = Reader::splitString(line, " ");
        std::cout << "Line: " << line << std::endl;
        for(auto part : parts)
        {
            std::cout << part << " ";
        }
        std::cout << std::endl;
        if(std::string::npos != parts.at(0).find("Distance:"))
        {
            time = false;
        }

        parts.erase(parts.begin());

        std::string result = std::accumulate(parts.begin(), parts.end(), std::string(""));
        std::cout << "Result " << result << std::endl;

        // for(auto part : parts)
        // {
            uint64_t value = Reader::stringToNumber(result);
            if(true == time)
            {
                times.emplace_back(value);
            }
            else
            {
                distances.emplace_back(value);
            }
        // }
    }

    uint32_t totalOptions = 1;

    for(auto i = 0; i < times.size(); i++)
    {
        totalOptions *= calculateDistance(times.at(i), distances.at(i));
    }
    return totalOptions;
}

uint32_t Race::calculateDistance(uint64_t time, uint64_t distance)
{
    // std::cout << "Time: " << time << ", distance: " << distance << std::endl;
    uint64_t options = 0;

    for(auto i = 1; i < time; i++)
    {
        uint64_t speed = i;
        uint64_t crossedDistance = speed * (time - i);

        // std::cout << "Speed: " << speed << ", time sailing: " << time-i << ", crossed distance: " << crossedDistance << std::endl;

        if(crossedDistance > distance)
        {
            uint64_t finalGoodTime = time - i;
            options = finalGoodTime - i + 1;
            std::cout << "Final good time: " << finalGoodTime << ", options: " << options << std::endl;
            std::cout << "Speed: " << speed << ", time sailing: " << time-i << ", crossed distance: " << crossedDistance << std::endl;
            break;
        }
    }
    return options;
}