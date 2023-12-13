#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <Reader.hpp>

class Garden
{
public:
    Garden();

    uint64_t findCLosestLocationForSeed(std::string inputData);

private:

    void populateMap(std::map<uint64_t, std::tuple<uint64_t, uint64_t>>& map, std::vector<std::string> lines);

    uint64_t calculateNextSource(std::map<uint64_t, std::tuple<uint64_t, uint64_t>>& map, uint64_t currentSource);

    uint64_t findSource(std::map<uint64_t, std::tuple<uint64_t, uint64_t>>& map, uint64_t destination);

    uint64_t findLowestLocation(std::vector<std::tuple<uint64_t, uint64_t>> seeds);

    std::vector<uint64_t> mSeeds;
    std::vector<uint64_t> mLocation;
    uint64_t mLowLocation;

    std::map<uint64_t, std::tuple<uint64_t, uint64_t>> mSeedToSoilMap;
    std::map<uint64_t, std::tuple<uint64_t, uint64_t>> mSoilToFertilizerMap;
    std::map<uint64_t, std::tuple<uint64_t, uint64_t>> mFertilizerToWaterMap;
    std::map<uint64_t, std::tuple<uint64_t, uint64_t>> mWaterToLightMap;
    std::map<uint64_t, std::tuple<uint64_t, uint64_t>> mLightToTemplMap;
    std::map<uint64_t, std::tuple<uint64_t, uint64_t>> mTempToHumidityMap;
    std::map<uint64_t, std::tuple<uint64_t, uint64_t>> mHumidityToLocationMap;

    std::string const kSeeds;
    std::string const kSTS;
    std::string const kSTF;
    std::string const kFTW;
    std::string const kWTL;
    std::string const kLTT;
    std::string const kTTH;
    std::string const kHTL;

};