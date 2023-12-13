#include <CubesBag.hpp>

CubesBag::CubesBag() :
    kRedCubes(12)
  , kGreenCubes(13)
  , kBlueCubes(14)
  , kDigits("0123456789")
  {}

bool CubesBag::possible(uint8_t redCubes = 0, uint8_t greenCubes = 0, uint8_t blueCubes = 0)
{
    return (redCubes <= kRedCubes) && (greenCubes <= kGreenCubes) && (blueCubes <= kBlueCubes);
}

bool CubesBag::analyzeGame(std::string game)
{
    std::vector<std::string> hands = Reader::splitString(game, ";");

    for(auto hand : hands)
    {
        std::vector<std::string> results = Reader::splitString(hand, " ");

        uint8_t redCubes = 0;
        uint8_t greenCubes = 0;
        uint8_t blueCubes = 0;
    }
}

int CubesBag::analyzeGames(std::vector<std::string> games)
{
    uint32_t sumIDs = 0;

    for(auto game : games)
    {
        std::string gameIDStr = game.substr(0, game.find(":"));
        size_t firstPos = gameIDStr.find_first_of(kDigits);
        size_t lastPos = gameIDStr.find_first_not_of(kDigits, firstPos);
        std::stringstream ss = std::stringstream(gameIDStr.substr(firstPos, lastPos));
        uint32_t gameID;
        ss >> gameID;

        std::string gameStr = game.substr(game.find(": "));
        if(true == analyzeGame(gameStr))
        {
            sumIDs += gameID;
        }
    }

    return sumIDs;
}