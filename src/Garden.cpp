#include <Garden.hpp>

Garden::Garden() : 
      mLowLocation(0)
    , kSeeds("seeds: ")
    , kSTS("seed-to-soil map:")
    , kSTF("soil-to-fertilizer map:")
    , kFTW("fertilizer-to-water map:")
    , kWTL("water-to-light map:")
    , kLTT("light-to-temperature map:")
    , kTTH("temperature-to-humidity map:")
    , kHTL("humidity-to-location map:")
    {}

uint64_t Garden::findCLosestLocationForSeed(std::string inputData)
{
    std::vector<std::string> sections = Reader::splitString(inputData, "\n\n");

    std::cout << "Num of sections: " << sections.size() << std::endl;

    std::vector<std::string> seeds = Reader::splitString(sections.at(0), " ");

    //To remove 'seeds:' string
    seeds.erase(seeds.begin());

    std::vector<std::tuple<uint64_t, uint64_t>> localSeeds;

    for(auto seediterator = seeds.begin(); seediterator != seeds.end(); seediterator++)
    {
        uint64_t seed = Reader::stringToNumber(*seediterator);
        uint64_t count = Reader::stringToNumber(*(++seediterator));
        localSeeds.emplace_back(std::make_tuple(seed, count));
    }

    std::sort(localSeeds.begin(), localSeeds.end(), [](auto first, auto second){return std::get<0>(first) < std::get<0>(second);});

    // for(auto seedTuple : localSeeds) //(auto seediterator = seeds.begin(); seediterator != seeds.end(); seediterator++)
    // {
    //     std::vector<uint64_t> temp;
    //     uint64_t seed = std::get<0>(seedTuple);//Reader::stringToNumber(*seediterator);
    //     uint64_t count = std::get<1>(seedTuple);//Reader::stringToNumber(*(++seediterator));
    //     std::cout << "Seed: " << seed << ", count: " << count << std::endl;
    //     if(false == mSeeds.empty() && seed < mSeeds.at(0))
    //     {
    //         for(auto i = 0; i < count; i++)
    //         {
    //            temp.emplace_back(seed + i);
    //         }
    //         mSeeds.insert(mSeeds.begin(), temp.begin(), temp.end());
    //     }
    //     else
    //     {
    //         for(auto i = 0; i < count; i++)
    //         {
    //             mSeeds.emplace_back(seed + i);
    //         }
    //     }
    // }

    // std::sort(mSeeds.begin(), mSeeds.end());

    std::cout << "Seeds sorted\n";

    //Remove first section with 'seeds'
    sections.erase(sections.begin());

    for(auto section : sections)
    {
        std::vector<std::string> lines = Reader::splitString(section, "\n");
        std::map<uint64_t, std::tuple<uint64_t, uint64_t>>* workMap;
        // std::cout << "Section:\n" << section << std::endl;
        if(std::string::npos != lines.at(0).find(kSTS))
        {
            workMap = & mSeedToSoilMap;
        }
        else if(std::string::npos != lines.at(0).find(kSTF))
        {
            workMap = & mSoilToFertilizerMap;
        }
        else if(std::string::npos != lines.at(0).find(kFTW))
        {
            workMap = & mFertilizerToWaterMap;
        }
        else if(std::string::npos != lines.at(0).find(kWTL))
        {
            workMap = & mWaterToLightMap;
        }
        else if(std::string::npos != lines.at(0).find(kLTT))
        {
            workMap = & mLightToTemplMap;
        }
        else if(std::string::npos != lines.at(0).find(kTTH))
        {
            workMap = & mTempToHumidityMap;
        }
        else if(std::string::npos != lines.at(0).find(kHTL))
        {
            workMap = & mHumidityToLocationMap;
        }

        lines.erase(lines.begin());
        populateMap(*workMap, lines);
        // std::cout << "Map size: " << workMap->size() << std::endl;
    }

    return findLowestLocation(localSeeds);


    uint64_t count = 0;
    for(auto seed : mSeeds)
    {
        count++;
        // uint64_t src = calculateNextSource(mSeedToSoilMap, seed);
        // std::cout << "Src: " << src << ", ";
        // src = calculateNextSource(mSoilToFertilizerMap, src);
        // std::cout << "Src: " << src << ", ";
        // src = calculateNextSource(mFertilizerToWaterMap, src);
        // std::cout << "Src: " << src << ", ";
        // src = calculateNextSource(mWaterToLightMap, src);
        // std::cout << "Src: " << src << ", ";
        // src = calculateNextSource(mLightToTemplMap, src);
        // std::cout << "Src: " << src << ", ";
        // src = calculateNextSource(mTempToHumidityMap, src);
        // std::cout << "Src: " << src << ", ";
        // src = calculateNextSource(mHumidityToLocationMap, src);
        // std::cout << "Src: " << src << std::endl;

        uint64_t location = calculateNextSource(mHumidityToLocationMap
                                , calculateNextSource(mTempToHumidityMap
                                , calculateNextSource(mLightToTemplMap
                                , calculateNextSource(mWaterToLightMap
                                , calculateNextSource(mFertilizerToWaterMap
                                , calculateNextSource(mSoilToFertilizerMap
                                , calculateNextSource(mSeedToSoilMap, seed)))))));
        mLocation.emplace_back(location);

        if(count%100000)
        {
            std::sort(mLocation.begin(), mLocation.end());
            uint64_t saveLoc = mLocation.at(0);
            mLocation.clear();
            mLocation.emplace_back(saveLoc);
            std::cout << mLocation.at(0) << std::endl;
        }
    }

    std::sort(mLocation.begin(), mLocation.end());

    return mLocation.at(0);
}

void Garden::populateMap(std::map<uint64_t, std::tuple<uint64_t, uint64_t>>& map, std::vector<std::string> lines )
{
    for(auto line : lines)
    {
        std::vector<std::string> numStrings = Reader::splitString(line, " ");
        std::tuple<uint64_t, uint64_t> destinationAndRange = std::make_tuple(Reader::stringToNumber(numStrings.at(0)), Reader::stringToNumber(numStrings.at(2)));
        map.emplace(Reader::stringToNumber(numStrings.at(1)), destinationAndRange);
    }
}

uint64_t Garden::calculateNextSource(std::map<uint64_t, std::tuple<uint64_t, uint64_t>>& map, uint64_t currentSource)
{
    uint64_t nextSource = currentSource;
    for(auto iterator = map.begin(); iterator != map.end(); iterator++)
    {
        if(iterator->first <= currentSource)
        {
            if((iterator->first + std::get<1>(iterator->second)) > currentSource)
            {
                int64_t diff =std::get<0>(iterator->second) - iterator->first;
                // std::cout << "Diff " << diff << "| ";
                // std::cout << "Current source + diff = " << static_cast<int64_t>(currentSource) + diff << "| ";
                nextSource = static_cast<uint64_t>(static_cast<int64_t>(currentSource) + diff);
                break;
            }
        }
    }

    // std::cout << "Current: " << currentSource << ", Next: " << nextSource << std::endl;

    return nextSource;
}

uint64_t Garden::findSource(std::map<uint64_t, std::tuple<uint64_t, uint64_t>>& map, uint64_t destination)
{
    uint64_t source = destination; 
    // std::cout << "Destination: " << destination << std::endl;
    for(auto iterator = map.begin(); iterator != map.end(); iterator++)
    {
        if(std::get<0>(iterator->second) <= destination)
        {
            if((std::get<0>(iterator->second) + std::get<1>(iterator->second)) > destination)
            {
                int64_t diff =iterator->first - std::get<0>(iterator->second);
                // std::cout << "Diff " << diff << "| ";
                // std::cout << "Current source + diff = " << static_cast<int64_t>(currentSource) + diff << "| ";
                source = static_cast<uint64_t>(static_cast<int64_t>(destination) + diff);
                break;
            }
        }
    }
    // std::cout << "Source: " << source << std::endl;
    return source;
}

uint64_t Garden::findLowestLocation(std::vector<std::tuple<uint64_t, uint64_t>> seeds)
{
    for(uint64_t i = 0; i < UINT64_MAX; i++)
    {
        uint64_t seedSource = findSource(mSeedToSoilMap,
                                findSource(mSoilToFertilizerMap,
                                findSource(mFertilizerToWaterMap,
                                findSource(mWaterToLightMap,
                                findSource(mLightToTemplMap,
                                findSource(mTempToHumidityMap,
                                findSource(mHumidityToLocationMap, i)))))));
        
        for(auto seed : seeds)
        {
            if(std::get<0>(seed) <= seedSource && ((std::get<0>(seed) + std::get<1>(seed)) > seedSource))
            {
                return i;
            }
        }
    }
}